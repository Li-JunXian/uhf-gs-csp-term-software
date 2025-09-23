#ifndef INCLUDE_EPS_BOARD_H_
#define INCLUDE_EPS_BOARD_H_

#include <stddef.h>
#include <stdint.h>
#include <param/param_types.h>
#include <eps.h>

#define EPS_BOARD_UID				0x00
#define EPS_BOARD_LOG_STORE			0x0F
#define EPS_BOARD_WDTI2C			0x10
#define EPS_BOARD_WDTGND			0x14
#define EPS_BOARD_WDTI2C_RST		0x18
#define EPS_BOARD_WDTCSP_CHAN(i)	(0x19 + i)
#define EPS_BOARD_WDTCSP_ADDR(i)	(0x1B + i)
#define EPS_BOARD_I2C_SLAVE			0x1D
#define EPS_BOARD_I2C_SPEED			0x1E
#define EPS_BOARD_I2C_ADDR			0x20
#define EPS_BOARD_CSP_ADDR			0x21
#define EPS_BOARD_HEATER_QUAD		0x22
#define EPS_BOARD_HEATER_ONBRD		0x23

#define EPS_BOARD_BATT_HWMAX		0x24
#define EPS_BOARD_BATT_MAX			0x26
#define EPS_BOARD_BATT_NORM			0x28
#define EPS_BOARD_BATT_SAFE			0x2A
#define EPS_BOARD_BATT_CRIT			0x2C

#define EPS_BOARD_INIT_OUT_NORM(i)		(0x30 + i)
#define EPS_BOARD_INIT_OUT_SAFE(i)		(0x38 + i)
#define EPS_BOARD_INIT_OUT_ON_S(i)		(0x40 + 2*i)
#define EPS_BOARD_INIT_OUT_OFF_S(i)		(0x50 + 2*i)

#define EPS_BOARD_CSP_RTABLE		0x60

#define EPS_BOARD_SIZE				0xA0

static const param_table_t PROGMEM eps_board[] = {
		{.name = "uid", 		.addr = EPS_BOARD_UID,					.type = PARAM_DATA,		.size = 15},
		{.name = "log_store", 	.addr = EPS_BOARD_LOG_STORE,			.type = PARAM_UINT8,	.size = sizeof(uint8_t)},
		{.name = "wdti2c", 		.addr = EPS_BOARD_WDTI2C,				.type = PARAM_UINT32,	.size = sizeof(uint32_t)},
		{.name = "wdtgnd", 		.addr = EPS_BOARD_WDTGND,				.type = PARAM_UINT32,	.size = sizeof(uint32_t)},
		{.name = "wdti2cRst", 	.addr = EPS_BOARD_WDTI2C_RST,			.type = PARAM_UINT8,	.size = sizeof(uint8_t)},
		{.name = "wdtcspChan", 	.addr = EPS_BOARD_WDTCSP_CHAN(0),		.type = PARAM_UINT8,	.size = sizeof(uint8_t), .count = 2},
		{.name = "wdtcspAddr", 	.addr = EPS_BOARD_WDTCSP_ADDR(0),		.type = PARAM_UINT8,	.size = sizeof(uint8_t), .count = 2},
		{.name = "i2cSlave", 	.addr = EPS_BOARD_I2C_SLAVE,			.type = PARAM_UINT8,	.size = sizeof(uint8_t)},
		{.name = "i2cSpeed", 	.addr = EPS_BOARD_I2C_SPEED,			.type = PARAM_UINT16,	.size = sizeof(uint16_t)},
		{.name = "i2cAddr", 	.addr = EPS_BOARD_I2C_ADDR,				.type = PARAM_UINT8,	.size = sizeof(uint8_t)},
		{.name = "cspAddr", 	.addr = EPS_BOARD_CSP_ADDR,				.type = PARAM_UINT8,	.size = sizeof(uint8_t)},
		{.name = "battHWmax", 	.addr = EPS_BOARD_BATT_HWMAX, 			.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
		{.name = "battMax", 	.addr = EPS_BOARD_BATT_MAX, 			.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
		{.name = "battNorm", 	.addr = EPS_BOARD_BATT_NORM, 			.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
		{.name = "battSafe", 	.addr = EPS_BOARD_BATT_SAFE, 			.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
		{.name = "battCrit", 	.addr = EPS_BOARD_BATT_CRIT, 			.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
		{.name = "heaterQuad", 	.addr = EPS_BOARD_HEATER_QUAD,			.type = PARAM_UINT8,	.size = sizeof(uint8_t)},
		{.name = "heaterOnBrd", .addr = EPS_BOARD_HEATER_ONBRD,			.type = PARAM_UINT8,	.size = sizeof(uint8_t)},

		{.name = "initNorm", 	.addr = EPS_BOARD_INIT_OUT_NORM(0), 	.type = PARAM_UINT8, 	.size = sizeof(uint8_t), .count = 8},
		{.name = "initSafe", 	.addr = EPS_BOARD_INIT_OUT_SAFE(0), 	.type = PARAM_UINT8, 	.size = sizeof(uint8_t), .count = 8},
		{.name = "initOnS", 	.addr = EPS_BOARD_INIT_OUT_ON_S(0), 	.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 8},
		{.name = "initOffS", 	.addr = EPS_BOARD_INIT_OUT_OFF_S(0),	.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 8},

		{.name = "cspRtable", 	.addr = EPS_BOARD_CSP_RTABLE,			.type = PARAM_STRING,	.size = 0x40},
};

static const int eps_board_count = sizeof(eps_board) / sizeof(eps_board[0]);

void eps_board_restore(void);

#endif /* INCLUDE_EPS_PARAM_H_ */
