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

#include <p60acu.h>

/**
 * Setup info about configuration parameters
 */
const param_table_t p60acu_config[] = {
	{.name = "mppt_mode",		.addr = P60ACU_MPPT_MODE,			.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
	{.name = "mppt_d_mode",		.addr = P60ACU_MPPT_DELTA_MODE,		.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
	{.name = "vboost", 			.addr = P60ACU_MPPT_VBOOST(0),		.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 6},
	{.name = "vbat_max_hi",		.addr = P60ACU_VBAT_MAX_HI,			.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
	{.name = "vbat_max_lo",		.addr = P60ACU_VBAT_MAX_LO,			.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
	{.name = "mppt_period",		.addr = P60ACU_MPPT_PERIOD,			.type = PARAM_UINT32, 	.size = sizeof(uint32_t)},
	{.name = "max_dv",			.addr = P60ACU_MPPT_MAX_DVBOOST,	.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
	{.name = "ov_mode",			.addr = P60ACU_MPPT_OV_MODE,		.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
};

const int p60acu_config_count = sizeof(p60acu_config) / sizeof(p60acu_config[0]);

/**
 * Setup info about calibration parameters
 */
const param_table_t p60acu_calibration[] = {
	{.name = "vref",			.addr = P60ACU_CAL_VREF,			.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
	{.name = "gain_vcc", 		.addr = P60ACU_CAL_GAIN_VCC,		.type = PARAM_FLOAT,	.size = sizeof(float)},
	{.name = "gain_vbat", 		.addr = P60ACU_CAL_GAIN_VBAT,		.type = PARAM_FLOAT,	.size = sizeof(float)},
	{.name = "gain_v_in",		.addr = P60ACU_CAL_GAIN_V_IN(0),	.type = PARAM_FLOAT,	.size = sizeof(float), .count = 6},
	{.name = "gain_c_in", 		.addr = P60ACU_CAL_GAIN_C_IN(0),	.type = PARAM_FLOAT,	.size = sizeof(float), .count = 6},
	{.name = "offs_c_in", 		.addr = P60ACU_CAL_OFFSET_C_IN(0),	.type = PARAM_INT16,	.size = sizeof(float), .count = 6},
	{.name = "gain_dac", 		.addr = P60ACU_CAL_GAIN_DAC(0),		.type = PARAM_FLOAT,	.size = sizeof(float), .count = 6},
};

const int p60acu_cal_count = sizeof(p60acu_calibration) / sizeof(p60acu_calibration[0]);

/**
 * Setup info about hk parameters
 */
const param_table_t p60acu_hk[] = {
	{.name = "c_in", 			.addr = P60ACU_HK_C_IN(0),			.type = PARAM_INT16, 	.size = sizeof(uint16_t), .count = 6},
	{.name = "v_in", 			.addr = P60ACU_HK_V_IN(0),			.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 6},
	{.name = "vbat", 			.addr = P60ACU_HK_VBAT,				.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
	{.name = "vcc", 			.addr = P60ACU_HK_VCC,				.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
	{.name = "temp", 			.addr = P60ACU_HK_TEMP(0),			.type = PARAM_INT16, 	.size = sizeof(int16_t), .count = 3},
	{.name = "mppt_mode", 		.addr = P60ACU_HK_MPPT_MODE,		.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
	{.name = "vboost", 			.addr = P60ACU_HK_MPPT_VBOOST(0),	.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 6},
	{.name = "power", 			.addr = P60ACU_HK_PWR(0),			.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 6},
	{.name = "dac_en", 			.addr = P60ACU_HK_DAC_EN(0),		.type = PARAM_UINT8, 	.size = sizeof(uint8_t), .count = 3},
	{.name = "dac_val", 		.addr = P60ACU_HK_MPPT_DAC_VAL(0),	.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 6},
	{.name = "bootcause", 		.addr = P60ACU_HK_BOOT_CAUSE,		.type = PARAM_UINT32, 	.size = sizeof(uint32_t), .flags = PARAM_F_READONLY},
	{.name = "bootcnt", 		.addr = P60ACU_HK_BOOT_COUNTER,		.type = PARAM_UINT32, 	.size = sizeof(uint32_t), .flags = PARAM_F_PERSIST},
	{.name = "uptime", 			.addr = P60ACU_HK_UPTIME,			.type = PARAM_UINT32, 	.size = sizeof(uint32_t), .flags = PARAM_F_READONLY},
	{.name = "resetcause", 		.addr = P60ACU_HK_RESET_CAUSE,		.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .flags = PARAM_F_PERSIST},
	{.name = "mppt_time", 		.addr = P60ACU_HK_MPPT_TIME,		.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
	{.name = "mppt_period", 	.addr = P60ACU_HK_MPPT_PERIOD,		.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
	{.name = "device_type", 	.addr = P60ACU_HK_DEVICE_TYPE(0),	.type = PARAM_UINT8, 	.size = sizeof(uint8_t),  .count = 8},
	{.name = "device_status", 	.addr = P60ACU_HK_DEVICE_STATUS(0),	.type = PARAM_UINT8, 	.size = sizeof(uint8_t),  .count = 8},
	{.name = "wdt_cnt_gnd", 	.addr = P60ACU_HK_CNT_WDTGND,		.type = PARAM_UINT32, 	.size = sizeof(uint32_t), .flags = PARAM_F_PERSIST},
	{.name = "wdt_gnd_left", 	.addr = P60ACU_HK_WDTGND_LEFT,		.type = PARAM_UINT32, 	.size = sizeof(uint32_t)},
};

const int p60acu_hk_count = sizeof(p60acu_hk) / sizeof(p60acu_hk[0]);

int p60acu_get_hk(param_index_t * mem, uint8_t node, uint32_t timeout) {

	mem->table = p60acu_hk;
	mem->mem_id = P60ACU_HK;
	mem->count = p60acu_hk_count;
	mem->size = P60ACU_HK_SIZE;
	int result = rparam_get_full_table(mem, node, P60_PORT_RPARAM, mem->mem_id, timeout);

	return (result == 0);

}

int p60acu_gndwdt_clear(uint8_t node, uint32_t timeout) {
	uint8_t magic = 0x78;
	return csp_transaction(CSP_PRIO_HIGH, node, P60_PORT_GNDWDT_RESET, timeout, &magic, 1, NULL, 0);
}
