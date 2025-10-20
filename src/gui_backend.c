#include "gui_backend.h"

#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include <util/log.h>

#include "doppler_freq_correction.h"
#include "send_packet.h"
#include "serial_rotator.h"

#define GUI_BACKEND_PORT 1029
#define GUI_BACKEND_MAX_CLIENTS 8
#define GUI_BACKEND_BUFFER_SIZE 1024
#define GUI_BACKEND_MAX_EVENTS 64

#ifndef PATH_MAX
#define PATH_MAX 256
#endif

struct gui_backend_event
{
    gui_backend_event_type_t type;
    struct timespec timestamp;
    char origin[32];
    char summary[128];
    char detail[256];
};

struct gui_backend_client
{
    int fd;
    char buffer[GUI_BACKEND_BUFFER_SIZE];
    size_t size;
};

struct gui_backend_state
{
    pthread_mutex_t lock;
    struct gui_backend_event events[GUI_BACKEND_MAX_EVENTS];
    size_t start;
    size_t count;

    char last_uplink_file[PATH_MAX];
    size_t last_uplink_bytes;
    int last_uplink_success;
    char last_uplink_origin[32];

    char last_downlink_file[PATH_MAX];
    size_t last_downlink_bytes;
    char last_downlink_origin[32];
    uint8_t last_downlink_src;
    uint8_t last_downlink_dst;

    int last_rotator_azimuth;
    int last_rotator_elevation;
    int last_rotator_success;
    
    char station_name[64];
    double station_lat;
    double station_lon;
    double station_alt_m;
    double station_true_north_deg;
    gui_backend_mode_t station_mode;
    int emergency_stop_engaged;
    time_t last_status_utc;
    char last_status_local[32];
    struct {
        double az_deg;
        double el_deg;
        int last_command_success;
    } antenna;
    struct {
        int tx_freq_hz;
        int rx_freq_hz;
    } rf;
    struct {
        uint32_t norad_id;
        double lat_deg;
        double lon_deg;
        double alt_km;
        double velocity_km_s;
        double range_km;
        double range_rate_km_s;
        time_t tle_epoch;
    } satellite;
    struct {
        size_t count;
        gui_backend_pass_t passes[GUI_BACKEND_MAX_PASSES];
    } pass_schedule;
};

static struct gui_backend_state gui_state = {
    .lock = PTHREAD_MUTEX_INITIALIZER,
};

typedef struct
{
    double az_deg;
    double el_deg;
    int last_command_success;
} gui_backend_snapshot_antenna_t;

typedef struct
{
    int tx_freq_hz;
    int rx_freq_hz;
} gui_backend_snapshot_rf_t;

typedef struct
{
    uint32_t norad_id;
    double lat_deg;
    double lon_deg;
    double alt_km;
    double velocity_km_s;
    double range_km;
    double range_rate_km_s;
    time_t tle_epoch;
    long tle_age_sec;
} gui_backend_snapshot_satellite_t;

typedef struct
{
    char station_name[64];
    gui_backend_mode_t station_mode;
    int emergency_stop_engaged;
    double station_lat;
    double station_lon;
    double station_alt_m;
    double station_true_north_deg;
    char utc_iso[32];
    char local_iso[32];
    gui_backend_snapshot_antenna_t antenna;
    gui_backend_snapshot_rf_t rf;
    gui_backend_snapshot_satellite_t sat;
    char passes_json[512];
    char faults_json[512];
} gui_backend_snapshot_t;

static void *gui_backend_thread(void *arg);
static void gui_backend_push_event(gui_backend_event_type_t type, const char *origin,
                                   const char *summary, const char *detail);
static void gui_backend_handle_command(struct gui_backend_client *client, const char *line);
static void gui_backend_send(struct gui_backend_client *client, const char *data);
static void gui_backend_send_fmt(struct gui_backend_client *client, const char *fmt, ...);
static void gui_backend_process_buffer(struct gui_backend_client *client);
static void gui_backend_close_client(struct gui_backend_client *client);
static void gui_backend_format_time(const struct timespec *ts, char *buf, size_t buf_len);
static void gui_backend_snapshot(gui_backend_snapshot_t *snap);
static void gui_backend_send_status_payload(struct gui_backend_client *client,
                                            const gui_backend_snapshot_t *snap,
                                            const char *message_type,
                                            int include_header);
static void gui_backend_emit_status_to_all(struct gui_backend_client *clients,
                                           size_t client_count);
static const char *gui_backend_mode_to_string(gui_backend_mode_t mode);
static gui_backend_mode_t gui_backend_mode_from_string(const char *value);
static const char *gui_backend_event_type_to_string(gui_backend_event_type_t type);

int gui_backend_start(pthread_t *thread)
{
    if (thread == NULL)
    {
        return -1;
    }

    int ret = pthread_create(thread, NULL, gui_backend_thread, NULL);
    if (ret != 0)
    {
        log_error("[GUI Backend] Failed to start thread (%d)", ret);
        return -1;
    }

    log_info("[GUI Backend] Listening for GUI frontend connections on port %d", GUI_BACKEND_PORT);
    return 0;
}

void gui_backend_notify_uplink(const char *origin, const char *file_path, size_t bytes, int success)
{
    pthread_mutex_lock(&gui_state.lock);

    gui_state.last_uplink_bytes = bytes;
    gui_state.last_uplink_success = success;
    if (origin != NULL)
    {
        strncpy(gui_state.last_uplink_origin, origin, sizeof(gui_state.last_uplink_origin) - 1);
        gui_state.last_uplink_origin[sizeof(gui_state.last_uplink_origin) - 1] = '\0';
    }
    if (file_path != NULL)
    {
        strncpy(gui_state.last_uplink_file, file_path, sizeof(gui_state.last_uplink_file) - 1);
        gui_state.last_uplink_file[sizeof(gui_state.last_uplink_file) - 1] = '\0';
    }
    else
    {
        gui_state.last_uplink_file[0] = '\0';
    }

    pthread_mutex_unlock(&gui_state.lock);

    char detail[256];
    snprintf(detail, sizeof(detail), "%s bytes=%zu", success ? "success" : "failure", bytes);
    gui_backend_push_event(success ? GUI_BACKEND_EVENT_UPLINK : GUI_BACKEND_EVENT_ERROR,
                           origin != NULL ? origin : "UNKNOWN",
                           "Uplink transmission", detail);
}

