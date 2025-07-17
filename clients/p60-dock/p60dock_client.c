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

#include <p60dock.h>

/**
 * Setup info about config parameters
 */
const param_table_t p60dock_config[] = {
	{.name = "out_name",		.addr = P60DOCK_OUT_NAME(0),		.type = PARAM_STRING, 	.size = P60DOCK_NAME_SIZE, .count = 13},
	{.name = "out_en", 			.addr = P60DOCK_OUT_EN(0),			.type = PARAM_UINT8, 	.size = sizeof(uint8_t), .count = 13},
	{.name = "out_on_cnt", 		.addr = P60DOCK_OUT_ON_CNT(0),		.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 13},
	{.name = "out_off_cnt", 	.addr = P60DOCK_OUT_OFF_CNT(0),		.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 13},

	{.name = "init_out_norm", 	.addr = P60DOCK_INIT_OUT_NORM(0), 	.type = PARAM_UINT8, 	.size = sizeof(uint8_t), .count = 13},
	{.name = "init_out_safe", 	.addr = P60DOCK_INIT_OUT_SAFE(0), 	.type = PARAM_UINT8, 	.size = sizeof(uint8_t), .count = 13},
	{.name = "init_on_dly", 	.addr = P60DOCK_INIT_ON_DELAY(0),	.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 13},
	{.name = "init_off_dly",	.addr = P60DOCK_INIT_OFF_DELAY(0),	.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 13},

	{.name = "cur_lu_lim", 		.addr = P60DOCK_CUR_LU_LIM(0),		.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 13},
	{.name = "cur_lim", 		.addr = P60DOCK_CUR_LIM(0),			.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 13},
	{.name = "cur_ema", 		.addr = P60DOCK_CUR_EMA(0),			.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 13},
	{.name = "cur_ema_gain", 	.addr = P60DOCK_CUR_EMA_GAIN,		.type = PARAM_FLOAT, 	.size = sizeof(float)},

	{.name = "vcc_vbat_link", 	.addr = P60DOCK_VCC_VBAT_LINK(0),	.type = PARAM_UINT8, 	.size = sizeof(uint8_t), .count = 4},
	{.name = "vcc_link", 		.addr = P60DOCK_VCC_LINK(0),		.type = PARAM_UINT8, 	.size = sizeof(uint8_t), .count = 4},

	{.name = "batt_pack",		.addr = P60DOCK_BATTERY_PACK,		.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},

	{.name = "batt_hwmax", 		.addr = P60DOCK_BATT_HWMAX, 		.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
	{.name = "batt_max", 		.addr = P60DOCK_BATT_MAX, 			.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
	{.name = "batt_norm", 		.addr = P60DOCK_BATT_NORM, 			.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
	{.name = "batt_safe", 		.addr = P60DOCK_BATT_SAFE, 			.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
	{.name = "batt_crit", 		.addr = P60DOCK_BATT_CRIT, 			.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},

	{.name = "bp_heat_mode",	.addr = P60DOCK_BP_HEATERMODE,		.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
	{.name = "bp_heat_low",		.addr = P60DOCK_BP_HEATER_LOW,		.type = PARAM_INT16, 	.size = sizeof(int16_t)},
	{.name = "bp_heat_high",	.addr = P60DOCK_BP_HEATER_HIGH,		.type = PARAM_INT16, 	.size = sizeof(int16_t)},

	{.name = "wdt_i2c_rst", 	.addr = P60DOCK_WDTI2C_RST,			.type = PARAM_UINT8,	.size = sizeof(uint8_t)},
	{.name = "wdt_can_rst", 	.addr = P60DOCK_WDTCAN_RST,			.type = PARAM_UINT8,	.size = sizeof(uint8_t)},
	{.name = "wdt_i2c", 		.addr = P60DOCK_WDTI2C,				.type = PARAM_UINT32,	.size = sizeof(uint32_t)},
	{.name = "wdt_can", 		.addr = P60DOCK_WDTCAN,				.type = PARAM_UINT32,	.size = sizeof(uint32_t)},
	{.name = "wdt_csp", 		.addr = P60DOCK_WDTCSP(0),			.type = PARAM_UINT32,	.size = sizeof(uint32_t), .count = 2},
	{.name = "wdt_csp_ping", 	.addr = P60DOCK_WDTCSP_PING_FAIL(0),.type = PARAM_UINT8,	.size = sizeof(uint8_t), .count = 2},
	{.name = "wdt_csp_chan", 	.addr = P60DOCK_WDTCSP_CHAN(0),		.type = PARAM_UINT8,	.size = sizeof(uint8_t), .count = 2},
	{.name = "wdt_csp_addr", 	.addr = P60DOCK_WDTCSP_ADDR(0),		.type = PARAM_UINT8,	.size = sizeof(uint8_t), .count = 2},

	{.name = "p60acu_chan",		.addr = P60DOCK_P60ACU_CHAN(0),		.type = PARAM_UINT8,	.size = sizeof(uint8_t), .count = 2},
	{.name = "p60acu_addr",		.addr = P60DOCK_P60ACU_ADDR(0),		.type = PARAM_UINT8,	.size = sizeof(uint8_t), .count = 2},
	{.name = "p60pdu_chan",		.addr = P60DOCK_P60PDU_CHAN(0),		.type = PARAM_UINT8,	.size = sizeof(uint8_t), .count = 4},
	{.name = "p60pdu_addr",		.addr = P60DOCK_P60PDU_ADDR(0),		.type = PARAM_UINT8,	.size = sizeof(uint8_t), .count = 4},

	{.name = "conv_5v_en", 		.addr = P60DOCK_CONV_5V0_EN,		.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
};

const int p60dock_config_count = sizeof(p60dock_config) / sizeof(p60dock_config[0]);

/**
 * Setup info about calibration parameters
 */
const param_table_t p60dock_calibration[] = {
	{.name = "gain_v_out", 		.addr = P60DOCK_CAL_GAIN_V_OUT(0),	.type = PARAM_FLOAT,	.size = sizeof(float), .count = 13},
	{.name = "gain_c_out",		.addr = P60DOCK_CAL_GAIN_C_OUT(0),	.type = PARAM_FLOAT,	.size = sizeof(float), .count = 13},
	{.name = "offs_c_out",		.addr = P60DOCK_CAL_OFFSET_C_OUT(0),.type = PARAM_INT16,	.size = sizeof(int16_t), .count = 13},
	{.name = "vref",			.addr = P60DOCK_CAL_VREF,			.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
	{.name = "gain_vbat_v",		.addr = P60DOCK_CAL_GAIN_VBAT_V,	.type = PARAM_FLOAT,	.size = sizeof(float)},
	{.name = "gain_vcc_c", 		.addr = P60DOCK_CAL_GAIN_VCC_C,		.type = PARAM_FLOAT,	.size = sizeof(float)},
	{.name = "offs_vcc_c",		.addr = P60DOCK_CAL_OFFSET_VCC_C,	.type = PARAM_INT16,	.size = sizeof(int16_t)},
	{.name = "gain_aux1",		.addr = P60DOCK_CAL_GAIN_AUX1,		.type = PARAM_FLOAT,	.size = sizeof(float)},
	{.name = "gain_aux2",		.addr = P60DOCK_CAL_GAIN_AUX2,		.type = PARAM_FLOAT,	.size = sizeof(float)},
	{.name = "offs_aux1",		.addr = P60DOCK_CAL_OFFSET_AUX1,	.type = PARAM_INT16,	.size = sizeof(int16_t)},
	{.name = "offs_aux2",		.addr = P60DOCK_CAL_OFFSET_AUX2,	.type = PARAM_INT16,	.size = sizeof(int16_t)},
	{.name = "gain_batt_v", 	.addr = P60DOCK_CAL_GAIN_BATT_V,	.type = PARAM_FLOAT, 	.size = sizeof(float)},
	{.name = "gain_batt_c", 	.addr = P60DOCK_CAL_GAIN_BATT_C,	.type = PARAM_FLOAT, 	.size = sizeof(float)},
	{.name = "offs_batt_c",		.addr = P60DOCK_CAL_OFFSET_BATT_C,	.type = PARAM_INT16,	.size = sizeof(int16_t)},
};

const int p60dock_cal_count = sizeof(p60dock_calibration) / sizeof(p60dock_calibration[0]);

/**
 * Setup info about hk parameters
 */
const param_table_t p60dock_hk[] = {
	{.name = "c_out", 			.addr = P60DOCK_HK_C_OUT(0),		.type = PARAM_INT16, 	.size = sizeof(int16_t),  .count = 13},
	{.name = "v_out", 			.addr = P60DOCK_HK_V_OUT(0),		.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 13},
	{.name = "out_en", 			.addr = P60DOCK_HK_OUT_EN(0),		.type = PARAM_UINT8, 	.size = sizeof(uint8_t),  .count = 13},
	{.name = "temp", 			.addr = P60DOCK_HK_TEMP(0),			.type = PARAM_INT16, 	.size = sizeof(int16_t), .count = 2},
	{.name = "bootcause", 		.addr = P60DOCK_HK_BOOT_CAUSE,		.type = PARAM_UINT32, 	.size = sizeof(uint32_t), .flags = PARAM_F_READONLY},
	{.name = "bootcnt", 		.addr = P60DOCK_HK_BOOT_COUNTER,	.type = PARAM_UINT32, 	.size = sizeof(uint32_t), .flags = PARAM_F_PERSIST},
	{.name = "uptime", 			.addr = P60DOCK_HK_UPTIME,			.type = PARAM_UINT32, 	.size = sizeof(uint32_t), .flags = PARAM_F_READONLY},
	{.name = "resetcause", 		.addr = P60DOCK_HK_RESET_CAUSE,		.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .flags = PARAM_F_PERSIST},
	{.name = "batt_mode", 		.addr = P60DOCK_HK_BATT_MODE,		.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
	{.name = "heater_on", 		.addr = P60DOCK_HK_HEATER_ON,		.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
	{.name = "conv_5v_en", 		.addr = P60DOCK_HK_CONV_5V0_EN,		.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
	{.name = "latchup", 		.addr = P60DOCK_HK_LATCHUP(0),		.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 13, .flags = PARAM_F_PERSIST},
	{.name = "vbat_v", 			.addr = P60DOCK_HK_VBAT_V,			.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
	{.name = "vcc_c", 			.addr = P60DOCK_HK_VCC_C,			.type = PARAM_INT16, 	.size = sizeof(int16_t)},
	{.name = "batt_c", 			.addr = P60DOCK_HK_BATTERY_C,		.type = PARAM_INT16, 	.size = sizeof(int16_t)},
	{.name = "batt_v", 			.addr = P60DOCK_HK_BATTERY_V,		.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
	{.name = "batt_temp", 		.addr = P60DOCK_HK_BP_TEMP(0),		.type = PARAM_INT16, 	.size = sizeof(int16_t), .count = 2},
	{.name = "device_type", 	.addr = P60DOCK_HK_DEVICE_TYPE(0),	.type = PARAM_UINT8, 	.size = sizeof(uint8_t),  .count = 8},
	{.name = "device_status", 	.addr = P60DOCK_HK_DEVICE_STATUS(0),.type = PARAM_UINT8, 	.size = sizeof(uint8_t),  .count = 8},
	{.name = "dearm_status", 	.addr = P60DOCK_HK_DEARM_STATUS,	.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
	{.name = "wdt_cnt_gnd", 	.addr = P60DOCK_HK_CNT_WDTGND,		.type = PARAM_UINT32, 	.size = sizeof(uint32_t), .flags = PARAM_F_PERSIST},
	{.name = "wdt_cnt_i2c", 	.addr = P60DOCK_HK_CNT_WDTI2C,		.type = PARAM_UINT32, 	.size = sizeof(uint32_t), .flags = PARAM_F_PERSIST},
	{.name = "wdt_cnt_can", 	.addr = P60DOCK_HK_CNT_WDTCAN,		.type = PARAM_UINT32, 	.size = sizeof(uint32_t), .flags = PARAM_F_PERSIST},
	{.name = "wdt_cnt_csp", 	.addr = P60DOCK_HK_CNT_WDTCSP(0),	.type = PARAM_UINT32, 	.size = sizeof(uint32_t), .count = 2, .flags = PARAM_F_PERSIST},
	{.name = "wdt_gnd_left", 	.addr = P60DOCK_HK_WDTGND_LEFT,		.type = PARAM_UINT32, 	.size = sizeof(uint32_t)},
	{.name = "wdt_i2c_left", 	.addr = P60DOCK_HK_WDTI2C_LEFT,		.type = PARAM_UINT32, 	.size = sizeof(uint32_t)},
	{.name = "wdt_can_left", 	.addr = P60DOCK_HK_WDTCAN_LEFT,		.type = PARAM_UINT32, 	.size = sizeof(uint32_t)},
	{.name = "wdt_csp_left", 	.addr = P60DOCK_HK_WDTCSP_LEFT(0),	.type = PARAM_UINT8,	.size = sizeof(uint8_t), .count = 2},
};

const int p60dock_hk_count = sizeof(p60dock_hk) / sizeof(p60dock_hk[0]);

int p60dock_get_hk(param_index_t * mem, uint8_t node, uint32_t timeout) {

	mem->table = p60dock_hk;
	mem->mem_id = P60DOCK_HK;
	mem->count = p60dock_hk_count;
	mem->size = P60DOCK_HK_SIZE;
	int result = rparam_get_full_table(mem, node, P60_PORT_RPARAM, mem->mem_id, timeout);

	return (result == 0);

}

int p60dock_gndwdt_clear(uint8_t node, uint32_t timeout) {
	uint8_t magic = 0x78;
	return csp_transaction(CSP_PRIO_HIGH, node, P60_PORT_GNDWDT_RESET, timeout, &magic, 1, NULL, 0);
}
