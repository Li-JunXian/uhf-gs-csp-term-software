/**
 * NanoCom firmware
 *
 * @author Johan De Claville Christiansen
 * Copyright 2014 GomSpace ApS. All rights reserved.
 */

#ifndef P60_PARAM_H_
#define P60_PARAM_H_

#include <stdint.h>
#include <param/param.h>

/**
 * Define memory space
 */
#define P60_BOARD_UID                   0x00
#define P60_BOARD_TYPE                  0x10
#define P60_BOARD_REV                   0x11
#define P60_BOARD_CSP_ADDR              0x12
#define P60_BOARD_I2C_ADDR              0x13
#define P60_BOARD_I2C_SPEED_KHZ         0x14
#define P60_BOARD_CAN_SPEED_KHZ         0x16
#define P60_BOARD_KISS_ENABLE           0x18
#define P60_BOARD_RS422_MODE            0x19
#define P60_BOARD_RS422_SPEED_KHZ       0x1C
#define P60_BOARD_RTABLE_STR            0x20  //! This one is 0x60 = 96 bytes
#define P60_BOARD_RTABLE_STR_SIZE       0x60

/** Define the memory size */
#define P60_BOARD_PARAM_SIZE            0x80

extern const param_table_t p60_config[];
extern const int p60_config_count;

#endif /* P60_PARAM_H_ */