void gui_backend_notify_downlink(const char *origin, const char *file_path, size_t bytes,
                                 uint8_t src, uint8_t dst)
{
    pthread_mutex_lock(&gui_state.lock);

    gui_state.last_downlink_bytes = bytes;
    gui_state.last_downlink_src = src;
    gui_state.last_downlink_dst = dst;
    if (origin != NULL)
    {
        strncpy(gui_state.last_downlink_origin, origin, sizeof(gui_state.last_downlink_origin) - 1);
        gui_state.last_downlink_origin[sizeof(gui_state.last_downlink_origin) - 1] = '\0';
    }
    if (file_path != NULL)
    {
        strncpy(gui_state.last_downlink_file, file_path, sizeof(gui_state.last_downlink_file) - 1);
        gui_state.last_downlink_file[sizeof(gui_state.last_downlink_file) - 1] = '\0';
    }
    else
    {
        gui_state.last_downlink_file[0] = '\0';
    }

    pthread_mutex_unlock(&gui_state.lock);

    char detail[256];
    snprintf(detail, sizeof(detail), "src=%u dst=%u bytes=%zu", src, dst, bytes);
    gui_backend_push_event(GUI_BACKEND_EVENT_DOWNLINK,
                           origin != NULL ? origin : "UNKNOWN",
                           "Downlink packet", detail);
}

void gui_backend_notify_rotator(int azimuth, int elevation, int success)
{
    pthread_mutex_lock(&gui_state.lock);
    gui_state.antenna.az_deg = azimuth;
    gui_state.antenna.el_deg = elevation;
    gui_state.antenna.last_command_success = success;
    gui_state.last_rotator_azimuth = azimuth;
    gui_state.last_rotator_elevation = elevation;
    gui_state.last_rotator_success = success;
    pthread_mutex_unlock(&gui_state.lock);

    char detail[128];
    snprintf(detail, sizeof(detail), "az=%d el=%d %s", azimuth, elevation,
             success ? "success" : "failed");
    gui_backend_push_event(success ? GUI_BACKEND_EVENT_INFO : GUI_BACKEND_EVENT_ERROR,
                           "ROTATOR", "Rotator command", detail);
}

void gui_backend_set_station_info(const char *name, double lat_deg, double lon_deg, double alt_m,
                                  double true_north_deg)
{
    time_t now = time(NULL);
    struct tm tm_local;
    char local_iso[sizeof(gui_state.last_status_local)] = "";
    if (localtime_r(&now, &tm_local) != NULL)
    {
        strftime(local_iso, sizeof(local_iso), "%Y-%m-%dT%H:%M:%S", &tm_local);
    }

    char applied_name[sizeof(gui_state.station_name)];
    pthread_mutex_lock(&gui_state.lock);
    if (name != NULL)
    {
        strncpy(gui_state.station_name, name, sizeof(gui_state.station_name) - 1);
        gui_state.station_name[sizeof(gui_state.station_name) - 1] = '\0';
    }
    strncpy(applied_name, gui_state.station_name, sizeof(applied_name) - 1);
    applied_name[sizeof(applied_name) - 1] = '\0';
    gui_state.station_lat = lat_deg;
    gui_state.station_lon = lon_deg;
    gui_state.station_alt_m = alt_m;
    gui_state.station_true_north_deg = true_north_deg;
    gui_state.last_status_utc = now;
    strncpy(gui_state.last_status_local, local_iso, sizeof(gui_state.last_status_local) - 1);
    gui_state.last_status_local[sizeof(gui_state.last_status_local) - 1] = '\0';
    pthread_mutex_unlock(&gui_state.lock);

    gui_backend_push_event(GUI_BACKEND_EVENT_INFO, "GUI", "Station info update", applied_name);
}

void gui_backend_set_mode(gui_backend_mode_t mode)
{
    gui_backend_mode_t previous;
    pthread_mutex_lock(&gui_state.lock);
    previous = gui_state.station_mode;
    gui_state.station_mode = mode;
    pthread_mutex_unlock(&gui_state.lock);

    if (previous != mode)
    {
        gui_backend_push_event(GUI_BACKEND_EVENT_INFO, "GUI", "Station mode",
                               gui_backend_mode_to_string(mode));
    }
}

void gui_backend_set_emergency_stop(int engaged)
{
    int previous;
    int new_value = engaged ? 1 : 0;
    pthread_mutex_lock(&gui_state.lock);
    previous = gui_state.emergency_stop_engaged;
    gui_state.emergency_stop_engaged = new_value;
    pthread_mutex_unlock(&gui_state.lock);

    if (previous != new_value)
    {
        gui_backend_push_event(new_value ? GUI_BACKEND_EVENT_ERROR : GUI_BACKEND_EVENT_INFO,
                               "GUI", "Emergency stop",
                               new_value ? "ENGAGED" : "CLEARED");
    }
}

void gui_backend_update_pass_schedule(const gui_backend_pass_t *passes, size_t count)
{
    if (passes == NULL)
    {
        return;
    }

    if (count > GUI_BACKEND_MAX_PASSES)
    {
        count = GUI_BACKEND_MAX_PASSES;
    }

    pthread_mutex_lock(&gui_state.lock);
    gui_state.pass_schedule.count = count;
    for (size_t i = 0; i < count; ++i)
    {
        gui_state.pass_schedule.passes[i] = passes[i];
    }
    pthread_mutex_unlock(&gui_state.lock);
}

