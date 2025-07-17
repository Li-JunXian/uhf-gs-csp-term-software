#ifndef RLOG_TYPES_H_
#define RLOG_TYPES_H_

#include <stdint.h>

typedef struct log_group_entry {
	uint8_t idx;
	uint8_t print_mask;
	uint8_t store_mask;
	uint8_t len;
	char name[];
} log_group_entry_t;

typedef struct __attribute__((packed)) {
	uint8_t level;
	uint8_t group;
	uint32_t sec;
	uint32_t nsec;
	uint8_t len;
	char data[];
} log_store_entry_t;

typedef enum __attribute__ ((packed)) rlog_action_e {
	RLOG_LIST = 0x00,
	RLOG_PRINT = 0x01,
	RLOG_STORE = 0x02,
	RLOG_HIST = 0x03,
} rlog_action;

typedef enum __attribute__ ((packed)) rlog_status_e {
	RLOG_OK = 0,
	RLOG_ERROR = 0xFF,
} rlog_status;

typedef struct __attribute__ ((packed)) rlog_request_s {
	uint8_t action;
	uint32_t level_mask;
	uint32_t group_mask;
	uint32_t ts_sec;
	uint32_t ts_nsec;
	uint16_t line_count;
	uint16_t line_offset;
	uint16_t cap_len;
	uint16_t csp_packets;
} rlog_request;

typedef struct __attribute__ ((packed)) rlog_reply_s {
	uint8_t action;
	uint8_t status;
	uint8_t idx_first;
	uint8_t no_elem;
	uint8_t total_elem;
	uint8_t data[];
} rlog_reply;

#endif /* RLOG_TYPES_H_ */
