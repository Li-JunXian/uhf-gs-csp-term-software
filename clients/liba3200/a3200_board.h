/**
 * @author Johan De Claville Christiansen
 * Copyright 2014 GomSpace ApS. All rights reserved.
 */

#ifndef A3200_PARAM_H_
#define A3200_PARAM_H_

#include <stdint.h>
#include <param/param.h>

/** FRAM FILE MAP */
#define FRAM_FNO_BOARD				0
#define FRAM_FNO_BOARD_RO			24
#define FRAM_FNO_BOARD_PERSIST		20
#define FRAM_FNO_BOARD_PERSIST_ADDR (20 * PARAM_MAX_FILESIZE)

/** FRAM MEMORY MAP (From 0x6000 to 0x7FFF is write-protected) */
#define A3200_FRAM_LOG				0x1000
#define A3200_FRAM_LOG_SIZE			0x1000

/** PARAM INDEX MAP */
#define A3200_BOARD					0

/**
 * Define memory space
 */
#define A3200_PWR_GSSB1				0x01
#define A3200_PWR_GSSB2				0x02
#define A3200_PWR_FLASH				0x03
#define A3200_PWR_PWM				0x04
#define A3200_PWR_GPS				0x05
#define A3200_PWR_WDE				0x06

#define A3200_SWLOAD_IMAGE			0x10
#define A3200_SWLOAD_IMAGE_SIZE		0x20
#define A3200_SWLOAD_COUNT      	0x30

#define A3200_FS_CHIPID				0x40
#define A3200_FS_DISABLE			0x41
#define A3200_FS_MOUNTED			0x42
#define A3200_LOG_FRAM				0x43

#define A3200_TEMP_A				0x50
#define A3200_TEMP_B				0x52

#define A3200_CUR_GSSB1				0x60
#define A3200_CUR_GSSB2				0x64
#define A3200_CUR_FLASH				0x68
#define A3200_CUR_PWM				0x6C
#define A3200_CUR_GPS				0x70
#define A3200_CUR_WDE				0x74

#define A3200_BOOT_COUNTER			0x78
#define A3200_BOOT_CAUSE			0x7C

#define A3200_CLOCK					0x80
#define A3200_TICKS					0x84

#define A3200_RTABLE_STR			0x90	//! This one is 0x60 = 96 bytes
#define A3200_RTABLE_STR_SIZE		0x60

#define A3200_TSYNC_NODE			0xF0
#define A3200_TSYNC_INTV			0xF2

#define A3200_RAM_IMAGE 			0xF4
#define A3200_GOSH_USART			0xF5

#define A3200_MEM_SIZE				0x100

/**
 * Setup info about parameters
 */
