/**
 * NanoCom firmware
 *
 * @author Johan De Claville Christiansen
 * Copyright 2014 GomSpace ApS. All rights reserved.
 */

#ifndef P60ACU_H_
#define P60ACU_H_

#include <stdint.h>
#include <param/param_types.h>

#include <p60.h>
#include <p60_board.h>
#include <p60acu_param.h>
#include <p60acu_cal.h>
#include <p60acu_hk.h>

/** FRAM MEMORY MAP */
#define P60ACU_FRAM_PARAM        0x0400
#define P60ACU_FRAM_CAL          0x0800
#define P60ACU_FRAM_HK           0x0C00

/** FRAM FILENAMES */
#define P60ACU_FNO_PARAM         1
#define P60ACU_FNO_PARAM_DFL     5
#define P60ACU_FNO_CAL           2
#define P60ACU_FNO_CAL_DFL       6

/** PARAM INDEX MAP */
/* Index 0 is reserved for board param */
#define P60ACU_PARAM             1
#define P60ACU_CAL               2
#define P60ACU_SCRATCH           3
#define P60ACU_HK                4

int p60acu_get_hk(param_index_t * mem, uint8_t node, uint32_t timeout);
int p60acu_gndwdt_clear(uint8_t node, uint32_t timeout);
void cmd_p60acu_setup(void);

#endif /* P60ACU_H_ */
