/**
 * NanoPower firmware
 *
 * @author Kristian Bay
 * Copyright 2015 GomSpace ApS. All rights reserved.
 */

#ifndef POWER_IF_H_
#define POWER_IF_H_

#include <stdint.h>

#define POWER_IF_SET            1
#define POWER_IF_GET            2
#define POWER_IF_LIST           3

#define POWER_IF_STATUS_OK      0
#define POWER_IF_STATUS_ERROR   1

#define POWER_IF_NAME_LEN       8

typedef struct __attribute__((packed)) {
	uint8_t              ch_idx;
	uint8_t              mode;
	uint16_t             on_cnt;
	uint16_t             off_cnt;
	uint16_t             cur_lu_lim;
	uint16_t             cur_lim;
	uint16_t             voltage;
	int16_t              current;
	uint16_t             latchup;
	char                 name[POWER_IF_NAME_LEN];
} power_if_ch_status_t;

typedef struct __attribute__((packed)) {
	uint8_t              ch_idx;
	uint8_t              mode;
	char                 name[8];
} power_if_list_t;

typedef struct __attribute__((packed)) {
	uint8_t              cmd;
	uint8_t              status;
	power_if_ch_status_t ch_status;
} power_if_cmd_request_t;

typedef struct __attribute__((packed)) {
	uint8_t              cmd;
	uint8_t              status;
	power_if_ch_status_t ch_status;
} power_if_cmd_response_t;

typedef struct __attribute__((packed)) {
	uint8_t              cmd;
	uint8_t              status;
	uint8_t              count;
	power_if_list_t      list[16];
} power_if_cmd_list_response_t;

int power_if_cmd(uint8_t node, uint8_t port, uint32_t timeout, uint8_t cmd, void * ch_status);
uint8_t power_if_get_ch_idx(char * ch_name, uint8_t * ch_no, uint8_t ch_no_max);

#endif /* POWER_IF_H_ */
