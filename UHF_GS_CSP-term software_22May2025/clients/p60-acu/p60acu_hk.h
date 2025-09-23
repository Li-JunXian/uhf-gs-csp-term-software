/**
 * NanoCom firmware
 *
 * @author Johan De Claville Christiansen
 * Copyright 2014 GomSpace ApS. All rights reserved.
 */

#ifndef P60ACU_HK_H_
#define P60ACU_HK_H_

#include <stdint.h>
#include <param/param_types.h>

/**
 * Define memory space
 */
#define P60ACU_HK_C_IN(i)               (0x00 + (2 * i)) /* 6 * int16_t */
#define P60ACU_HK_V_IN(i)               (0x0C + (2 * i)) /* 6 * int16_t */
#define P60ACU_HK_VBAT                   0x18            /* uint16_t */
#define P60ACU_HK_VCC                    0x1A            /* uint16_t */
#define P60ACU_HK_TEMP(i)               (0x1C + (2 * i)) /* 3 * int16_t */
#define P60ACU_HK_MPPT_MODE              0x22            /* uint8_t */
#define P60ACU_HK_MPPT_VBOOST(i)        (0x24 + (2 * i)) /* 6 * uint16_t */
#define P60ACU_HK_PWR(i)                (0x30 + (2 * i)) /* 6 * uint16_t */
#define P60ACU_HK_DAC_EN(i)             (0x3C + (i))     /* 3 * uint8_t */
#define P60ACU_HK_MPPT_DAC_VAL(i)       (0x40 + (2 * i)) /* 6 * uint16_t */
#define P60ACU_HK_BOOT_CAUSE             0x50            /* uint32_t */
#define P60ACU_HK_BOOT_COUNTER           0x54            /* uint32_t */
#define P60ACU_HK_UPTIME                 0x58            /* uint32_t */
#define P60ACU_HK_RESET_CAUSE            0x5C            /* uint16_t */
#define P60ACU_HK_MPPT_TIME              0x5E            /* uint16_t */
#define P60ACU_HK_MPPT_PERIOD            0x60            /* uint16_t */
#define P60ACU_HK_DEVICE_TYPE(i)        (0x64 + (i))     /* 8 * uint8_t */
#define P60ACU_HK_DEVICE_STATUS(i)      (0x6C + (i))     /* 8 * uint8_t */
#define P60ACU_HK_CNT_WDTGND             0x74            /* uint32_t */
#define P60ACU_HK_WDTGND_LEFT            0x78            /* uint32_t */

/** Define the memory size */
#define P60ACU_HK_SIZE                   0x7C

extern const param_table_t p60acu_hk[];
extern const int p60acu_hk_count;

#endif /* P60ACU_HK_H_ */