void gui_backend_update_satellite(const gui_backend_satellite_t *satellite)
{
    if (satellite == NULL)
    {
        return;
    }

    pthread_mutex_lock(&gui_state.lock);
    gui_state.satellite.norad_id = satellite->norad_id;
    gui_state.satellite.lat_deg = satellite->lat_deg;
    gui_state.satellite.lon_deg = satellite->lon_deg;
    gui_state.satellite.alt_km = satellite->alt_km;
    gui_state.satellite.velocity_km_s = satellite->velocity_km_s;
    gui_state.satellite.range_km = satellite->range_km;
    gui_state.satellite.range_rate_km_s = satellite->range_rate_km_s;
    gui_state.satellite.tle_epoch = satellite->tle_epoch;
    pthread_mutex_unlock(&gui_state.lock);
}

static void gui_backend_push_event(gui_backend_event_type_t type, const char *origin,
                                   const char *summary, const char *detail)
{
    struct gui_backend_event event;
    clock_gettime(CLOCK_REALTIME, &event.timestamp);
    event.type = type;
    strncpy(event.origin, origin != NULL ? origin : "UNKNOWN", sizeof(event.origin) - 1);
    event.origin[sizeof(event.origin) - 1] = '\0';
    strncpy(event.summary, summary != NULL ? summary : "", sizeof(event.summary) - 1);
    event.summary[sizeof(event.summary) - 1] = '\0';
    if (detail != NULL)
    {
        strncpy(event.detail, detail, sizeof(event.detail) - 1);
        event.detail[sizeof(event.detail) - 1] = '\0';
    }
    else
    {
        event.detail[0] = '\0';
    }

    pthread_mutex_lock(&gui_state.lock);
    size_t index = (gui_state.start + gui_state.count) % GUI_BACKEND_MAX_EVENTS;
    gui_state.events[index] = event;
    if (gui_state.count < GUI_BACKEND_MAX_EVENTS)
    {
        gui_state.count++;
    }
    else
    {
        gui_state.start = (gui_state.start + 1) % GUI_BACKEND_MAX_EVENTS;
    }
    pthread_mutex_unlock(&gui_state.lock);
}

static void gui_backend_format_time(const struct timespec *ts, char *buf, size_t buf_len)
{
    struct tm tm_time;
    time_t seconds = ts->tv_sec;
    gmtime_r(&seconds, &tm_time);
    strftime(buf, buf_len, "%Y-%m-%dT%H:%M:%SZ", &tm_time);
}

static void gui_backend_send(struct gui_backend_client *client, const char *data)
{
    size_t len = strlen(data);
    const char *ptr = data;
    while (len > 0)
    {
        ssize_t written = send(client->fd, ptr, len, 0);
        if (written <= 0)
        {
            break;
        }
        ptr += written;
        len -= written;
    }
}

static void gui_backend_send_fmt(struct gui_backend_client *client, const char *fmt, ...)
{
    char buffer[GUI_BACKEND_BUFFER_SIZE];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, ap);
    va_end(ap);
    gui_backend_send(client, buffer);
}

