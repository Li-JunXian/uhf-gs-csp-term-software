/**
 * NanoCom firmware
 *
 * @author Johan De Claville Christiansen
 * Copyright 2014 GomSpace ApS. All rights reserved.
 */

#ifndef P60DOCK_PARAM_H_
#define P60DOCK_PARAM_H_

#include <stdint.h>
#include <param/param_types.h>

/**
 * Define memory space
 */
#define P60DOCK_NAME_SIZE                8

#define P60DOCK_OUT_NAME(i)             (0x00 + (P60DOCK_NAME_SIZE * i)) /* 13 * 8 */
#define P60DOCK_OUT_EN(i)               (0x68 + (i))     /* 13 * uint8_t */
#define P60DOCK_OUT_ON_CNT(i)           (0x76 + (2 * i)) /* 13 * uint16_t */
#define P60DOCK_OUT_OFF_CNT(i)          (0x90 + (2 * i)) /* 13 * uint16_t */

#define P60DOCK_INIT_OUT_NORM(i)        (0xAA + (i))     /* 13 * uint8_t */
#define P60DOCK_INIT_OUT_SAFE(i)        (0xB7 + (i))     /* 13 * uint8_t */
#define P60DOCK_INIT_ON_DELAY(i)        (0xC4 + (2 * i)) /* 13 * uint16_t */
#define P60DOCK_INIT_OFF_DELAY(i)       (0xDE + (2 * i)) /* 13 * uint16_t */

#define P60DOCK_CUR_LU_LIM(i)           (0xF8 + (2 * i)) /* 13 * uint16_t */
#define P60DOCK_CUR_LIM(i)              (0x112 + (2 * i)) /* 13 * uint16_t */
#define P60DOCK_CUR_EMA(i)              (0x12C + (2 * i)) /* 13 * uint16_t */
#define P60DOCK_CUR_EMA_GAIN             0x148

#define P60DOCK_VCC_LINK(i)             (0x14C + (i)) /* 4 * uint8_t */
#define P60DOCK_VCC_VBAT_LINK(i)        (0x150 + (i)) /* 4 * uint8_t */

#define P60DOCK_BATTERY_PACK             0x154

#define P60DOCK_BATT_HWMAX               0x156
#define P60DOCK_BATT_MAX                 0x158
#define P60DOCK_BATT_NORM                0x15A
#define P60DOCK_BATT_SAFE                0x15C
#define P60DOCK_BATT_CRIT                0x15E

#define P60DOCK_BP_HEATERMODE            0x160
#define P60DOCK_BP_HEATER_LOW            0x162
#define P60DOCK_BP_HEATER_HIGH           0x164

#define P60DOCK_WDTI2C_RST               0x166
#define P60DOCK_WDTCAN_RST               0x167
#define P60DOCK_WDTI2C                   0x168
#define P60DOCK_WDTCAN                   0x16C
#define P60DOCK_WDTCSP(i)               (0x170 + (4 * i)) /* 2 * uint32_t */
#define P60DOCK_WDTCSP_PING_FAIL(i)     (0x178 + (i))     /* 2 * uint8_t */
#define P60DOCK_WDTCSP_CHAN(i)          (0x17A + (i))     /* 2 * uint8_t */
#define P60DOCK_WDTCSP_ADDR(i)          (0x17C + (i))     /* 2 * uint8_t */

#define P60DOCK_P60ACU_CHAN(i)          (0x17E + (i))     /* 2 * uint8_t */
#define P60DOCK_P60ACU_ADDR(i)          (0x180 + (i))     /* 2 * uint8_t */
#define P60DOCK_P60PDU_CHAN(i)          (0x182 + (i))     /* 4 * uint8_t */
#define P60DOCK_P60PDU_ADDR(i)          (0x186 + (i))     /* 4 * uint8_t */

#define P60DOCK_CONV_5V0_EN              0x18A

/** Define the memory size */
#define P60DOCK_PARAM_SIZE               0x18C

extern const param_table_t p60dock_config[];
extern const int p60dock_config_count;

#endif /* P60DOCK_PARAM_H_ */
