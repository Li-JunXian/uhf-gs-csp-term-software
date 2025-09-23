#ifndef GSSB_H_
#define GSSB_H_

#define GSSB_PORT 15

enum {
	CMD_SUN_SAMPLE_SUNSENSOR = 0,		/* Sunsensor */
	CMD_SUN_GET_SUNSENSOR_DATA = 1,		/* Sunsensor */
	CMD_SUN_ADC_CONF = 2,			/* Sunsensor */
	CMD_SUN_ADC_SAVE_CONF = 3,		/* Sunsensor */
	CMD_SUN_SAMPLE_TEMP = 4,		/* Sunsensor */
	CMD_SUN_GET_TEMP = 5,			/* Sunsensor */
	CMD_SET_I2C_ADDR = 6,			/* General */
	CMD_COMMIT_I2C_ADDR = 7,		/* General */
	CMD_GET_ID = 8,				/* General */
	CMD_ISTAGE_GET_RELEASE_STATUS = 9,	/* Interstage */
	CMD_ISTAGE_GET_TEMP = 10,		/* Interstage */
	CMD_ISTAGE_BURN = 11,			/* Interstage */
	CMD_ISTAGE_GET_SUNSENSOR_VOLTAGE = 12,	/* Interstage */
	CMD_GET_UUID = 13,			/* General */
	CMD_ISTAGE_GET_BURN_SETTINGS1 = 14,	/* Interstage */
	CMD_ISTAGE_GET_BURN_SETTINGS2 = 15,	/* Interstage */
	CMD_ISTAGE_SET_BURN_SETTINGS1 = 16,	/* Interstage */
	CMD_ISTAGE_SET_BURN_SETTINGS2 = 17,	/* Interstage */
	CMD_ISTAGE_SET_BURN_SETTINGS3 = 18,	/* Interstage */
	CMD_ISTAGE_GET_BURN_COUNTERS = 19,	/* Interstage */
	CMD_ISTAGE_SETTINGS_UNLOCK = 20,	/* Interstage */
	CMD_ISTAGE_GET_BURN_CNT_RUNNING = 21,	/* Interstage */
	CMD_SOFT_RESET = 22,			/* Interstage */
	CMD_ISTAGE_CHANGE_STATE = 23,		/* Interstage */
	CMD_SUN_SAMPLE_GET_SUNSENSOR = 100,	/* Sunsensor (only used on CSP interface)*/
	CMD_SUN_SAMPLE_GET_TEMP = 101,		/* Sunsensor (only used on CSP interface) */

};

enum {
	GSSB_CMD_IDENT = 110,			/* Request gssb dev version and uuid */
	GSSB_CMD_ISTAGE_STATUS,			/* Get interstage status */
	GSSB_CMD_ISTAGE_CONF_GET,		/* Get interstage configuration */
	GSSB_CMD_ISTAGE_CONF_SET,		/* Set interstage configuration */
	GSSB_CMD_ISTAGE_REBOOT,			/* Soft reboot interstage */
	GSSB_CMD_ISTAGE_ARM,			/* Arm interstage for auto deploy */
	GSSB_CMD_ISTAGE_DEPLOY,			/* Manual deploy antenna */
	GSSB_CMD_ISTAGE_STATE,			/* Get the current interstage state */
	GSSB_CMD_ISTAGE_SENSORS,		/* Read interstage sensors */
};

typedef struct __attribute__((__packed__)) istage_burn_settings {
	uint16_t std_time_ms;		// Burn time for first burn [ms]
	uint16_t increment_ms;		// How much to increment burn time for each retry [ms]
	uint16_t short_cnt_down; 	// The burn process will run once after this cntr runs out [s]
	uint8_t max_repeat;		// Max number of retries
	uint8_t rep_time_s;		// Time between retries [s]
	uint8_t switch_polarity;	// Release sense switch polarity
	uint8_t status;			
	uint8_t locked;
	uint8_t reboot_deploy_cnt;
} istage_burn_settings_t;

typedef struct __attribute__((__packed__)) gssb_csp_cmd_resp {
	int8_t return_code;
	uint8_t buff[8];
} gssb_csp_cmd_resp_t;

typedef struct __attribute__((__packed__)) gssb_csp_cmd {
	uint8_t gssb_csp_cmd;
	uint8_t addr;
	uint8_t buff[9];
	uint8_t tx_len;
	uint8_t rx_len;
	uint16_t timeout_ms;
} gssb_csp_cmd_t;

typedef struct __attribute__((__packed__)) gssb_csp_cmd2 {
	uint8_t cmd_id;
	uint8_t i2c_addr;
	uint16_t i2c_timeout_ms;
} gssb_csp_cmd2_t;

typedef struct __attribute__((__packed__)) gssb_csp_ident {
	int8_t 		i2c_res;
	uint32_t 	uuid;
	uint8_t 	version[8];
} gssb_csp_ident_t;