static void gui_backend_snapshot(gui_backend_snapshot_t *snap)
{
    if (snap == NULL)
    {
        return;
    }

    memset(snap, 0, sizeof(*snap));

    gui_backend_pass_t passes[GUI_BACKEND_MAX_PASSES];
    size_t pass_count = 0;
    gui_backend_snapshot_satellite_t sat_state = {0};
    gui_backend_snapshot_antenna_t antenna_state = {0};
    gui_backend_snapshot_rf_t rf_state = {0};
    gui_backend_mode_t station_mode = GUI_BACKEND_MODE_IDLE;
    int emergency_stop_engaged = 0;
    double station_lat = 0.0;
    double station_lon = 0.0;
    double station_alt_m = 0.0;
    double station_true_north = 0.0;
    char station_name[sizeof(gui_state.station_name)] = "";
    time_t last_status_utc = 0;
    char last_status_local[sizeof(gui_state.last_status_local)] = "";

    pthread_mutex_lock(&gui_state.lock);
    pass_count = gui_state.pass_schedule.count;
    if (pass_count > GUI_BACKEND_MAX_PASSES)
    {
        pass_count = GUI_BACKEND_MAX_PASSES;
    }
    if (pass_count > 0)
    {
        memcpy(passes, gui_state.pass_schedule.passes,
               pass_count * sizeof(gui_backend_pass_t));
    }
    sat_state.norad_id = gui_state.satellite.norad_id;
    sat_state.lat_deg = gui_state.satellite.lat_deg;
    sat_state.lon_deg = gui_state.satellite.lon_deg;
    sat_state.alt_km = gui_state.satellite.alt_km;
    sat_state.velocity_km_s = gui_state.satellite.velocity_km_s;
    sat_state.range_km = gui_state.satellite.range_km;
    sat_state.range_rate_km_s = gui_state.satellite.range_rate_km_s;
    sat_state.tle_epoch = gui_state.satellite.tle_epoch;

    antenna_state = (gui_backend_snapshot_antenna_t){
        .az_deg = gui_state.antenna.az_deg,
        .el_deg = gui_state.antenna.el_deg,
        .last_command_success = gui_state.antenna.last_command_success,
    };
    rf_state = (gui_backend_snapshot_rf_t){
        .tx_freq_hz = gui_state.rf.tx_freq_hz,
        .rx_freq_hz = gui_state.rf.rx_freq_hz,
    };
    station_mode = gui_state.station_mode;
    emergency_stop_engaged = gui_state.emergency_stop_engaged;
    station_lat = gui_state.station_lat;
    station_lon = gui_state.station_lon;
    station_alt_m = gui_state.station_alt_m;
    station_true_north = gui_state.station_true_north_deg;
    strncpy(station_name, gui_state.station_name, sizeof(station_name) - 1);
    station_name[sizeof(station_name) - 1] = '\0';
    last_status_utc = gui_state.last_status_utc;
    strncpy(last_status_local, gui_state.last_status_local,
            sizeof(last_status_local) - 1);
    last_status_local[sizeof(last_status_local) - 1] = '\0';
    pthread_mutex_unlock(&gui_state.lock);

    time_t now = time(NULL);
    sat_state.tle_age_sec = (sat_state.tle_epoch > 0)
                                ? (long)difftime(now, sat_state.tle_epoch)
                                : 0;

    strncpy(snap->station_name, station_name, sizeof(snap->station_name) - 1);
    snap->station_name[sizeof(snap->station_name) - 1] = '\0';
    snap->station_mode = station_mode;
    snap->emergency_stop_engaged = emergency_stop_engaged;
    snap->station_lat = station_lat;
    snap->station_lon = station_lon;
    snap->station_alt_m = station_alt_m;
    snap->station_true_north_deg = station_true_north;
    snap->antenna = antenna_state;
    snap->rf = rf_state;
    snap->sat = sat_state;

    time_t utc_ref = (last_status_utc != 0) ? last_status_utc : now;
    struct tm tm_utc;
    if (gmtime_r(&utc_ref, &tm_utc) != NULL)
    {
        strftime(snap->utc_iso, sizeof(snap->utc_iso), "%Y-%m-%dT%H:%M:%SZ", &tm_utc);
    }
    else
    {
        snap->utc_iso[0] = '\0';
    }

    if (last_status_local[0] != '\0')
    {
        strncpy(snap->local_iso, last_status_local, sizeof(snap->local_iso) - 1);
        snap->local_iso[sizeof(snap->local_iso) - 1] = '\0';
    }
    else
    {
        struct tm tm_local;
        if (localtime_r(&utc_ref, &tm_local) != NULL)
        {
            strftime(snap->local_iso, sizeof(snap->local_iso), "%Y-%m-%dT%H:%M:%S",
                     &tm_local);
        }
        else
        {
            snap->local_iso[0] = '\0';
        }
    }

    /* Build pass schedule JSON */
    size_t offset = 0;
    size_t remaining = sizeof(snap->passes_json);
    if (remaining > 0)
    {
        offset = snprintf(snap->passes_json, remaining, "[");
        if (offset >= remaining)
        {
            snap->passes_json[sizeof(snap->passes_json) - 1] = '\0';
        }
        else
        {
            for (size_t i = 0; i < pass_count && offset < remaining; ++i)
            {
                char aos_iso[32] = "";
                char los_iso[32] = "";
                struct tm tm_pass;
                if (gmtime_r(&passes[i].aos_utc, &tm_pass) != NULL)
                {
                    strftime(aos_iso, sizeof(aos_iso), "%Y-%m-%dT%H:%M:%SZ", &tm_pass);
                }
                if (gmtime_r(&passes[i].los_utc, &tm_pass) != NULL)
                {
                    strftime(los_iso, sizeof(los_iso), "%Y-%m-%dT%H:%M:%SZ", &tm_pass);
                }

                int written = snprintf(
                    snap->passes_json + offset,
                    remaining - offset,
                    "{\"name\":\"%s\",\"aos\":\"%s\",\"los\":\"%s\",\"duration_sec\":%u,"
                    "\"peak_elevation_deg\":%u}%s",
                    passes[i].name,
                    aos_iso,
                    los_iso,
                    passes[i].duration_sec,
                    passes[i].peak_elevation_deg,
                    (i + 1 < pass_count) ? "," : "");
                if (written < 0 || (size_t)written >= remaining - offset)
                {
                    offset = remaining - 1;
                    break;
                }
                offset += (size_t)written;
            }
            if (offset < remaining)
            {
                if (pass_count > 0)
                {
                    int append = snprintf(snap->passes_json + offset,
                                          remaining - offset, "]");
                    if (append > 0)
                    {
                        offset += (size_t)append;
                    }
                }
                else
                {
                    snprintf(snap->passes_json + offset, remaining - offset, "]");
                }
            }
            snap->passes_json[sizeof(snap->passes_json) - 1] = '\0';
        }
    }

    if (pass_count == 0)
    {
        strncpy(snap->passes_json, "[]", sizeof(snap->passes_json) - 1);
        snap->passes_json[sizeof(snap->passes_json) - 1] = '\0';
    }
    else
    {
        if (strchr(snap->passes_json, ']') == NULL)
        {
            strncpy(snap->passes_json, "[]", sizeof(snap->passes_json) - 1);
            snap->passes_json[sizeof(snap->passes_json) - 1] = '\0';
        }
    }

    strncpy(snap->faults_json, "[]", sizeof(snap->faults_json) - 1);
    snap->faults_json[sizeof(snap->faults_json) - 1] = '\0';
}

