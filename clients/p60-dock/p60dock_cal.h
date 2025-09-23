/**
 * NanoCom firmware
 *
 * @author Johan De Claville Christiansen
 * Copyright 2014 GomSpace ApS. All rights reserved.
 */

#ifndef P60DOCK_CAL_H_
#define P60DOCK_CAL_H_

#include <stdint.h>
#include <param/param_types.h>

/**
 * Define memory space
 */

#define P60DOCK_CAL_GAIN_V_OUT(i)        (0x00 + (4 * i)) /* 13 * float */
#define P60DOCK_CAL_GAIN_C_OUT(i)        (0x34 + (4 * i)) /* 13 * float */
#define P60DOCK_CAL_OFFSET_C_OUT(i)      (0x68 + (2 * i)) /* 13 * uint16_t */
#define P60DOCK_CAL_VREF                  0x82
#define P60DOCK_CAL_GAIN_VBAT_V           0x84
#define P60DOCK_CAL_GAIN_VCC_C            0x88
#define P60DOCK_CAL_OFFSET_VCC_C          0x8C
#define P60DOCK_CAL_GAIN_AUX1             0x90
#define P60DOCK_CAL_GAIN_AUX2             0x94
#define P60DOCK_CAL_OFFSET_AUX1           0x98
#define P60DOCK_CAL_OFFSET_AUX2           0x9A
#define P60DOCK_CAL_GAIN_BATT_V           0x9C
#define P60DOCK_CAL_GAIN_BATT_C           0xA0
#define P60DOCK_CAL_OFFSET_BATT_C         0xA4

/** Define the memory size */
#define P60DOCK_CAL_SIZE                  0xA6

extern const param_table_t p60dock_calibration[];
extern const int p60dock_cal_count;

#endif /* P60DOCK_CAL_H_ */