typedef struct __attribute__((__packed__)) gssb_csp_istage_status {
	int8_t 		i2c_res;		// Result of i2c transaction with gssb dev
	uint16_t 	deploy_in_s;		// Time before deploying
	uint8_t 	number_of_deploys;	// Number of deploy tries since boot
	uint8_t		active_knife;		// The thermal knife that will be used in next deploy
	uint8_t		state;			// Interstage state. 0 = not armed, 1 = armed for manual deploy, 2 = armed for auto deploy, 3 and 4 = deploying
	uint16_t	total_number_of_deploys;// Total number of deploys in interstage life time
	uint8_t		reboot_deploy_cnt;	// Number of reboots left before interstage will deploy on boot
	uint8_t 	release_status;		// Release switch state. 1 = released, 0 = not released
} gssb_istage_status_t;

typedef struct __attribute__((__packed__)) gssb_csp_istage_sensors {
	int8_t 		i2c_res;
	uint16_t 	sun_voltage;
	int16_t		panel_temp;
} gssb_istage_sensors_t;

typedef struct __attribute__((__packed__)) istage_burn_settings_cmd {
	gssb_csp_cmd2_t cmd;
	istage_burn_settings_t settings;
} istage_burn_settings_cmd_t;

typedef struct __attribute__((__packed__)) istage_arm_cmd {
	gssb_csp_cmd2_t cmd;
	uint8_t 	arm;
} istage_arm_cmd_t;

typedef struct __attribute__((__packed__)) istage_state_cmd {
	gssb_csp_cmd2_t cmd;
	uint8_t 	data;
} istage_state_cmd_t;


/**
 * Set the gssb server node
 * @param node csp address of node
 * @param timeout timeout of commands
 */
void gssb_node(uint8_t node, uint16_t timeout);

/**
 * Used for specifying which gssb device to command
 * @param gssb_i2c_addr gssb device i2c address
 * @param gssb_i2c_timeout_ms timeout for i2c communication with gssb device
 */
void gssb_dev(uint8_t gssb_i2c_addr, uint16_t gssb_i2c_timeout_ms);

/**
 * Send command to gssb server
 * @param cmd command to server
 * @param resp pointer to struct of the expected response type
 * @param resp_size expected size of respons
 * @return > 0 on success else <= 0
 */
int gssb_cmd_csp(gssb_csp_cmd2_t * cmd, void * resp, uint8_t resp_size);

/**
 * Request version and uuid from gssb device
 * @param resp pointer to gssb_csp_ident_t
 * @return > 0 on success else <= 0
 */
int gssb_ident(gssb_csp_ident_t * resp);

/**
 * Soft reboot a gssb device
 * @return > 0 on success else <= 0
 */
int gssb_reboot(void);

/**
 * Sends antenna deplot command to interstage
 * @return > 0 on success else <= 0
 */
int gssb_istage_deploy(void);

/**
 * Sends arm command to interstage.
 * @param arm 1 for auto deploy and 0 for not armed
 * @return > 0 on success else <= 0
 */
int gssb_istage_arm(uint8_t arm);

/**
 * Sets the state of the interstage. One need to use this command
 * to set the interstage in the armed manual state before it is possible
 * to use the deploy command
 * @param state. 0 = Not armed, 1 = armed manual
 * @return > 0 on success else <= 0
 */
int gssb_istage_set_state(uint8_t state);

/**
 * Get interstage sensor values
 * @param sensors pointer to gssb_istage_sensors_t
 * @return > 0 on success else <= 0
 */
int gssb_istage_sensors(gssb_istage_sensors_t * sensors);

/**
 * Get the configuration from the interstage
 * @param settings pointer to istage_burn_settings_t 
 * @return > 0 on success else <= 0
 */
int gssb_interstage_settings_get(istage_burn_settings_t * settings);

/**
 * Sets interstage configuration
 * @param settings pointer to istage_burn_settings_t 
 * @return > 0 on success else <= 0
 */
int gssb_interstage_settings_set(istage_burn_settings_t * settings);

/**
 * Request interstage status
 * @param status pointer to gssb_istage_status_t
 * @return > 0 on success else <= 0
 */
int gssb_interstage_status(gssb_istage_status_t * status);


int gssb_get_csp(uint8_t csp_node, int csp_timeout, gssb_csp_cmd_t * cmd, gssb_csp_cmd_resp_t * response);
int sunsensor_get_data_csp(uint8_t csp_node, int csp_timeout, uint8_t addr, uint16_t timeout_ms, gssb_csp_cmd_resp_t * response, uint16_t *sun);
int sunsensor_sample_data_csp(uint8_t csp_node, int csp_timeout, uint8_t addr, uint16_t timeout_ms, gssb_csp_cmd_resp_t * response);
int sunsensor_sample_get_data_csp(uint8_t csp_node, int csp_timeout, uint8_t addr, uint16_t timeout_ms, gssb_csp_cmd_resp_t * response);
int sunsensor_get_temp_csp(uint8_t csp_node, int csp_timeout, uint8_t addr, uint16_t timeout_ms, gssb_csp_cmd_resp_t * response, float *temp);
int sunsensor_sample_temp_csp(uint8_t csp_node, int csp_timeout, uint8_t addr, uint16_t timeout_ms, gssb_csp_cmd_resp_t * response);
int sunsensor_sample_get_temp_csp(uint8_t csp_node, int csp_timeout, uint8_t addr, uint16_t timeout_ms, gssb_csp_cmd_resp_t * response);
int istage_unlock_csp(uint8_t csp_node, int csp_timeout, uint8_t addr, uint16_t timeout_ms, gssb_csp_cmd_resp_t * response);
#endif /* GSSB_H_ */