static void gui_backend_send_status_payload(struct gui_backend_client *client,
                                            const gui_backend_snapshot_t *snap,
                                            const char *message_type,
                                            int include_header)
{
    if (client == NULL || snap == NULL || message_type == NULL)
    {
        return;
    }

    if (include_header)
    {
        gui_backend_send(client, "OK STATUS\n");
    }

    gui_backend_send_fmt(
        client,
        "{\"type\":\"%s\",\"station\":{\"name\":\"%s\",\"mode\":\"%s\","
        "\"emergency_stop\":%s,\"lat\":%.6f,\"lon\":%.6f,\"alt_m\":%.1f,"
        "\"true_north_deg\":%.2f,\"time_utc\":\"%s\",\"time_local\":\"%s\"},"
        "\"antenna\":{\"az_deg\":%.2f,\"el_deg\":%.2f,\"last_command_success\":%s},"
        "\"rf\":{\"tx_hz\":%d,\"rx_hz\":%d},"
        "\"satellite\":{\"norad\":%u,\"lat_deg\":%.3f,\"lon_deg\":%.3f,"
        "\"alt_km\":%.2f,\"velocity_km_s\":%.3f,\"range_km\":%.2f,"
        "\"range_rate_km_s\":%.3f,\"tle_age_sec\":%ld},"
        "\"passes\":%s,"
        "\"faults\":%s}\n",
        message_type,
        snap->station_name,
        gui_backend_mode_to_string(snap->station_mode),
        snap->emergency_stop_engaged ? "true" : "false",
        snap->station_lat, snap->station_lon, snap->station_alt_m,
        snap->station_true_north_deg, snap->utc_iso, snap->local_iso,
        snap->antenna.az_deg, snap->antenna.el_deg,
        snap->antenna.last_command_success ? "true" : "false",
        snap->rf.tx_freq_hz, snap->rf.rx_freq_hz,
        snap->sat.norad_id, snap->sat.lat_deg, snap->sat.lon_deg,
        snap->sat.alt_km, snap->sat.velocity_km_s,
        snap->sat.range_km, snap->sat.range_rate_km_s,
        snap->sat.tle_age_sec,
        snap->passes_json,
        snap->faults_json);

    if (include_header)
    {
        gui_backend_send(client, "END\n");
    }
}

static void gui_backend_emit_status_to_all(struct gui_backend_client *clients,
                                           size_t client_count)
{
    if (clients == NULL || client_count == 0)
    {
        return;
    }

    gui_backend_snapshot_t snap;
    gui_backend_snapshot(&snap);
    for (size_t i = 0; i < client_count; ++i)
    {
        if (clients[i].fd >= 0)
        {
            gui_backend_send_status_payload(&clients[i], &snap, "telemetry", 0);
        }
    }
}

static void gui_backend_print_help(struct gui_backend_client *client)
{
    gui_backend_send(client, "OK HELP\n");
    gui_backend_send(client, "PING - check connectivity\n");
    gui_backend_send(client, "STATUS - retrieve current backend status\n");
    gui_backend_send(client, "SET_SAT <id> - select satellite number\n");
    gui_backend_send(client, "SET_TX <freq_hz> - set transmitter frequency\n");
    gui_backend_send(client, "SET_RX <freq_hz> - set receiver frequency\n");
    gui_backend_send(client, "SET_AZEL <az_deg> <el_deg> - move antenna\n");
    gui_backend_send(client, "SEND_PACKET <pri> <src> <dst> <dst_port> <src_port> <hmac> <xtea> <rdp> <crc> <hex_payload>\n");
    gui_backend_send(client, "LAST_UPLINK - show latest uplink summary\n");
    gui_backend_send(client, "LAST_DOWNLINK - show latest downlink summary\n");
    gui_backend_send(client, "GET_EVENTS [count] - list recent telemetry events\n");
    gui_backend_send(client, "END\n");
}

static int gui_backend_parse_long(const char *token, long min, long max, long *value)
{
    char *endptr = NULL;
    errno = 0;
    long parsed = strtol(token, &endptr, 0);
    if (errno != 0 || endptr == token || *endptr != '\0')
    {
        return -1;
    }
    if (parsed < min || parsed > max)
    {
        return -1;
    }
    *value = parsed;
    return 0;
}

static void gui_backend_handle_status(struct gui_backend_client *client)
{
    gui_backend_snapshot_t snap;
    gui_backend_snapshot(&snap);
    gui_backend_send_status_payload(client, &snap, "status", 1);
}

static void gui_backend_handle_set_sat(struct gui_backend_client *client, const char *token)
{
    if (token == NULL)
    {
        gui_backend_send(client, "ERROR Missing satellite id\n");
        return;
    }
    long value = 0;
    if (gui_backend_parse_long(token, 1, 0xFF, &value) != 0)
    {
        gui_backend_send(client, "ERROR Invalid satellite id\n");
        return;
    }
    if (!mcs_sat_sel((uint32_t)value))
    {
        gui_backend_send(client, "ERROR Unable to select satellite\n");
        return;
    }
    gui_backend_push_event(GUI_BACKEND_EVENT_INFO, "GUI", "Satellite selection", token);
    gui_backend_send_fmt(client, "OK SATELLITE %ld\n", value);
}

static void gui_backend_handle_set_freq(struct gui_backend_client *client, const char *token,
                                        int is_tx)
{
    if (token == NULL)
    {
        gui_backend_send(client, "ERROR Missing frequency\n");
        return;
    }
    long value = 0;
    if (gui_backend_parse_long(token, 0, LONG_MAX, &value) != 0)
    {
        gui_backend_send(client, "ERROR Invalid frequency\n");
        return;
    }

    int success = 0;
    if (is_tx)
    {
        success = doppler_set_tx_freq((uint32_t)value);
    }
    else
    {
        success = doppler_set_rx_freq((uint32_t)value);
    }

    if (!success)
    {
        gui_backend_send(client, "ERROR Frequency configuration failed\n");
        return;
    }

    pthread_mutex_lock(&gui_state.lock);
    if (is_tx)
    {
        gui_state.rf.tx_freq_hz = (int)value;
    }
    else
    {
        gui_state.rf.rx_freq_hz = (int)value;
    }
    pthread_mutex_unlock(&gui_state.lock);

    gui_backend_push_event(GUI_BACKEND_EVENT_INFO, "GUI",
                           is_tx ? "Set TX frequency" : "Set RX frequency", token);
    gui_backend_send_fmt(client, "OK %s %ld\n", is_tx ? "TX" : "RX", value);
}

