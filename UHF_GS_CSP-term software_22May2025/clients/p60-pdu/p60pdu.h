/**
 * NanoCom firmware
 *
 * @author Johan De Claville Christiansen
 * Copyright 2014 GomSpace ApS. All rights reserved.
 */

#ifndef P60PDU_H_
#define P60PDU_H_

#include <stdint.h>
#include <param/param_types.h>

#include <p60.h>
#include <p60_board.h>
#include <p60pdu_param.h>
#include <p60pdu_cal.h>
#include <p60pdu_hk.h>

#include <power_if.h>

/** FRAM MEMORY MAP */
#define P60PDU_FRAM_PARAM       0x0400
#define P60PDU_FRAM_CAL         0x0800
#define P60PDU_FRAM_HK          0x0C00

/** FRAM FILENAMES */
#define P60PDU_FNO_PARAM        1
#define P60PDU_FNO_PARAM_DFL    5
#define P60PDU_FNO_CAL          2
#define P60PDU_FNO_CAL_DFL      6

/** PARAM INDEX MAP */
/* Index 0 is reserved for board param */
#define P60PDU_PARAM            1
#define P60PDU_CAL              2
#define P60PDU_SCRATCH          3
#define P60PDU_HK               4

int p60pdu_get_hk(param_index_t * mem, uint8_t node, uint32_t timeout);
int p60pdu_gndwdt_clear(uint8_t node, uint32_t timeout);
int p60pdu_output_get(uint8_t node, char * ch_name, uint8_t * mode, uint8_t * ch_no);
int p60pdu_output_set(uint8_t node, char * ch_name, uint8_t mode, uint8_t * ch_no);

void cmd_p60pdu_setup(void);

#endif /* P60PDU_H_ */
