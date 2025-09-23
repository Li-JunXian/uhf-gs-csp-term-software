#ifndef INCLUDE_EPS_HK_H_
#define INCLUDE_EPS_HK_H_

#include <stddef.h>
#include <stdint.h>
#include <param/param_types.h>
#include <eps.h>

/**
 * Houskeeping structure:
 * This data is updated from various sources at various intervals
 * The information here is always converted into "human readable" format
 * i.e. mA, mV, counters and status bits 0/1.
 *
 * Missing variables from HOUSEKEEPING2
 * uint8_t  SOC;          	    	//! State of charge [%]
 * int32_t  Icount;       	    	//! Coloumb count [As] AmpereSeconds
 *
 */
typedef struct hk_s {

	/** Updated by task_hk.c at 2 Hz */
	uint16_t vboost[3];				//! Voltage of boost converters [mV] [PV1, PV2, PV3]
	uint16_t vbatt;					//! Voltage of battery [mV]
	uint16_t curout[6];				//! Current out [mA]
	uint16_t curin[3];				//! Current in [mA]
	uint16_t cursun;				//! Current from boost converters
	uint16_t cursys;				//! Current out of battery
	int16_t temp[6];				//! Temperature sensors [0 = TEMP1, TEMP2, TEMP3, TEMP4, BATT0, BATT1]
	uint8_t output[8];				//! Status of outputs
	uint16_t output_on_delta[8];	//! Time till power on
	uint16_t output_off_delta[8];	//! Time till power off
	uint32_t wdt_i2c_time_left;		//! Time left on I2C wdt
	uint32_t wdt_gnd_time_left;		//! Time left on I2C wdt
	uint32_t counter_boot; 			//! Number of EPS reboots
	uint32_t counter_wdt_i2c;		//! Number of WDT I2C reboots
	uint32_t counter_wdt_gnd;		//! Number of WDT GND reboots
	uint32_t counter_wdt_csp[2];	//! Number of WDT CSP reboots
	uint8_t  wdt_csp_pings_left[2];	//! Pings left on CSP wdt

	/** Updated by main at boot */
	uint8_t	bootcause;				//! Cause of last EPS reset

	/** Updated by latchup.c */
	uint16_t latchup[6];			//! Number of latch-ups

	/** Updated by task_mppt.c */
	uint8_t battmode;				//! Mode for battery [0 = normal, 1 = undervoltage, 2 = overvoltage]
	uint8_t pptmode;				//! Mode for PPT [0 = HW, 1 = MPPT, 2 = FIXED]

} __attribute__((packed)) hk_t;

extern hk_t hk;

#define EPS_HK_VBOOST(i)		(offsetof(hk_t, vboost) + sizeof(uint16_t) * i)
#define EPS_HK_VBATT			(offsetof(hk_t, vbatt))
#define EPS_HK_CUROUT(i)		(offsetof(hk_t, curout) + sizeof(uint16_t) * i)
#define EPS_HK_CURIN(i)			(offsetof(hk_t, curin) + sizeof(uint16_t) * i)
#define EPS_HK_CURSUN			(offsetof(hk_t, cursun))
#define EPS_HK_CURSYS			(offsetof(hk_t, cursys))
#define EPS_HK_TEMP(i)			(offsetof(hk_t, temp) + sizeof(int16_t) * i)
#define EPS_HK_OUT_VAL(i)		(offsetof(hk_t, output) + sizeof(uint8_t) * i)
#define EPS_HK_WDT_I2C_S		(offsetof(hk_t, wdt_i2c_time_left))
#define EPS_HK_WDT_GND_S		(offsetof(hk_t, wdt_gnd_time_left))
#define EPS_HK_CNT_BOOT			(offsetof(hk_t, counter_boot))
#define EPS_HK_CNT_WDTI2C		(offsetof(hk_t, counter_wdt_i2c))
#define EPS_HK_CNT_WDTGND		(offsetof(hk_t, counter_wdt_gnd))
#define EPS_HK_CNT_WDTCSP(i)	(offsetof(hk_t, counter_wdt_csp) + sizeof(uint32_t) * i)
#define EPS_HK_WDT_CSP_C(i)		(offsetof(hk_t, wdt_csp_pings_left) + sizeof(uint8_t) * i)
#define EPS_HK_BOOTCAUSE		(offsetof(hk_t, bootcause))
#define EPS_HK_LATCHUPS(i)		(offsetof(hk_t, latchup) + sizeof(uint16_t) * i)
#define EPS_HK_BATTMODE			(offsetof(hk_t, battmode))
#define EPS_HK_PPTMODE			(offsetof(hk_t, pptmode))