static void gui_backend_handle_set_azel(struct gui_backend_client *client,
                                           const char *az_token, const char *el_token)
{
    if (az_token == NULL || el_token == NULL)
    {
        gui_backend_send(client, "ERROR Missing azimuth/elevation\n");
        return;
    }
    long azimuth = 0;
    long elevation = 0;
    if (gui_backend_parse_long(az_token, -360, 360, &azimuth) != 0 ||
        gui_backend_parse_long(el_token, -90, 180, &elevation) != 0)
    {
        gui_backend_send(client, "ERROR Invalid azimuth/elevation\n");
        return;
    }
    printf("[GUI_backend] serial_set_az_el(%ld, %ld)\n", azimuth, elevation);

    serial_set_az_el((int)azimuth, (int)elevation);

}

static int gui_backend_hex_to_bytes(const char *hex, uint8_t **buffer_out, size_t *length_out)
{
    size_t len = strlen(hex);
    if (len % 2 != 0 || len == 0)
    {
        return -1;
    }
    size_t bytes = len / 2;
    uint8_t *buffer = malloc(bytes);
    if (buffer == NULL)
    {
        return -1;
    }
    for (size_t i = 0; i < bytes; ++i)
    {
        int high = toupper((unsigned char)hex[2 * i]);
        int low = toupper((unsigned char)hex[2 * i + 1]);
        if (!isxdigit(high) || !isxdigit(low))
        {
            free(buffer);
            return -1;
        }
        high = (high <= '9') ? high - '0' : high - 'A' + 10;
        low = (low <= '9') ? low - '0' : low - 'A' + 10;
        buffer[i] = (uint8_t)((high << 4) | low);
    }
    *buffer_out = buffer;
    *length_out = bytes;
    return 0;
}

static void gui_backend_handle_send_packet(struct gui_backend_client *client, char **tokens,
                                           int token_count)
{
    if (token_count < 11)
    {
        gui_backend_send(client, "ERROR Missing packet parameters\n");
        return;
    }

    long pri, src, dst, dst_port, src_port, hmac, xtea, rdp, crc;
    if (gui_backend_parse_long(tokens[1], 0, 3, &pri) != 0 ||
        gui_backend_parse_long(tokens[2], 0, 255, &src) != 0 ||
        gui_backend_parse_long(tokens[3], 0, 255, &dst) != 0 ||
        gui_backend_parse_long(tokens[4], 0, 63, &dst_port) != 0 ||
        gui_backend_parse_long(tokens[5], 0, 63, &src_port) != 0 ||
        gui_backend_parse_long(tokens[6], 0, 1, &hmac) != 0 ||
        gui_backend_parse_long(tokens[7], 0, 1, &xtea) != 0 ||
        gui_backend_parse_long(tokens[8], 0, 1, &rdp) != 0 ||
        gui_backend_parse_long(tokens[9], 0, 1, &crc) != 0)
    {
        gui_backend_send(client, "ERROR Invalid header parameters\n");
        return;
    }

    const char *hex_payload = tokens[10];
    uint8_t *payload = NULL;
    size_t payload_len = 0;
    if (gui_backend_hex_to_bytes(hex_payload, &payload, &payload_len) != 0)
    {
        gui_backend_send(client, "ERROR Invalid payload\n");
        return;
    }

    struct mcs_packet_header {
        uint8_t priority;
        uint8_t src;
        uint8_t dst;
        uint8_t dst_port;
        uint8_t src_port;
        uint8_t hmac;
        uint8_t xtea;
        uint8_t rdp;
        uint8_t crc;
    } header = {
        .priority = (uint8_t)pri,
        .src = (uint8_t)src,
        .dst = (uint8_t)dst,
        .dst_port = (uint8_t)dst_port,
        .src_port = (uint8_t)src_port,
        .hmac = (uint8_t)hmac,
        .xtea = (uint8_t)xtea,
        .rdp = (uint8_t)rdp,
        .crc = (uint8_t)crc,
    };

    int status = send_packet_struct("GUI", &header, payload, payload_len);
    free(payload);
    if (status != 0)
    {
        gui_backend_send(client, "ERROR Packet transmission failed\n");
        return;
    }

    gui_backend_send_fmt(client, "OK SEND_PACKET %zu\n", payload_len);
}

static void gui_backend_handle_get_events(struct gui_backend_client *client, const char *token)
{
    size_t limit = GUI_BACKEND_MAX_EVENTS;
    if (token != NULL)
    {
        long value = 0;
        if (gui_backend_parse_long(token, 1, GUI_BACKEND_MAX_EVENTS, &value) != 0)
        {
            gui_backend_send(client, "ERROR Invalid event count\n");
            return;
        }
        limit = (size_t)value;
    }

    struct gui_backend_event events[GUI_BACKEND_MAX_EVENTS];
    size_t count = 0;

    pthread_mutex_lock(&gui_state.lock);
    size_t total = gui_state.count;
    if (limit < total)
    {
        total = limit;
    }
    for (size_t i = 0; i < total; ++i)
    {
        size_t index = (gui_state.start + gui_state.count - total + i) % GUI_BACKEND_MAX_EVENTS;
        events[i] = gui_state.events[index];
    }
    count = total;
    pthread_mutex_unlock(&gui_state.lock);

    gui_backend_send_fmt(client, "OK EVENTS %zu\n", count);
    for (size_t i = 0; i < count; ++i)
    {
        char timestamp[32];
        gui_backend_format_time(&events[i].timestamp, timestamp, sizeof(timestamp));
        gui_backend_send_fmt(client, "%s %s %s | %s (%s)\n",
                             timestamp,
                             gui_backend_event_type_to_string(events[i].type),
                             events[i].origin,
                             events[i].summary,
                             events[i].detail);
    }
    gui_backend_send(client, "END\n");
}

