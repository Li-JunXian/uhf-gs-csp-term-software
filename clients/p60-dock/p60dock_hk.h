#ifndef P60DOCK_HK_H_
#define P60DOCK_HK_H_

#include <stdint.h>
#include <param/param_types.h>

/**
 * Define memory space
 */
#define P60DOCK_HK_C_OUT(i)             (0x00 + (2 * i)) /* 13 * int16_t */
#define P60DOCK_HK_V_OUT(i)             (0x1A + (2 * i)) /* 13 * uint16_t */
#define P60DOCK_HK_OUT_EN(i)            (0x34 + (i))     /* 13 * uint8_t */
#define P60DOCK_HK_TEMP(i)              (0x44 + (2 * i)) /*  2 * int16_t */
#define P60DOCK_HK_BOOT_CAUSE            0x48
#define P60DOCK_HK_BOOT_COUNTER          0x4C
#define P60DOCK_HK_UPTIME                0x50
#define P60DOCK_HK_RESET_CAUSE           0x54
#define P60DOCK_HK_BATT_MODE             0x56
#define P60DOCK_HK_HEATER_ON             0x57
#define P60DOCK_HK_CONV_5V0_EN           0x58

#define P60DOCK_HK_LATCHUP(i)           (0x5A + (2 * i)) /* 13 * uint16_t */
#define P60DOCK_HK_VBAT_V                0x74
#define P60DOCK_HK_VCC_C                 0x76
#define P60DOCK_HK_BATTERY_C             0x78
#define P60DOCK_HK_BATTERY_V             0x7A
#define P60DOCK_HK_BP_TEMP(i)           (0x7C + (2 * i)) /* 2 * int16_t */
#define P60DOCK_HK_DEVICE_TYPE(i)       (0x80 + (i))     /* 8 * uint8_t */
#define P60DOCK_HK_DEVICE_STATUS(i)     (0x88 + (i))     /* 8 * uint8_t */
#define P60DOCK_HK_DEARM_STATUS          0x90
#define P60DOCK_HK_CNT_WDTGND            0x94
#define P60DOCK_HK_CNT_WDTI2C            0x98
#define P60DOCK_HK_CNT_WDTCAN            0x9C
#define P60DOCK_HK_CNT_WDTCSP(i)        (0xA0 + (4 * i)) /* 2 * uint32_t */
#define P60DOCK_HK_WDTGND_LEFT           0xA8
#define P60DOCK_HK_WDTI2C_LEFT           0xAC
#define P60DOCK_HK_WDTCAN_LEFT           0xB0
#define P60DOCK_HK_WDTCSP_LEFT(i)       (0xB4 + (i))     /* 2 * uint8_t */

/** Define the memory size */
#define P60DOCK_HK_SIZE                  0xB6

extern const param_table_t p60dock_hk[];
extern const int p60dock_hk_count;

#endif /* P60DOCK_HK_H_ */
