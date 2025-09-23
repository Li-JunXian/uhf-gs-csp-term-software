#ifndef INCLUDE_EPS_CAL_H_
#define INCLUDE_EPS_CAL_H_

#include <stddef.h>
#include <stdint.h>
#include <param/param_types.h>
#include <eps.h>

#define EPS_CAL_VREF				0x00
#define EPS_CAL_GAIN_VBATT			0x04
#define EPS_CAL_GAIN_VBOOST(i)		(0x08 + i*4)
#define EPS_CAL_GAIN_DAC(i)			(0x14 + i*4)
#define EPS_CAL_GAIN_CURIN(i)		(0x20 + i*4)
#define EPS_CAL_GAIN_CUROUT(i)		(0x2C + i*4)
#define EPS_CAL_GAIN_CURSUN			0x44
#define EPS_CAL_GAIN_CURSYS			0x48
#define EPS_CAL_OFFSET_CURIN(i)		(0x4C + i*4)
#define EPS_CAL_OFFSET_CUROUT(i)	(0x58 + i*4)
#define EPS_CAL_OFFSET_CURSUN		0x70
#define EPS_CAL_OFFSET_CURSYS		0x74

#define EPS_CAL_SIZE				0x78

static const param_table_t PROGMEM eps_cal[] = {
		{.name = "vref", 			.addr = EPS_CAL_VREF,						.type = PARAM_FLOAT,	.size = sizeof(float)},
		{.name = "gainVbatt", 		.addr = EPS_CAL_GAIN_VBATT,					.type = PARAM_FLOAT,	.size = sizeof(float)},
		{.name = "gainVboost", 		.addr = EPS_CAL_GAIN_VBOOST(0),				.type = PARAM_FLOAT,	.size = sizeof(float), .count = 3},
		{.name = "gainDac", 		.addr = EPS_CAL_GAIN_DAC(0),				.type = PARAM_FLOAT,	.size = sizeof(float), .count = 3},
		{.name = "gainCurIn", 		.addr = EPS_CAL_GAIN_CURIN(0),				.type = PARAM_FLOAT,	.size = sizeof(float), .count = 3},
		{.name = "gainCurOut", 		.addr = EPS_CAL_GAIN_CUROUT(0),				.type = PARAM_FLOAT,	.size = sizeof(float), .count = 6},
		{.name = "gainCurSys", 		.addr = EPS_CAL_GAIN_CURSYS,				.type = PARAM_FLOAT,	.size = sizeof(float)},
		{.name = "gainCurSun", 		.addr = EPS_CAL_GAIN_CURSUN,				.type = PARAM_FLOAT,	.size = sizeof(float)},
		{.name = "offCurIn", 		.addr = EPS_CAL_OFFSET_CURIN(0),			.type = PARAM_FLOAT,	.size = sizeof(float), .count = 3},
		{.name = "offCurOut", 		.addr = EPS_CAL_OFFSET_CUROUT(0),			.type = PARAM_FLOAT,	.size = sizeof(float), .count = 6},
		{.name = "offCurSys", 		.addr = EPS_CAL_OFFSET_CURSUN,				.type = PARAM_FLOAT,	.size = sizeof(float)},
		{.name = "offCurSun", 		.addr = EPS_CAL_OFFSET_CURSYS,				.type = PARAM_FLOAT,	.size = sizeof(float)},
};

static const int eps_cal_count = sizeof(eps_cal) / sizeof(eps_cal[0]);

#define CAL_VBATT_GAIN 3.16
#define CAL_VBOOST_GAIN 3.16
#define CAL_DAC_GAIN 2.945

#define CAL_CURIN_GAIN 1.0/(0.010*100.0)
#define CAL_CUROUT_GAIN 1.0/(0.025*50.0)
#define CAL_CURSYS_GAIN 1.0/(0.025*50.0)
#define CAL_CURSUN_GAIN 1.0/(0.005*100.0)

void eps_cal_restore(void);

#define calibration_get_curout_offset(i) 	param_get_float(EPS_CAL_OFFSET_CUROUT(i), EPS_CAL)
#define calibration_get_curin_offset(i)		param_get_float(EPS_CAL_OFFSET_CURIN(i), EPS_CAL)
#define calibration_get_cursys_offset()		param_get_float(EPS_CAL_OFFSET_CURSYS, EPS_CAL)
#define calibration_get_cursun_offset()		param_get_float(EPS_CAL_OFFSET_CURSUN, EPS_CAL)

#define calibration_get_vref_mv()			param_get_float(EPS_CAL_VREF, EPS_CAL)
#define calibration_get_vbatt_gain()		param_get_float(EPS_CAL_GAIN_VBATT, EPS_CAL)
#define calibration_get_vboost_gain(i)		param_get_float(EPS_CAL_GAIN_VBOOST(i), EPS_CAL)
#define calibration_get_dac_gain(i)			param_get_float(EPS_CAL_GAIN_DAC(i), EPS_CAL)
#define calibration_get_curin_gain(i)		param_get_float(EPS_CAL_GAIN_CURIN(i), EPS_CAL)
#define calibration_get_curout_gain(i)		param_get_float(EPS_CAL_GAIN_CUROUT(i), EPS_CAL)
#define calibration_get_cursys_gain()		param_get_float(EPS_CAL_GAIN_CURSYS, EPS_CAL)
#define calibration_get_cursun_gain()		param_get_float(EPS_CAL_GAIN_CURSUN, EPS_CAL)

#define calibration_set_curout_offset(i, v)	param_set_float(EPS_CAL_OFFSET_CUROUT(i), EPS_CAL, v)
#define calibration_set_curin_offset(i, v)	param_set_float(EPS_CAL_OFFSET_CURIN(i), EPS_CAL, v)
#define calibration_set_cursys_offset(v)	param_set_float(EPS_CAL_OFFSET_CURSYS, EPS_CAL, v)
#define calibration_set_cursun_offset(v)	param_set_float(EPS_CAL_OFFSET_CURSUN, EPS_CAL, v)

#define calibration_set_vref_mv(v)			param_set_float(EPS_CAL_VREF, EPS_CAL, v)
#define calibration_set_vbatt_gain(v)		param_set_float(EPS_CAL_GAIN_VBATT, EPS_CAL, v)
#define calibration_set_vboost_gain(i, v)	param_set_float(EPS_CAL_GAIN_VBOOST(i), EPS_CAL, v)
#define calibration_set_dac_gain(i, v)		param_set_float(EPS_CAL_GAIN_DAC(i), EPS_CAL, v)
#define calibration_set_curin_gain(i, v)	param_set_float(EPS_CAL_GAIN_CURIN(i), EPS_CAL, v)
#define calibration_set_curout_gain(i, v)	param_set_float(EPS_CAL_GAIN_CUROUT(i), EPS_CAL, v)
#define calibration_set_cursys_gain(v)		param_set_float(EPS_CAL_GAIN_CURSYS, EPS_CAL, v)
#define calibration_set_cursun_gain(v)		param_set_float(EPS_CAL_GAIN_CURSUN, EPS_CAL, v)

#endif /* INCLUDE_EPS_CAL_H_ */