static void gui_backend_handle_last_uplink(struct gui_backend_client *client)
{
    char file[PATH_MAX];
    char origin[32];
    size_t bytes;
    int success;

    pthread_mutex_lock(&gui_state.lock);
    strncpy(file, gui_state.last_uplink_file, sizeof(file));
    file[sizeof(file) - 1] = '\0';
    strncpy(origin, gui_state.last_uplink_origin, sizeof(origin));
    origin[sizeof(origin) - 1] = '\0';
    bytes = gui_state.last_uplink_bytes;
    success = gui_state.last_uplink_success;
    pthread_mutex_unlock(&gui_state.lock);

    gui_backend_send_fmt(client, "OK LAST_UPLINK origin=%s bytes=%zu status=%s file=%s\n",
                         origin, bytes, success ? "success" : "failure", file);
}

static void gui_backend_handle_last_downlink(struct gui_backend_client *client)
{
    char file[PATH_MAX];
    char origin[32];
    size_t bytes;
    uint8_t src;
    uint8_t dst;

    pthread_mutex_lock(&gui_state.lock);
    strncpy(file, gui_state.last_downlink_file, sizeof(file));
    file[sizeof(file) - 1] = '\0';
    strncpy(origin, gui_state.last_downlink_origin, sizeof(origin));
    origin[sizeof(origin) - 1] = '\0';
    bytes = gui_state.last_downlink_bytes;
    src = gui_state.last_downlink_src;
    dst = gui_state.last_downlink_dst;
    pthread_mutex_unlock(&gui_state.lock);

    gui_backend_send_fmt(client, "OK LAST_DOWNLINK origin=%s bytes=%zu src=%u dst=%u file=%s\n",
                         origin, bytes, src, dst, file);
}

static void gui_backend_handle_command(struct gui_backend_client *client, const char *line)
{
    char command[GUI_BACKEND_BUFFER_SIZE];
    strncpy(command, line, sizeof(command) - 1);
    command[sizeof(command) - 1] = '\0';

    char *tokens[16];
    int token_count = 0;
    char *saveptr = NULL;
    char *token = strtok_r(command, " ", &saveptr);
    while (token != NULL && token_count < (int)(sizeof(tokens) / sizeof(tokens[0])))
    {
        tokens[token_count++] = token;
        token = strtok_r(NULL, " ", &saveptr);
    }

    if (token_count == 0)
    {
        return;
    }

    const char *cmd = tokens[0];

    if (strcasecmp(cmd, "PING") == 0)
    {
        gui_backend_send(client, "OK PONG\n");
    }
    else if (strcasecmp(cmd, "HELP") == 0)
    {
        gui_backend_print_help(client);
    }
    else if (strcasecmp(cmd, "STATUS") == 0)
    {
        gui_backend_handle_status(client);
    }
    else if (strcasecmp(cmd, "SET_MODE") == 0)
    {
        if (token_count <= 1)
        {
            gui_backend_send(client, "ERROR Missing mode\n");
            return;
        }
        gui_backend_set_mode(gui_backend_mode_from_string(tokens[1]));
        gui_backend_send_fmt(client, "OK SET_MODE %s\n", tokens[1]);
    }
    else if (strcasecmp(cmd, "SET_EMERGENCY") == 0)
    {
        if (token_count <= 1)
        {
            gui_backend_send(client, "ERROR Missing emergency state\n");
            return;
        }
        int engage = (strcasecmp(tokens[1], "true") == 0 ||
                      strcasecmp(tokens[1], "1") == 0 ||
                      strcasecmp(tokens[1], "on") == 0);
        gui_backend_set_emergency_stop(engage);
        gui_backend_send_fmt(client, "OK SET_EMERGENCY %s\n", engage ? "true" : "false");
    }
    else if (strcasecmp(cmd, "SET_SAT") == 0 || strcasecmp(cmd, "SET_SATELLITE") == 0)
    {
        gui_backend_handle_set_sat(client, token_count > 1 ? tokens[1] : NULL);
    }
    else if (strcasecmp(cmd, "SET_TX") == 0)
    {
        gui_backend_handle_set_freq(client, token_count > 1 ? tokens[1] : NULL, 1);
    }
    else if (strcasecmp(cmd, "SET_RX") == 0)
    {
        gui_backend_handle_set_freq(client, token_count > 1 ? tokens[1] : NULL, 0);
    }
    else if (strcasecmp(cmd, "SET_AZEL") == 0)
    {
        gui_backend_handle_set_azel(client,
                                       token_count > 1 ? tokens[1] : NULL,
                                       token_count > 2 ? tokens[2] : NULL);
    }
    else if (strcasecmp(cmd, "SEND_PACKET") == 0)
    {
        gui_backend_handle_send_packet(client, tokens, token_count);
    }
    else if (strcasecmp(cmd, "GET_EVENTS") == 0)
    {
        gui_backend_handle_get_events(client, token_count > 1 ? tokens[1] : NULL);
    }
    else if (strcasecmp(cmd, "LAST_UPLINK") == 0)
    {
        gui_backend_handle_last_uplink(client);
    }
    else if (strcasecmp(cmd, "LAST_DOWNLINK") == 0)
    {
        gui_backend_handle_last_downlink(client);
    }
    else
    {
        gui_backend_send(client, "ERROR Unknown command\n");
    }
}

static void gui_backend_process_buffer(struct gui_backend_client *client)
{
    size_t processed = 0;
    while (processed < client->size)
    {
        char *newline = memchr(client->buffer + processed, '\n', client->size - processed);
        if (newline == NULL)
        {
            break;
        }
        size_t line_length = (size_t)(newline - (client->buffer + processed));
        if (line_length >= GUI_BACKEND_BUFFER_SIZE)
        {
            line_length = GUI_BACKEND_BUFFER_SIZE - 1;
        }
        char line[GUI_BACKEND_BUFFER_SIZE];
        memcpy(line, client->buffer + processed, line_length);
        line[line_length] = '\0';
        if (line_length > 0 && line[line_length - 1] == '\r')
        {
            line[line_length - 1] = '\0';
        }
        gui_backend_handle_command(client, line);
        processed = (newline - client->buffer) + 1;
    }

    if (processed > 0)
    {
        memmove(client->buffer, client->buffer + processed, client->size - processed);
        client->size -= processed;
    }
}

