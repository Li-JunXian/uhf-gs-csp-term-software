/**
 * @file send_packet.h
 */

#include <stddef.h>
#include <stdint.h>
typedef struct {
        uint8_t priority;
        uint8_t src;
        uint8_t dst;
        uint8_t dst_port;
        uint8_t src_port;
        uint8_t hmac;
        uint8_t xtea;
        uint8_t rdp;
        uint8_t crc;
} mcs_packet_header_t;

/* Send/Uplink data packet */
int send_packet(char * mcs_uplink_filename);
int send_packet_struct(const char *origin, const mcs_packet_header_t *header,
                       const uint8_t *payload, size_t payload_len);

/* Create curent function call timestamp */
char * get_time(char * my_str);
