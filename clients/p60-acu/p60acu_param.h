/**
 * NanoCom firmware
 *
 * @author Kristian Bay
 * Copyright 2015 GomSpace ApS. All rights reserved.
 */

#ifndef P60ACU_PARAM_H_
#define P60ACU_PARAM_H_

#include <stdint.h>
#include <param/param_types.h>

/**
 * Define memory space
 */
#define P60ACU_MPPT_MODE                 0x00
#define P60ACU_MPPT_DELTA_MODE           0x01
#define P60ACU_MPPT_VBOOST(i)           (0x02 + (2 * i)) /* 6 * int16_t */
#define P60ACU_VBAT_MAX_HI               0x10
#define P60ACU_VBAT_MAX_LO               0x12
#define P60ACU_MPPT_PERIOD               0x14
#define P60ACU_MPPT_MAX_DVBOOST          0x18
#define P60ACU_MPPT_OV_MODE              0x1A

/** Define the memory size */
#define P60ACU_PARAM_SIZE                0x1B

extern const param_table_t p60acu_config[];
extern const int p60acu_config_count;

#endif /* P60ACU_PARAM_H_ */
