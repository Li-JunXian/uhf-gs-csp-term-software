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
#include <sys/select.h>
#include <sys/socket.h>
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
};

static struct gui_backend_state gui_state = {
    .lock = PTHREAD_MUTEX_INITIALIZER,
};

static void *gui_backend_thread(void *arg);
static void gui_backend_push_event(gui_backend_event_type_t type, const char *origin,
                                   const char *summary, const char *detail);
static void gui_backend_handle_command(struct gui_backend_client *client, const char *line);
static void gui_backend_send(struct gui_backend_client *client, const char *data);
static void gui_backend_send_fmt(struct gui_backend_client *client, const char *fmt, ...);
static void gui_backend_process_buffer(struct gui_backend_client *client);
static void gui_backend_close_client(struct gui_backend_client *client);
static void gui_backend_format_time(const struct timespec *ts, char *buf, size_t buf_len);

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

static void gui_backend_print_help(struct gui_backend_client *client)
{
    gui_backend_send(client, "OK HELP\n");
    gui_backend_send(client, "PING - check connectivity\n");
    gui_backend_send(client, "STATUS - retrieve current backend status\n");
    gui_backend_send(client, "SET_SAT <id> - select satellite number\n");
    gui_backend_send(client, "SET_TX <freq_hz> - set transmitter frequency\n");
    gui_backend_send(client, "SET_RX <freq_hz> - set receiver frequency\n");
    gui_backend_send(client, "SET_ROTATOR <az_deg> <el_deg> - move antenna\n");
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
    int sat_no = mcs_sat_read();
    int tx_freq = doppler_get_tx_freq();
    int rx_freq = doppler_get_rx_freq();
    char tle1[128] = "";
    char tle2[128] = "";
    doppler_get_tle(tle1, sizeof(tle1), tle2, sizeof(tle2));

    char uplink_file[PATH_MAX];
    char uplink_origin[32];
    size_t uplink_bytes;
    int uplink_success;

    char downlink_file[PATH_MAX];
    char downlink_origin[32];
    size_t downlink_bytes;
    uint8_t downlink_src;
    uint8_t downlink_dst;

    int rot_az;
    int rot_el;
    int rot_success;

    pthread_mutex_lock(&gui_state.lock);
    strncpy(uplink_file, gui_state.last_uplink_file, sizeof(uplink_file));
    uplink_file[sizeof(uplink_file) - 1] = '\0';
    strncpy(uplink_origin, gui_state.last_uplink_origin, sizeof(uplink_origin));
    uplink_origin[sizeof(uplink_origin) - 1] = '\0';
    uplink_bytes = gui_state.last_uplink_bytes;
    uplink_success = gui_state.last_uplink_success;

    strncpy(downlink_file, gui_state.last_downlink_file, sizeof(downlink_file));
    downlink_file[sizeof(downlink_file) - 1] = '\0';
    strncpy(downlink_origin, gui_state.last_downlink_origin, sizeof(downlink_origin));
    downlink_origin[sizeof(downlink_origin) - 1] = '\0';
    downlink_bytes = gui_state.last_downlink_bytes;
    downlink_src = gui_state.last_downlink_src;
    downlink_dst = gui_state.last_downlink_dst;

    rot_az = gui_state.last_rotator_azimuth;
    rot_el = gui_state.last_rotator_elevation;
    rot_success = gui_state.last_rotator_success;
    pthread_mutex_unlock(&gui_state.lock);

    gui_backend_send(client, "OK STATUS\n");
    gui_backend_send_fmt(client, "satellite=%d\n", sat_no);
    gui_backend_send_fmt(client, "tx_freq=%d\n", tx_freq);
    gui_backend_send_fmt(client, "rx_freq=%d\n", rx_freq);
    gui_backend_send_fmt(client, "tle1=%s\n", tle1);
    gui_backend_send_fmt(client, "tle2=%s\n", tle2);
    gui_backend_send_fmt(client, "last_uplink_origin=%s\n", uplink_origin);
    gui_backend_send_fmt(client, "last_uplink_file=%s\n", uplink_file);
    gui_backend_send_fmt(client, "last_uplink_bytes=%zu\n", uplink_bytes);
    gui_backend_send_fmt(client, "last_uplink_status=%s\n", uplink_success ? "success" : "failure");
    gui_backend_send_fmt(client, "last_downlink_origin=%s\n", downlink_origin);
    gui_backend_send_fmt(client, "last_downlink_file=%s\n", downlink_file);
    gui_backend_send_fmt(client, "last_downlink_bytes=%zu\n", downlink_bytes);
    gui_backend_send_fmt(client, "last_downlink_src=%u\n", downlink_src);
    gui_backend_send_fmt(client, "last_downlink_dst=%u\n", downlink_dst);
    gui_backend_send_fmt(client, "rotator_last_az=%d\n", rot_az);
    gui_backend_send_fmt(client, "rotator_last_el=%d\n", rot_el);
    gui_backend_send_fmt(client, "rotator_last_status=%s\n", rot_success ? "success" : "failure");
    gui_backend_send(client, "END\n");
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

    gui_backend_push_event(GUI_BACKEND_EVENT_INFO, "GUI",
                           is_tx ? "Set TX frequency" : "Set RX frequency", token);
    gui_backend_send_fmt(client, "OK %s %ld\n", is_tx ? "TX" : "RX", value);
}

static void gui_backend_handle_set_rotator(struct gui_backend_client *client,
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

    int success = serial_set_az_el((int)azimuth, (int)elevation);
    gui_backend_notify_rotator((int)azimuth, (int)elevation, success);
    if (!success)
    {
        gui_backend_send(client, "ERROR Rotator command failed\n");
        return;
    }

    gui_backend_send_fmt(client, "OK ROTATOR %ld %ld\n", azimuth, elevation);
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
        gui_backend_send_fmt(client, "%s %s %s | %s\n", timestamp,
                             events[i].origin, events[i].summary, events[i].detail);
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
    else if (strcasecmp(cmd, "SET_ROTATOR") == 0)
    {
        gui_backend_handle_set_rotator(client,
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

        int ready = select(max_fd + 1, &readfds, NULL, NULL, NULL);
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
