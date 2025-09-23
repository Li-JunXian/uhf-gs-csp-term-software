/**
 * NanoPower firmware
 *
 * @author Kristian Bay
 * Copyright 2015 GomSpace ApS. All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#include <csp/csp.h>
#include <csp/csp_endian.h>

#include <power_if.h>

uint8_t power_if_get_ch_idx(char * ch_name, uint8_t * ch_no, uint8_t ch_no_max) {

	uint8_t result = 1;
	uint8_t len = strlen(ch_name);
	for (int i = 0; i < len; i++) {
		if (!isdigit(ch_name[i])) {
			result = 0;
			break;
		}
	}
	if (result) {
		*ch_no = atoi(ch_name);
		if (*ch_no >= ch_no_max) {
			result = 0;
		}
	}
	return result;
}

int power_if_cmd(uint8_t node, uint8_t port, uint32_t timeout, uint8_t cmd, void * ch_status_p) {

	power_if_cmd_request_t req;

	if ((cmd == POWER_IF_SET) || (cmd == POWER_IF_GET)) {
		power_if_cmd_response_t resp;
		power_if_ch_status_t * ch_status = (power_if_ch_status_t *)ch_status_p;
		if (cmd == POWER_IF_SET) {
			req.cmd = POWER_IF_SET;
			req.ch_status.mode = ch_status->mode;
			req.ch_status.on_cnt = csp_hton16(ch_status->on_cnt);
			req.ch_status.off_cnt = csp_hton16(ch_status->off_cnt);
		} else {
			req.cmd = POWER_IF_GET;
			req.ch_status.mode = 0;
			req.ch_status.on_cnt = 0;
			req.ch_status.off_cnt = 0;
		}
		ch_status->name[POWER_IF_NAME_LEN - 1] = 0;
		strcpy(req.ch_status.name, ch_status->name);
		if (csp_transaction(CSP_PRIO_HIGH, node, port, timeout, &req, sizeof(power_if_cmd_request_t), &resp, sizeof(power_if_cmd_response_t))) {
			if ((resp.cmd == POWER_IF_SET) || (resp.cmd == POWER_IF_GET)) {
				if (resp.status == POWER_IF_STATUS_OK) {
					ch_status->ch_idx = resp.ch_status.ch_idx;
					ch_status->mode = resp.ch_status.mode;
					ch_status->on_cnt = csp_ntoh16(resp.ch_status.on_cnt);
					ch_status->off_cnt = csp_ntoh16(resp.ch_status.off_cnt);
					ch_status->cur_lu_lim = csp_ntoh16(resp.ch_status.cur_lu_lim);
					ch_status->cur_lim = csp_ntoh16(resp.ch_status.cur_lim);
					ch_status->voltage = csp_ntoh16(resp.ch_status.voltage);
					ch_status->current = csp_ntoh16(resp.ch_status.current);
					ch_status->latchup = csp_ntoh16(resp.ch_status.latchup);
					strncpy(ch_status->name, resp.ch_status.name, POWER_IF_NAME_LEN - 1);
					/* Ensure zero termination*/
					ch_status->name[POWER_IF_NAME_LEN - 1] = 0;
					return 1;
				}
			}
		}
	} else if (cmd == POWER_IF_LIST) {
		power_if_cmd_list_response_t * ch_list = (power_if_cmd_list_response_t *)ch_status_p;
		req.cmd = POWER_IF_LIST;
		req.ch_status.mode = 0;
		req.ch_status.on_cnt = 0;
		req.ch_status.off_cnt = 0;
		req.ch_status.name[0] = 0;
		if (csp_transaction(CSP_PRIO_HIGH, node, port, timeout, &req, sizeof(power_if_cmd_request_t), ch_list, sizeof(power_if_cmd_list_response_t))) {
			if ((ch_list->cmd == POWER_IF_LIST) && (ch_list->status == POWER_IF_STATUS_OK)) {
				return 1;
			}
		}
	}

	return 0;
}