static const param_table_t a3200_config[] = {

		{.name = "pwrGSSB1",			.addr = A3200_PWR_GSSB1,			.type = PARAM_UINT8,		.size = sizeof(uint8_t)},
		{.name = "pwrGSSB2",			.addr = A3200_PWR_GSSB2,			.type = PARAM_UINT8,		.size = sizeof(uint8_t)},
		{.name = "pwrFlash",			.addr = A3200_PWR_FLASH,			.type = PARAM_UINT8,		.size = sizeof(uint8_t)},
		{.name = "pwrPWM",				.addr = A3200_PWR_PWM,				.type = PARAM_UINT8,		.size = sizeof(uint8_t)},
		{.name = "pwrGPS",				.addr = A3200_PWR_GPS,				.type = PARAM_UINT8,		.size = sizeof(uint8_t)},
		{.name = "pwrWDE",				.addr = A3200_PWR_WDE,				.type = PARAM_UINT8,		.size = sizeof(uint8_t)},

		{.name = "swload_image",		.addr = A3200_SWLOAD_IMAGE,			.type = PARAM_STRING,		.size = A3200_SWLOAD_IMAGE_SIZE, .flags = PARAM_F_PERSIST},
		{.name = "swload_count",		.addr = A3200_SWLOAD_COUNT,			.type = PARAM_UINT16,		.size = sizeof(uint16_t), .flags = PARAM_F_PERSIST},

		{.name = "fs_chipid",			.addr = A3200_FS_CHIPID,			.type = PARAM_UINT8,		.size = sizeof(uint8_t), .flags = PARAM_F_PERSIST},
		{.name = "fs_disable",			.addr = A3200_FS_DISABLE,			.type = PARAM_UINT8,		.size = sizeof(uint8_t), .flags = PARAM_F_PERSIST},
		{.name = "fs_mounted",			.addr = A3200_FS_MOUNTED,			.type = PARAM_UINT8,		.size = sizeof(uint8_t), .flags = PARAM_F_READONLY},
		{.name = "log_fram",			.addr = A3200_LOG_FRAM,				.type = PARAM_UINT8,		.size = sizeof(uint8_t), .flags = PARAM_F_PERSIST},

		{.name = "temp_a",				.addr = A3200_TEMP_A,				.type = PARAM_INT16,		.size = sizeof(int16_t), .flags = PARAM_F_READONLY},
		{.name = "temp_b",				.addr = A3200_TEMP_B,				.type = PARAM_INT16,		.size = sizeof(int16_t), .flags = PARAM_F_READONLY},

		{.name = "curGSSB1",			.addr = A3200_CUR_GSSB1,			.type = PARAM_UINT16,		.size = sizeof(uint16_t), .flags = PARAM_F_READONLY},
		{.name = "curGSSB2",			.addr = A3200_CUR_GSSB2,			.type = PARAM_UINT16,		.size = sizeof(uint16_t), .flags = PARAM_F_READONLY},
		{.name = "curFlash",			.addr = A3200_CUR_FLASH,			.type = PARAM_UINT16,		.size = sizeof(uint16_t), .flags = PARAM_F_READONLY},
		{.name = "curPWM",				.addr = A3200_CUR_PWM,				.type = PARAM_UINT16,		.size = sizeof(uint16_t), .flags = PARAM_F_READONLY},
		{.name = "curGPS",				.addr = A3200_CUR_GPS,				.type = PARAM_UINT16,		.size = sizeof(uint16_t), .flags = PARAM_F_READONLY},
		{.name = "curWDE",				.addr = A3200_CUR_WDE,				.type = PARAM_UINT16,		.size = sizeof(uint16_t), .flags = PARAM_F_READONLY},

		{.name = "boot_count", 			.addr = A3200_BOOT_COUNTER, 		.type = PARAM_UINT16, 		.size = sizeof(uint16_t), .flags = PARAM_F_PERSIST},
		{.name = "boot_cause", 			.addr = A3200_BOOT_CAUSE, 			.type = PARAM_X32, 			.size = sizeof(uint32_t), .flags = PARAM_F_READONLY},

		{.name = "clock", 				.addr = A3200_CLOCK, 				.type = PARAM_UINT32, 		.size = sizeof(uint32_t), .flags = PARAM_F_READONLY},
		{.name = "ticks", 				.addr = A3200_TICKS, 				.type = PARAM_UINT32, 		.size = sizeof(uint32_t), .flags = PARAM_F_READONLY},

		{.name = "csp_rtable",			.addr = A3200_RTABLE_STR,			.type = PARAM_STRING,		.size = A3200_RTABLE_STR_SIZE, .flags = PARAM_F_PERSIST},

		{.name = "tsync_node", 			.addr = A3200_TSYNC_NODE, 			.type = PARAM_INT16, 		.size = sizeof(int16_t), .flags = PARAM_F_PERSIST},
		{.name = "tsync_intv", 			.addr = A3200_TSYNC_INTV, 			.type = PARAM_UINT16, 		.size = sizeof(uint16_t), .flags = PARAM_F_PERSIST},
		{.name = "ram_image",			.addr = A3200_RAM_IMAGE,			.type = PARAM_INT8, 		.size = sizeof(uint8_t)},
		{.name = "gosh_usart",			.addr = A3200_GOSH_USART,			.type = PARAM_INT8, 		.size = sizeof(uint8_t), .flags = PARAM_F_PERSIST},

};

#define A3200_CONFIG_COUNT (sizeof(a3200_config) / sizeof(a3200_config[0]))

void a3200_board_fallback(void);
void a3200_board_callback(uint16_t addr, param_index_t * mem);

#endif /* A3200_PARAM_H_ */
