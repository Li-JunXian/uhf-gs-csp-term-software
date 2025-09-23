/**
 * NanoCom firmware
 *
 * @author Johan De Claville Christiansen
 * Copyright 2014 GomSpace ApS. All rights reserved.
 */

#ifndef P60PDU_PARAM_H_
#define P60PDU_PARAM_H_

#include <stdint.h>
#include <param/param_types.h>

/**
 * Define memory space
 */
#define P60PDU_OUT_NAME_SIZE            8
#define P60PDU_OUT_NAME(i)             (0x00 + (P60PDU_OUT_NAME_SIZE * i))

#define P60PDU_OUT_EN(i)               (0x48 + (i))     /* 9 * uint8_t */
#define P60PDU_OUT_ON_CNT(i)           (0x52 + (2 * i)) /* 9 * uint16_t */
#define P60PDU_OUT_OFF_CNT(i)          (0x64 + (2 * i)) /* 9 * uint16_t */

#define P60PDU_INIT_OUT_NORM(i)        (0x76 + (i))     /* 9 * uint8_t */
#define P60PDU_INIT_OUT_SAFE(i)        (0x80 + (i))     /* 9 * uint8_t */
#define P60PDU_INIT_ON_DELAY(i)        (0x8A + (2 * i)) /* 9 * uint16_t */
#define P60PDU_INIT_OFF_DELAY(i)       (0x9C + (2 * i)) /* 9 * uint16_t */
#define P60PDU_SAFE_OFF_DELAY(i)       (0xAE + (i))     /* 9 * uint8_t */

#define P60PDU_CUR_LU_LIM(i)           (0xB8 + (2 * i)) /* 9 * uint16_t */
#define P60PDU_CUR_LIM(i)              (0xCA + (2 * i)) /* 9 * uint16_t */
#define P60PDU_CUR_EMA(i)              (0xDC + (2 * i)) /* 9 * uint16_t */
#define P60PDU_OUT_LINK(i)             (0xEE + (i))     /* 9 * uint8_t */
#define P60PDU_OUT_CONV(i)             (0xF8 + (i))     /* 9 * uint8_t */
#define P60PDU_OUT_VOLTAGE(i)          (0x102 + (2 * i)) /* 9 * uint16_t */

#define P60PDU_CONV_EN(i)              (0x114 + (i))    /* 3 * uint8_t */
#define P60PDU_CUR_EMA_GAIN             0x118

#define P60PDU_BATT_HWMAX               0x11C
#define P60PDU_BATT_MAX                 0x11E
#define P60PDU_BATT_NORM                0x120
#define P60PDU_BATT_SAFE                0x122
#define P60PDU_BATT_CRIT                0x124

#define P60PDU_WDTI2C_RST               0x126
#define P60PDU_WDTCAN_RST               0x127
#define P60PDU_WDTI2C                   0x128
#define P60PDU_WDTCAN                   0x12C
#define P60PDU_WDTCSP(i)               (0x130 + (4 * i)) /* 2 * uint32_t */
#define P60PDU_WDTCSP_PING_FAIL(i)     (0x138 + (i))     /* 2 * uint8_t */
#define P60PDU_WDTCSP_CHAN(i)          (0x13A + (i))     /* 2 * uint8_t */
#define P60PDU_WDTCSP_ADDR(i)          (0x13C + (i))     /* 2 * uint8_t */

/** Define the memory size */
#define P60PDU_PARAM_SIZE               0x13E

extern const param_table_t p60pdu_config[];
extern const int p60pdu_config_count;

#endif /* P60PDU_PARAM_H_ */
