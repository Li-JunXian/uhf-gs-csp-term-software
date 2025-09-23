/**
 * NanoCom firmware
 *
 * @author Johan De Claville Christiansen
 * Copyright 2014 GomSpace ApS. All rights reserved.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <csp/csp.h>
#include <csp/csp_endian.h>

#include <p60pdu.h>

/**
 * Setup info about configuration parameters
 */
const param_table_t p60pdu_config[] = {
	{.name = "out_name",		.addr = P60PDU_OUT_NAME(0),			.type = PARAM_STRING, 	.size = P60PDU_OUT_NAME_SIZE, .count = 9},
	{.name = "out_en", 			.addr = P60PDU_OUT_EN(0),			.type = PARAM_UINT8, 	.size = sizeof(uint8_t), .count = 9},
	{.name = "out_on_cnt", 		.addr = P60PDU_OUT_ON_CNT(0),		.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 9},
	{.name = "out_off_cnt", 	.addr = P60PDU_OUT_OFF_CNT(0),		.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 9},
	{.name = "init_out_norm", 	.addr = P60PDU_INIT_OUT_NORM(0), 	.type = PARAM_UINT8, 	.size = sizeof(uint8_t), .count = 9},
	{.name = "init_out_safe", 	.addr = P60PDU_INIT_OUT_SAFE(0), 	.type = PARAM_UINT8, 	.size = sizeof(uint8_t), .count = 9},
	{.name = "init_on_dly", 	.addr = P60PDU_INIT_ON_DELAY(0), 	.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 9},
	{.name = "init_off_dly", 	.addr = P60PDU_INIT_OFF_DELAY(0),	.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 9},
	{.name = "safe_off_dly", 	.addr = P60PDU_SAFE_OFF_DELAY(0),	.type = PARAM_UINT8, 	.size = sizeof(uint8_t), .count = 9},

	{.name = "cur_lu_lim", 		.addr = P60PDU_CUR_LU_LIM(0),		.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 9},
	{.name = "cur_lim", 		.addr = P60PDU_CUR_LIM(0),			.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 9},
	{.name = "cur_ema", 		.addr = P60PDU_CUR_EMA(0),			.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 9},

	{.name = "out_link", 		.addr = P60PDU_OUT_LINK(0),			.type = PARAM_UINT8, 	.size = sizeof(uint8_t), .count = 9},
	{.name = "out_conv", 		.addr = P60PDU_OUT_CONV(0), 		.type = PARAM_UINT8, 	.size = sizeof(uint8_t), .count = 9},
	{.name = "out_voltage", 	.addr = P60PDU_OUT_VOLTAGE(0), 		.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 9},

	{.name = "conv_en", 		.addr = P60PDU_CONV_EN(0),			.type = PARAM_UINT8, 	.size = sizeof(uint8_t), .count = 3},
	{.name = "cur_ema_gain", 	.addr = P60PDU_CUR_EMA_GAIN,		.type = PARAM_FLOAT, 	.size = sizeof(float)},

	{.name = "batt_hwmax", 		.addr = P60PDU_BATT_HWMAX, 			.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
	{.name = "batt_max", 		.addr = P60PDU_BATT_MAX, 			.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
	{.name = "batt_norm", 		.addr = P60PDU_BATT_NORM, 			.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
	{.name = "batt_safe", 		.addr = P60PDU_BATT_SAFE, 			.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
	{.name = "batt_crit", 		.addr = P60PDU_BATT_CRIT, 			.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},

	{.name = "wdt_i2c_rst", 	.addr = P60PDU_WDTI2C_RST,			.type = PARAM_UINT8,	.size = sizeof(uint8_t)},
	{.name = "wdt_can_rst", 	.addr = P60PDU_WDTCAN_RST,			.type = PARAM_UINT8,	.size = sizeof(uint8_t)},
	{.name = "wdt_i2c", 		.addr = P60PDU_WDTI2C,				.type = PARAM_UINT32,	.size = sizeof(uint32_t)},
	{.name = "wdt_can", 		.addr = P60PDU_WDTCAN,				.type = PARAM_UINT32,	.size = sizeof(uint32_t)},
	{.name = "wdt_csp", 		.addr = P60PDU_WDTCSP(0),			.type = PARAM_UINT32,	.size = sizeof(uint32_t), .count = 2},
	{.name = "wdt_csp_ping", 	.addr = P60PDU_WDTCSP_PING_FAIL(0),	.type = PARAM_UINT8,	.size = sizeof(uint8_t), .count = 2},
	{.name = "wdt_csp_chan", 	.addr = P60PDU_WDTCSP_CHAN(0),		.type = PARAM_UINT8,	.size = sizeof(uint8_t), .count = 2},
	{.name = "wdt_csp_addr", 	.addr = P60PDU_WDTCSP_ADDR(0),		.type = PARAM_UINT8,	.size = sizeof(uint8_t), .count = 2},
};

const int p60pdu_config_count = sizeof(p60pdu_config) / sizeof(p60pdu_config[0]);

/**
 * Setup info about calibration parameters
 */
const param_table_t p60pdu_calibration[] = {
	{.name = "vref",			.addr = P60PDU_CAL_VREF,			.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
	{.name = "gain_vcc", 		.addr = P60PDU_CAL_GAIN_VCC,		.type = PARAM_FLOAT,	.size = sizeof(float)},
	{.name = "gain_vbat", 		.addr = P60PDU_CAL_GAIN_VBAT,		.type = PARAM_FLOAT,	.size = sizeof(float)},
	{.name = "gain_v_out", 		.addr = P60PDU_CAL_GAIN_V_OUT(0),	.type = PARAM_FLOAT,	.size = sizeof(float), .count = 9},
	{.name = "gain_c_out", 		.addr = P60PDU_CAL_GAIN_C_OUT(0),	.type = PARAM_FLOAT,	.size = sizeof(float), .count = 9},
	{.name = "offs_c_out", 		.addr = P60PDU_CAL_OFFSET_C_OUT(0),	.type = PARAM_INT16,	.size = sizeof(int16_t), .count = 9},
};

const int p60pdu_cal_count = sizeof(p60pdu_calibration) / sizeof(p60pdu_calibration[0]);

/**
 * Setup info about hk parameters
 */
const param_table_t p60pdu_hk[] = {
	{.name = "c_out", 			.addr = P60PDU_HK_C_OUT(0),			.type = PARAM_INT16, 	.size = sizeof(int16_t), .count = 9},
	{.name = "v_out", 			.addr = P60PDU_HK_V_OUT(0),			.type = PARAM_UINT16, 	.size = sizeof(int16_t), .count = 9},
	{.name = "vcc", 			.addr = P60PDU_HK_VCC,				.type = PARAM_UINT16, 	.size = sizeof(int16_t)},
	{.name = "vbat", 			.addr = P60PDU_HK_VBAT,				.type = PARAM_UINT16, 	.size = sizeof(int16_t)},
	{.name = "temp", 			.addr = P60PDU_HK_TEMP,				.type = PARAM_INT16, 	.size = sizeof(int16_t)},
	{.name = "conv_en", 		.addr = P60PDU_HK_CONV(0),			.type = PARAM_UINT8, 	.size = sizeof(uint8_t), .count = 3},
	{.name = "out_en", 			.addr = P60PDU_HK_OUT_EN(0),		.type = PARAM_UINT8, 	.size = sizeof(uint8_t), .count = 9},
	{.name = "bootcause", 		.addr = P60PDU_HK_BOOT_CAUSE,		.type = PARAM_UINT32, 	.size = sizeof(uint32_t), .flags = PARAM_F_READONLY},
	{.name = "bootcnt", 		.addr = P60PDU_HK_BOOT_COUNTER,		.type = PARAM_UINT32, 	.size = sizeof(uint32_t), .flags = PARAM_F_PERSIST},
	{.name = "uptime", 			.addr = P60PDU_HK_UPTIME,			.type = PARAM_UINT32, 	.size = sizeof(uint32_t), .flags = PARAM_F_READONLY},
	{.name = "resetcause", 		.addr = P60PDU_HK_RESET_CAUSE,		.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .flags = PARAM_F_PERSIST},
	{.name = "batt_mode", 		.addr = P60PDU_HK_BATT_MODE,		.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
	{.name = "latchup", 		.addr = P60PDU_HK_LATCHUP(0),		.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 9, .flags = PARAM_F_PERSIST},
	{.name = "device_type", 	.addr = P60PDU_HK_DEVICE_TYPE(0),	.type = PARAM_UINT8, 	.size = sizeof(uint8_t),  .count = 8},
	{.name = "device_status", 	.addr = P60PDU_HK_DEVICE_STATUS(0),	.type = PARAM_UINT8, 	.size = sizeof(uint8_t),  .count = 8},
	{.name = "wdt_cnt_gnd", 	.addr = P60PDU_HK_CNT_WDTGND,		.type = PARAM_UINT32, 	.size = sizeof(uint32_t), .flags = PARAM_F_PERSIST},
	{.name = "wdt_cnt_i2c", 	.addr = P60PDU_HK_CNT_WDTI2C,		.type = PARAM_UINT32, 	.size = sizeof(uint32_t), .flags = PARAM_F_PERSIST},
	{.name = "wdt_cnt_can", 	.addr = P60PDU_HK_CNT_WDTCAN,		.type = PARAM_UINT32, 	.size = sizeof(uint32_t), .flags = PARAM_F_PERSIST},
	{.name = "wdt_cnt_csp", 	.addr = P60PDU_HK_CNT_WDTCSP(0),	.type = PARAM_UINT32, 	.size = sizeof(uint32_t), .count = 2, .flags = PARAM_F_PERSIST},
	{.name = "wdt_gnd_left", 	.addr = P60PDU_HK_WDTGND_LEFT,		.type = PARAM_UINT32, 	.size = sizeof(uint32_t)},
	{.name = "wdt_i2c_left", 	.addr = P60PDU_HK_WDTI2C_LEFT,		.type = PARAM_UINT32, 	.size = sizeof(uint32_t)},
	{.name = "wdt_can_left", 	.addr = P60PDU_HK_WDTCAN_LEFT,		.type = PARAM_UINT32, 	.size = sizeof(uint32_t)},
	{.name = "wdt_csp_left", 	.addr = P60PDU_HK_WDTCSP_LEFT(0),	.type = PARAM_UINT8,	.size = sizeof(uint8_t), .count = 2},
};

const int p60pdu_hk_count = sizeof(p60pdu_hk) / sizeof(p60pdu_hk[0]);

int p60pdu_get_hk(param_index_t * mem, uint8_t node, uint32_t timeout) {

	mem->table = p60pdu_hk;
	mem->mem_id = P60PDU_HK;
	mem->count = p60pdu_hk_count;
	mem->size = P60PDU_HK_SIZE;
	int result = rparam_get_full_table(mem, node, P60_PORT_RPARAM, mem->mem_id, timeout);

	return (result == 0);

}

int p60pdu_gndwdt_clear(uint8_t node, uint32_t timeout) {
	uint8_t magic = 0x78;
	return csp_transaction(CSP_PRIO_HIGH, node, P60_PORT_GNDWDT_RESET, timeout, &magic, 1, NULL, 0);
}