#define EPS_HK_SIZE				sizeof(hk_t)

#ifdef __AVR__
static const param_table_t PROGMEM eps_hk[] = {
#else
static const param_table_t eps_hk[] = {
#endif
	{.name = "vboost", 		.addr = EPS_HK_VBOOST(0),			.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 3},
	{.name = "vbatt", 		.addr = EPS_HK_VBATT,				.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
	{.name = "curout", 		.addr = EPS_HK_CUROUT(0),			.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 6},
	{.name = "curin", 		.addr = EPS_HK_CURIN(0),			.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 3},
	{.name = "cursun", 		.addr = EPS_HK_CURSUN,				.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
	{.name = "cursys", 		.addr = EPS_HK_CURSYS,				.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
	{.name = "temp", 		.addr = EPS_HK_TEMP(0),				.type = PARAM_INT16, 	.size = sizeof(int16_t), .count = 6},
	{.name = "out_val",		.addr = EPS_HK_OUT_VAL(0),			.type = PARAM_UINT8, 	.size = sizeof(uint8_t), .count = 8},

	{.name = "wdtI2cS", 	.addr = EPS_HK_WDT_I2C_S,			.type = PARAM_UINT32, 	.size = sizeof(uint32_t)},
	{.name = "wdtGndS", 	.addr = EPS_HK_WDT_GND_S,			.type = PARAM_UINT32, 	.size = sizeof(uint32_t)},
	{.name = "cntBoot", 	.addr = EPS_HK_CNT_BOOT,			.type = PARAM_UINT32, 	.size = sizeof(uint32_t), .flags = PARAM_F_PERSIST},
	{.name = "cntWdtI2c", 	.addr = EPS_HK_CNT_WDTI2C,			.type = PARAM_UINT32, 	.size = sizeof(uint32_t), .flags = PARAM_F_PERSIST},
	{.name = "cntWdtGnd", 	.addr = EPS_HK_CNT_WDTGND,			.type = PARAM_UINT32, 	.size = sizeof(uint32_t), .flags = PARAM_F_PERSIST},
	{.name = "cntWdtCsp", 	.addr = EPS_HK_CNT_WDTCSP(0),		.type = PARAM_UINT32, 	.size = sizeof(uint32_t), .count = 2, .flags = PARAM_F_PERSIST},
	{.name = "wdtCspC", 	.addr = EPS_HK_WDT_CSP_C(0),		.type = PARAM_UINT8, 	.size = sizeof(uint8_t), .count = 2, .flags = PARAM_F_PERSIST},

	{.name = "bootcause", 	.addr = EPS_HK_BOOTCAUSE,			.type = PARAM_UINT8, 	.size = sizeof(uint8_t), .flags = PARAM_F_PERSIST},

	{.name = "latchups",	.addr = EPS_HK_LATCHUPS(0),			.type = PARAM_UINT16, 	.size = sizeof(uint16_t), .count = 6},

	{.name = "battmode", 	.addr = EPS_HK_BATTMODE,			.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
	{.name = "pptmode", 	.addr = EPS_HK_PPTMODE,				.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
};

#define EPS_HK_COUNT (sizeof(eps_hk) / sizeof(eps_hk[0]))

void eps_param_restore(void);

#endif /* INCLUDE_EPS_HK_H_ */
