#ifndef GUI_BACKEND_H
#define GUI_BACKEND_H

#include <stddef.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>

#define GUI_BACKEND_MAX_PASSES 16

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
        GUI_BACKEND_MODE_IDLE,
        GUI_BACKEND_MODE_TRACKING,
        GUI_BACKEND_MODE_MAINTENANCE
} gui_backend_mode_t;

typedef struct {
        char name[32];
        time_t aos_utc;
        time_t los_utc;
        uint16_t duration_sec;
        uint16_t peak_elevation_deg;
} gui_backend_pass_t;

void gui_backend_set_station_info(const char *name, double lat_deg, double lon_deg, double alt_m,
                                  double true_north_deg);
void gui_backend_set_mode(gui_backend_mode_t mode);
void gui_backend_set_emergency_stop(int engaged);
void gui_backend_update_pass_schedule(const gui_backend_pass_t *passes, size_t count);

typedef struct {
        uint32_t norad_id;
        double lat_deg;
        double lon_deg;
        double alt_km;
        double velocity_km_s;
        double range_km;
        double range_rate_km_s;
        time_t tle_epoch;
} gui_backend_satellite_t;

void gui_backend_update_satellite(const gui_backend_satellite_t *satellite);

/** GUI backend event types used for telemetry streaming. */
typedef enum {
        GUI_BACKEND_EVENT_UPLINK,
        GUI_BACKEND_EVENT_DOWNLINK,
        GUI_BACKEND_EVENT_INFO,
        GUI_BACKEND_EVENT_ERROR,
} gui_backend_event_type_t;

/**
 * Start the GUI backend server thread.
 *
 * @param thread[out] Pointer that receives the thread handle on success.
 * @return 0 on success, -1 on failure.
 */
int gui_backend_start(pthread_t *thread);

/**
 * Record details about the most recent uplink attempt.
 *
 * @param origin Component that originated the uplink (for example "MCS" or "GUI").
 * @param file_path Optional path to the stored packet (may be NULL).
 * @param bytes Number of payload bytes transmitted.
 * @param success 1 if the transmission succeeded, 0 otherwise.
 */
void gui_backend_notify_uplink(const char *origin, const char *file_path, size_t bytes, int success);

/**
 * Record details about the most recent downlink packet.
 *
 * @param origin Component that handled the packet (for example "TASK_SERVER").
 * @param file_path Path to the saved packet on disk.
 * @param bytes Number of payload bytes contained in the downlink packet.
 * @param src Source node address of the packet.
 * @param dst Destination node address of the packet.
 */
void gui_backend_notify_downlink(const char *origin, const char *file_path, size_t bytes, uint8_t src, uint8_t dst);

/**
 * Track the last requested rotator target.
 *
 * @param azimuth Azimuth command in degrees.
 * @param elevation Elevation command in degrees.
 * @param success 1 if the command was accepted, 0 otherwise.
 */
void gui_backend_notify_rotator(int azimuth, int elevation, int success);

#ifdef __cplusplus
}
#endif

#endif /* GUI_BACKEND_H */
