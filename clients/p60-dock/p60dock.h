/**
 * NanoCom firmware
 *
 * @author Johan De Claville Christiansen
 * Copyright 2014 GomSpace ApS. All rights reserved.
 */

#ifndef P60DOCK_H_
#define P60DOCK_H_

#include <stdint.h>

#include <param/param_types.h>

#include <p60.h>
#include <p60_board.h>
#include <p60dock_param.h>
#include <p60dock_cal.h>
#include <p60dock_hk.h>

/** FRAM MEMORY MAP */
#define P60DOCK_FRAM_PARAM        0x0400
#define P60DOCK_FRAM_CAL          0x0800
#define P60DOCK_FRAM_HK           0x0C00

#define P60DOCK_FRAM_GNDWDT       0x1F00

/** FRAM FILENAMES */
#define P60DOCK_FNO_PARAM         1
#define P60DOCK_FNO_PARAM_DFL     5
#define P60DOCK_FNO_CAL           2
#define P60DOCK_FNO_CAL_DFL       6

/** PARAM INDEX MAP */
/* Index 0 is reserved for board param */
#define P60DOCK_PARAM             1
#define P60DOCK_CAL               2
#define P60DOCK_SCRATCH           3
#define P60DOCK_HK                4

#define P60DOCK_BATT_PACK_OTHER   0
#define P60DOCK_BATT_PACK_BP2     1
#define P60DOCK_BATT_PACK_BP4     2
#define P60DOCK_BATT_PACK_BPX     3

int p60dock_get_hk(param_index_t * mem, uint8_t node, uint32_t timeout);
int p60dock_gndwdt_clear(uint8_t node, uint32_t timeout);
int p60dock_output_get(uint8_t node, char * ch_name, uint8_t * mode, uint8_t * ch_no);
int p60dock_output_set(uint8_t node, char * ch_name, uint8_t mode, uint8_t * ch_no);

void cmd_p60dock_setup(void);

#endif /* P60DOCK_H_ */
