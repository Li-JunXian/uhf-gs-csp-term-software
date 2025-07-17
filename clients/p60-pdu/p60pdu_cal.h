/**
 * NanoCom firmware
 *
 * @author Johan De Claville Christiansen
 * Copyright 2014 GomSpace ApS. All rights reserved.
 */

#ifndef P60PDU_CAL_H_
#define P60PDU_CAL_H_

#include <stdint.h>
#include <param/param_types.h>

/**
 * Define memory space
 */

#define P60PDU_CAL_VREF                 0x00
#define P60PDU_CAL_GAIN_VCC             0x04
#define P60PDU_CAL_GAIN_VBAT            0x08
#define P60PDU_CAL_GAIN_V_OUT(i)       (0x0C + (4 * i)) /* 9 * float */
#define P60PDU_CAL_GAIN_C_OUT(i)       (0x30 + (4 * i)) /* 9 * float */
#define P60PDU_CAL_OFFSET_C_OUT(i)     (0x54 + (2 * i)) /* 9 * uint16_t */

/** Define the memory size */
#define P60PDU_CAL_SIZE                 0x66

extern const param_table_t p60pdu_calibration[];
extern const int p60pdu_cal_count;

#endif /* P60PDU_CAL_H_ */