static void gui_backend_close_client(struct gui_backend_client *client)
{
    if (client->fd >= 0)
    {
        close(client->fd);
        client->fd = -1;
        client->size = 0;
    }
}

static void *gui_backend_thread(void *arg)
{
    (void)arg;
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        log_error("[GUI Backend] Unable to create socket: %s", strerror(errno));
        return NULL;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
   addr.sin_family = AF_INET;
   addr.sin_addr.s_addr = INADDR_ANY;
   addr.sin_port = htons(GUI_BACKEND_PORT);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        log_error("[GUI Backend] Unable to bind socket: %s", strerror(errno));
        close(server_fd);
        return NULL;
    }

    if (listen(server_fd, GUI_BACKEND_MAX_CLIENTS) < 0)
    {
        log_error("[GUI Backend] Unable to listen on socket: %s", strerror(errno));
        close(server_fd);
        return NULL;
    }

    struct gui_backend_client clients[GUI_BACKEND_MAX_CLIENTS];
    for (size_t i = 0; i < GUI_BACKEND_MAX_CLIENTS; ++i)
    {
        clients[i].fd = -1;
        clients[i].size = 0;
    }

    time_t last_broadcast = 0;

    while (1)
    {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        int max_fd = server_fd;

        for (size_t i = 0; i < GUI_BACKEND_MAX_CLIENTS; ++i)
        {
            if (clients[i].fd >= 0)
            {
                FD_SET(clients[i].fd, &readfds);
                if (clients[i].fd > max_fd)
                {
                    max_fd = clients[i].fd;
                }
            }
        }

        struct timeval timeout = {
            .tv_sec = 1,
            .tv_usec = 0,
        };

        int ready = select(max_fd + 1, &readfds, NULL, NULL, &timeout);

        time_t now = time(NULL);
        if (difftime(now, last_broadcast) >= 1.0)
        {
            gui_backend_emit_status_to_all(clients, GUI_BACKEND_MAX_CLIENTS);
            last_broadcast = now;
        }

        if (ready == 0)
        {
            continue;
        }

        if (ready < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            log_error("[GUI Backend] select() failed: %s", strerror(errno));
            break;
        }

        if (FD_ISSET(server_fd, &readfds))
        {
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
            if (client_fd < 0)
            {
                log_error("[GUI Backend] accept() failed: %s", strerror(errno));
            }
            else
            {
                size_t slot;
                for (slot = 0; slot < GUI_BACKEND_MAX_CLIENTS; ++slot)
                {
                    if (clients[slot].fd < 0)
                    {
                        clients[slot].fd = client_fd;
                        clients[slot].size = 0;
                        gui_backend_send_fmt(&clients[slot],
                                             "OK CONNECTED gui-backend %s\n",
                                             inet_ntoa(client_addr.sin_addr));
                        break;
                    }
                }
                if (slot == GUI_BACKEND_MAX_CLIENTS)
                {
                    log_warning("[GUI Backend] Too many clients, rejecting connection");
                    close(client_fd);
                }
            }
        }

        for (size_t i = 0; i < GUI_BACKEND_MAX_CLIENTS; ++i)
        {
            if (clients[i].fd >= 0 && FD_ISSET(clients[i].fd, &readfds))
            {
                ssize_t received = recv(clients[i].fd,
                                        clients[i].buffer + clients[i].size,
                                        sizeof(clients[i].buffer) - clients[i].size,
                                        0);
                if (received <= 0)
                {
                    gui_backend_close_client(&clients[i]);
                    continue;
                }
                clients[i].size += (size_t)received;
                if (clients[i].size >= sizeof(clients[i].buffer))
                {
                    clients[i].size = 0; // overflow, reset buffer
                    gui_backend_send(&clients[i], "ERROR Input too long\n");
                }
                else
                {
                    gui_backend_process_buffer(&clients[i]);
                }
            }
        }
    }

    close(server_fd);
    for (size_t i = 0; i < GUI_BACKEND_MAX_CLIENTS; ++i)
    {
        gui_backend_close_client(&clients[i]);
    }
    return NULL;
}

static const char *gui_backend_mode_to_string(gui_backend_mode_t mode)
{
    switch (mode)
    {
        case GUI_BACKEND_MODE_IDLE:
            return "IDLE";
        case GUI_BACKEND_MODE_TRACKING:
            return "TRACKING";
        case GUI_BACKEND_MODE_MAINTENANCE:
            return "MAINTENANCE";
        default:
            return "UNKNOWN";
    }
}

static gui_backend_mode_t gui_backend_mode_from_string(const char *value)
{
    if (value == NULL)
    {
        return GUI_BACKEND_MODE_IDLE;
    }

    if (strcasecmp(value, "tracking") == 0)
    {
        return GUI_BACKEND_MODE_TRACKING;
    }
    if (strcasecmp(value, "maintenance") == 0 ||
        strcasecmp(value, "maint") == 0)
    {
        return GUI_BACKEND_MODE_MAINTENANCE;
    }
    if (strcasecmp(value, "idle") == 0)
    {
        return GUI_BACKEND_MODE_IDLE;
    }

    return GUI_BACKEND_MODE_IDLE;
}

static const char *gui_backend_event_type_to_string(gui_backend_event_type_t type)
{
    switch (type)
    {
        case GUI_BACKEND_EVENT_UPLINK:
            return "UPLINK";
        case GUI_BACKEND_EVENT_DOWNLINK:
            return "DOWNLINK";
        case GUI_BACKEND_EVENT_INFO:
            return "INFO";
        case GUI_BACKEND_EVENT_ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}
