#ifndef BPX_H_
#define BPX_H_

#include <stdint.h>

#define NODE_BPX                7

#define BPX_PORT_RESET  7 //For slave interface
#define BPX_PORT_HK		9
#define BPX_PORT_RESET_COUNTERS 15
#define BPX_PORT_CONFIG_CMD 17
#define BPX_PORT_CONFIG_GET 18
#define BPX_PORT_CONFIG_SET 19




typedef struct __attribute__((packed)) {
	uint8_t battheater_mode;						//! Mode for battheater [0 = Manual, 1 = Auto]
	int8_t battheater_low;							//! Turn heater on at [degC]
	int8_t battheater_high;							//! Trun heater off at [degC]
} bpx_config_t;



typedef struct __attribute__((packed)) {
	uint16_t cur_charge;		//! Charge current in mA
	uint16_t cur_discharge;		//! Discharge current in mA
	uint16_t cur_heater;		//! Heater current in mA
	uint16_t vbatt;			//! Battery voltage in mV
	int16_t bat_temperature1;	//! Battery temperature 1 in degC
	int16_t bat_temperature2;	//! Battery temperature 2 in degC
	int16_t bat_temperature3;	//! Battery temperature 3 in degC
	int16_t bat_temperature4;	//! Battery temperature 4 in degC
	uint32_t counter_boot;		//! Number of reboots
	uint8_t	bootcause;		//! Cause of last reset
} bpx_hk_t;



/**
 * Return houskeeping values
 * @param hk pointer to struct
 * @return -1 if err 0 if ok
 */
int bpx_hk_get(bpx_hk_t * hk);

/**
 * Reset EEPROM based counters
 * @return csp status
 */
int bpx_counters_reset(void);

void bpx_node_set(uint8_t node);


/**
 * Pack, unpack and print config
 * @param conf pointer to config
 */
void bpx_config_print(bpx_config_t * conf);

/**
 * Do config command
 * @param cmd
 * @return csp status
 */
int bpx_config_cmd(uint8_t cmd);

/**
 * Get running bpx config
 * @param config pointner to config
 * @return csp status
 */
int bpx_config_get(bpx_config_t * config);

/**
 * Set running config on bpx
 * @param config pointer to config
 * @return csp status
 */
int bpx_config_set(bpx_config_t * config);





#endif /* BPX_H_ */
