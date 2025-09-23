#ifndef P60PDU_HK_H_
#define P60PDU_HK_H_

#include <stdint.h>
#include <param/param_types.h>

/**
 * Define memory space
 */
#define P60PDU_HK_C_OUT(i)             (0x00 + (2 * i)) /* 9 * int16_t */
#define P60PDU_HK_V_OUT(i)             (0x12 + (2 * i)) /* 9 * int16_t */
#define P60PDU_HK_VCC                   0x24
#define P60PDU_HK_VBAT                  0x26
#define P60PDU_HK_TEMP                  0x28
#define P60PDU_HK_CONV(i)              (0x2A + (i)) /* 3 * uint8_t */
#define P60PDU_HK_OUT_EN(i)            (0x2E + (i)) /* 9 * uint8_t */
#define P60PDU_HK_BOOT_CAUSE            0x38
#define P60PDU_HK_BOOT_COUNTER          0x3C
#define P60PDU_HK_UPTIME                0x40
#define P60PDU_HK_RESET_CAUSE           0x44
#define P60PDU_HK_BATT_MODE             0x46
#define P60PDU_HK_LATCHUP(i)           (0x48 + (2 * i)) /* 9 * uint16_t */
#define P60PDU_HK_DEVICE_TYPE(i)       (0x5A + (i))     /* 8 * uint8_t */
#define P60PDU_HK_DEVICE_STATUS(i)     (0x62 + (i))     /* 8 * uint8_t */
#define P60PDU_HK_CNT_WDTGND            0x6C
#define P60PDU_HK_CNT_WDTI2C            0x70
#define P60PDU_HK_CNT_WDTCAN            0x74
#define P60PDU_HK_CNT_WDTCSP(i)        (0x78 + (4 * i)) /* 2 * uint32_t */
#define P60PDU_HK_WDTGND_LEFT           0x80
#define P60PDU_HK_WDTI2C_LEFT           0x84
#define P60PDU_HK_WDTCAN_LEFT           0x88
#define P60PDU_HK_WDTCSP_LEFT(i)       (0x8C + (i))     /* 2 * uint8_t */

/** Define the memory size */
#define P60PDU_HK_SIZE                  0x90

extern const param_table_t p60pdu_hk[];
extern const int p60pdu_hk_count;

#endif /* P60PDU_HK_H_ */
