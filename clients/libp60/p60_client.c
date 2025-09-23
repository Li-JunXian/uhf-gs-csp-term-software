/**
 * NanoCom firmware
 *
 * @author Johan De Claville Christiansen
 * Copyright 2014 GomSpace ApS. All rights reserved.
 */

#include <stddef.h>
#include <stdlib.h>
#include <csp/csp.h>
#include <csp/csp_endian.h>

#include <p60.h>
#include <p60_board.h>

/**
 * Setup info about board configuration parameters
 */
const param_table_t p60_config[] = {
	{.name = "uid", 		.addr = P60_BOARD_UID,					.type = PARAM_STRING,	.size = 16},
	{.name = "type", 		.addr = P60_BOARD_TYPE,					.type = PARAM_UINT8,	.size = sizeof(uint8_t)},
	{.name = "rev", 		.addr = P60_BOARD_REV,					.type = PARAM_UINT8,	.size = sizeof(uint8_t)},
	{.name = "csp_addr",	.addr = P60_BOARD_CSP_ADDR,				.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
	{.name = "i2c_addr", 	.addr = P60_BOARD_I2C_ADDR,				.type = PARAM_UINT8,	.size = sizeof(uint8_t)},
	{.name = "i2c_speed", 	.addr = P60_BOARD_I2C_SPEED_KHZ,		.type = PARAM_UINT16,	.size = sizeof(uint16_t)},
	{.name = "can_speed",	.addr = P60_BOARD_CAN_SPEED_KHZ,		.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
	{.name = "kiss_en",		.addr = P60_BOARD_KISS_ENABLE,			.type = PARAM_UINT8,	.size = sizeof(uint8_t)},
	{.name = "rs422_mode",	.addr = P60_BOARD_RS422_MODE,			.type = PARAM_UINT8,	.size = sizeof(uint8_t)},
	{.name = "rs422_speed",	.addr = P60_BOARD_RS422_SPEED_KHZ,		.type = PARAM_UINT32,	.size = sizeof(uint32_t)},
	{.name = "csp_rtable",	.addr = P60_BOARD_RTABLE_STR,			.type = PARAM_STRING,	.size = P60_BOARD_RTABLE_STR_SIZE},

};

const int p60_config_count = sizeof(p60_config) / sizeof(p60_config[0]);
