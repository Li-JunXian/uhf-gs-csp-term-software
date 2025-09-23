/**
 * NanoCom firmware
 *
 * @author Kristian Bay
 * Copyright 2015 GomSpace ApS. All rights reserved.
 */

#ifndef P60ACU_CAL_H_
#define P60ACU_CAL_H_

#include <stdint.h>
#include <param/param_types.h>

/**
 * Define memory space
 */
#define P60ACU_CAL_VREF                  0x00

#define P60ACU_CAL_GAIN_VCC              0x04
#define P60ACU_CAL_GAIN_VBAT             0x08
#define P60ACU_CAL_GAIN_V_IN(i)         (0x0C + (4 * i)) /* 6 * float */
#define P60ACU_CAL_GAIN_C_IN(i)         (0x24 + (4 * i)) /* 6 * float */
#define P60ACU_CAL_OFFSET_C_IN(i)       (0x3C + (4 * i)) /* 6 * int16 */
#define P60ACU_CAL_GAIN_DAC(i)          (0x54 + (4 * i)) /* 6 * float */

/** Define the memory size */
#define P60ACU_CAL_SIZE                  0x6C

extern const param_table_t p60acu_calibration[];
extern const int p60acu_cal_count;

#endif /* P60ACU_CAL_H_ */
