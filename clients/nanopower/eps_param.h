#ifndef INCLUDE_EPS_PARAM_H_
#define INCLUDE_EPS_PARAM_H_

#include <stddef.h>
#include <stdint.h>
#include <param/param_types.h>
#include <eps.h>

#define EPS_PARAM_PPTMODE		0x00
#define EPS_PARAM_HEATERMODE	0x01
#define EPS_PARAM_HEATER_LOW	0x02
#define EPS_PARAM_HEATER_HIGH	0x03
#define EPS_PARAM_VBOOST(i)		(0x04 + 2 * i)

#define EPS_PARAM_OUT_EN(i)		(0x10 + (1 * i))
#define EPS_PARAM_OUT_ON_IN(i)	(0x20 + (2 * i))
#define EPS_PARAM_OUT_OFF_IN(i)	(0x30 + (2 * i))

#define EPS_PARAM_CUR_LIM(i)	(0x40 + (2 * i))
#define EPS_PARAM_CUR_EMA(i)	(0x50 + (2 * i))
#define EPS_PARAM_CUR_EMA_GAIN	0x5C

#define EPS_PARAM_SIZE			0x60

static const param_table_t PROGMEM eps_param[] = {

		{.name = "ppt_mode", 		.addr = EPS_PARAM_PPTMODE,					.type = PARAM_UINT8,	.size = sizeof(uint8_t)},
		{.name = "heater_mode", 	.addr = EPS_PARAM_HEATERMODE, 				.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "heater_low", 		.addr = EPS_PARAM_HEATER_LOW, 				.type = PARAM_INT8, 	.size = sizeof(int8_t)},
		{.name = "heater_high", 	.addr = EPS_PARAM_HEATER_HIGH, 				.type = PARAM_INT8, 	.size = sizeof(int8_t)},
		{.name = "vboost", 			.addr = EPS_PARAM_VBOOST(0), 				.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 3},

		{.name = "out_en", 			.addr = EPS_PARAM_OUT_EN(0),				.type = PARAM_UINT8, 	.size = sizeof(uint8_t), .count = 8},
		{.name = "out_on_in", 		.addr = EPS_PARAM_OUT_ON_IN(0),				.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 8},
		{.name = "out_off_in", 		.addr = EPS_PARAM_OUT_OFF_IN(0),			.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 8},

		{.name = "cur_lim", 		.addr = EPS_PARAM_CUR_LIM(0),				.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 6},
		{.name = "cur_ema", 		.addr = EPS_PARAM_CUR_EMA(0),				.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 6},
		{.name = "cur_ema_gain", 	.addr = EPS_PARAM_CUR_EMA_GAIN,				.type = PARAM_FLOAT, 	.size = sizeof(float)},

};

static const int eps_param_count = sizeof(eps_param) / sizeof(eps_param[0]);

void eps_param_callback(uint16_t addr, param_index_t * mem);

#endif /* INCLUDE_EPS_PARAM_H_ */
