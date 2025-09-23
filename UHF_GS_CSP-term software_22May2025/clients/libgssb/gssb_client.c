#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

#include <util/crc32.h>
#include <util/byteorder.h>

#include <io/gssb.h>

#include <csp/csp.h>
#include <csp/csp_endian.h>

static uint8_t gssb_csp_node = 1;
static uint16_t gssb_csp_timeout = 1000;
static uint8_t i2c_addr = 5;
static uint16_t i2c_timeout_ms = 50;

void gssb_node(uint8_t node, uint16_t timeout) {
	gssb_csp_node = node;
	gssb_csp_timeout = timeout;
}

void gssb_dev(uint8_t gssb_i2c_addr, uint16_t gssb_i2c_timeout_ms) {
	i2c_addr = gssb_i2c_addr;
	i2c_timeout_ms = gssb_i2c_timeout_ms;
}

int gssb_get_csp(uint8_t csp_node, int csp_timeout, gssb_csp_cmd_t * cmd, gssb_csp_cmd_resp_t * response) {
	
	int ret = csp_transaction(CSP_PRIO_NORM, csp_node, 
			GSSB_PORT, csp_timeout, 
			cmd, sizeof(*cmd), 
			response, sizeof(*response));
	
	return ret;
}

int gssb_cmd_csp(gssb_csp_cmd2_t * cmd, void * resp, uint8_t resp_size) {

	int ret = csp_transaction(CSP_PRIO_NORM, gssb_csp_node, 
			GSSB_PORT, gssb_csp_timeout, 
			cmd, sizeof(*cmd), 
			resp, resp_size);
	return ret;


}

int gssb_ident(gssb_csp_ident_t * resp) {
	int res;
	gssb_csp_cmd2_t cmd;

	cmd.cmd_id = GSSB_CMD_IDENT;
	cmd.i2c_addr = i2c_addr;
	cmd.i2c_timeout_ms = csp_hton16(i2c_timeout_ms);

	res = gssb_cmd_csp(&cmd, (void*) resp, sizeof(*resp));
	resp->uuid = csp_ntoh32(resp->uuid);

	return res;
}

int gssb_reboot(void) {
	gssb_csp_cmd2_t cmd;

	cmd.cmd_id = GSSB_CMD_ISTAGE_REBOOT;
	cmd.i2c_addr = i2c_addr;
	cmd.i2c_timeout_ms = csp_hton16(i2c_timeout_ms);

	return gssb_cmd_csp(&cmd, NULL, 0);
}

int gssb_istage_deploy(void) {
	gssb_csp_cmd2_t cmd;

	cmd.cmd_id = GSSB_CMD_ISTAGE_DEPLOY;
	cmd.i2c_addr = i2c_addr;
	cmd.i2c_timeout_ms = csp_hton16(i2c_timeout_ms);

	return gssb_cmd_csp(&cmd, NULL, 0);
}

int gssb_istage_arm(uint8_t arm) {
	istage_arm_cmd_t cmd;
	
	if (arm) {
		cmd.arm = 0x04;
	} else {
		cmd.arm = 0x08;
	}

	cmd.cmd.cmd_id = GSSB_CMD_ISTAGE_ARM;
	cmd.cmd.i2c_addr = i2c_addr;
	cmd.cmd.i2c_timeout_ms = csp_hton16(i2c_timeout_ms);

	return csp_transaction(CSP_PRIO_NORM, gssb_csp_node, GSSB_PORT, gssb_csp_timeout, 
			&cmd, sizeof(cmd), 
			NULL, 0);
}

int gssb_istage_set_state(uint8_t state) {
	istage_state_cmd_t cmd;

	cmd.cmd.cmd_id = GSSB_CMD_ISTAGE_STATE;
	cmd.cmd.i2c_addr = i2c_addr;
	cmd.cmd.i2c_timeout_ms = csp_hton16(i2c_timeout_ms);
	cmd.data = state;


	return csp_transaction(CSP_PRIO_NORM, gssb_csp_node, GSSB_PORT, gssb_csp_timeout, 
			&cmd, sizeof(cmd), 
			NULL, 0);
}

int gssb_istage_sensors(gssb_istage_sensors_t * sensors) {
	int res;
	gssb_csp_cmd2_t cmd;

	cmd.cmd_id = GSSB_CMD_ISTAGE_SENSORS;
	cmd.i2c_addr = i2c_addr;
	cmd.i2c_timeout_ms = csp_hton16(i2c_timeout_ms);

	res = gssb_cmd_csp(&cmd, (void*) sensors, sizeof(*sensors));

	sensors->panel_temp = csp_ntoh16(sensors->panel_temp);
	sensors->sun_voltage = csp_ntoh16(sensors->sun_voltage);

	return res;
}

int gssb_interstage_settings_get(istage_burn_settings_t * settings) {
	int res;
	gssb_csp_cmd2_t cmd;

	cmd.cmd_id = GSSB_CMD_ISTAGE_CONF_GET; 
	cmd.i2c_addr = i2c_addr;
	cmd.i2c_timeout_ms = csp_hton16(i2c_timeout_ms);

	res = gssb_cmd_csp(&cmd, (void*) settings, sizeof(*settings));
	
	settings->std_time_ms = csp_ntoh16(settings->std_time_ms);
	settings->increment_ms = csp_ntoh16(settings->increment_ms);
	settings->short_cnt_down = csp_ntoh16(settings->short_cnt_down);

	return res;
}

int gssb_interstage_settings_set(istage_burn_settings_t * settings) {
	istage_burn_settings_cmd_t conf_cmd;

	conf_cmd.cmd.cmd_id = GSSB_CMD_ISTAGE_CONF_SET; 

	conf_cmd.cmd.i2c_addr = i2c_addr;
	conf_cmd.cmd.i2c_timeout_ms = csp_hton16(i2c_timeout_ms);

	settings->std_time_ms = csp_hton16(settings->std_time_ms);
	settings->increment_ms = csp_hton16(settings->increment_ms);
	settings->short_cnt_down = csp_hton16(settings->short_cnt_down);

	memcpy(&conf_cmd.settings, settings, sizeof(*settings));

	return csp_transaction(CSP_PRIO_NORM, gssb_csp_node, GSSB_PORT, gssb_csp_timeout, 
			&conf_cmd, sizeof(conf_cmd), 
			NULL, 0);
}

int gssb_interstage_status(gssb_istage_status_t * status) {
	int res;
	gssb_csp_cmd2_t cmd;

	cmd.cmd_id = GSSB_CMD_ISTAGE_STATUS;
	cmd.i2c_addr = i2c_addr;
	cmd.i2c_timeout_ms = csp_hton16(i2c_timeout_ms);

	res = gssb_cmd_csp(&cmd, (void*) status, sizeof(*status));

	status->deploy_in_s = csp_ntoh16(status->deploy_in_s);
	status->total_number_of_deploys = csp_ntoh16(status->total_number_of_deploys);

	return res;
}

int sunsensor_get_data_csp(uint8_t csp_node, int csp_timeout, uint8_t addr, uint16_t timeout_ms, gssb_csp_cmd_resp_t * response, uint16_t *sun) {
	gssb_csp_cmd_t cmd;
	int i;
	cmd.gssb_csp_cmd = CMD_SUN_GET_SUNSENSOR_DATA;
	cmd.buff[0] = cmd.gssb_csp_cmd;
	cmd.addr = addr;
	cmd.timeout_ms = timeout_ms;
	cmd.tx_len = 1;
	cmd.rx_len = 8;

	int ret = gssb_get_csp(csp_node, csp_timeout, &cmd, response);
	for (i=0; i <= 7; i += 2) {
		sun[i/2] = (response->buff[i] << 8) + response->buff[i+1];
	}
	return ret;
}

int sunsensor_sample_data_csp(uint8_t csp_node, int csp_timeout, uint8_t addr, uint16_t timeout_ms, gssb_csp_cmd_resp_t * response) {
	gssb_csp_cmd_t cmd;
	cmd.gssb_csp_cmd = CMD_SUN_SAMPLE_SUNSENSOR;
	cmd.buff[0] = cmd.gssb_csp_cmd;
	cmd.addr = addr;
	cmd.timeout_ms = timeout_ms;
	cmd.tx_len = 1;
	cmd.rx_len = 1;
	int ret = gssb_get_csp(csp_node, csp_timeout, &cmd, response);
	return ret;
}


int sunsensor_sample_get_data_csp(uint8_t csp_node, int csp_timeout, uint8_t addr, uint16_t timeout_ms, gssb_csp_cmd_resp_t * response) {
	gssb_csp_cmd_t cmd;
	cmd.gssb_csp_cmd = CMD_SUN_SAMPLE_GET_SUNSENSOR;
	cmd.buff[0] = cmd.gssb_csp_cmd;
	cmd.addr = addr;
	cmd.timeout_ms = timeout_ms;
	cmd.tx_len = 1;
	cmd.rx_len = 8;

	return gssb_get_csp(csp_node, csp_timeout, &cmd, response);
}


int sunsensor_get_temp_csp(uint8_t csp_node, int csp_timeout, uint8_t addr, uint16_t timeout_ms, gssb_csp_cmd_resp_t * response, float *temp) {
	gssb_csp_cmd_t cmd;
	int ret;
	float res_temp;
	cmd.gssb_csp_cmd = CMD_SUN_GET_TEMP;
	cmd.buff[0] = cmd.gssb_csp_cmd;
	cmd.addr = addr;
	cmd.timeout_ms = timeout_ms;
	cmd.tx_len = 1;
	cmd.rx_len = 2;

	ret = gssb_get_csp(csp_node, csp_timeout, &cmd, response);
	res_temp = (float)(((int16_t)((response->buff[0] << 8) + response->buff[1])) >> 2 ) * 0.03125;
	memcpy(temp, &res_temp, 4);
	return ret;
}

int sunsensor_sample_temp_csp(uint8_t csp_node, int csp_timeout, uint8_t addr, uint16_t timeout_ms, gssb_csp_cmd_resp_t * response) {
	gssb_csp_cmd_t cmd;

	cmd.gssb_csp_cmd = CMD_SUN_SAMPLE_TEMP;
	cmd.buff[0] = cmd.gssb_csp_cmd;
	cmd.addr = addr;
	cmd.timeout_ms = timeout_ms;
	cmd.tx_len = 1;
	cmd.rx_len = 0;

	int ret = gssb_get_csp(csp_node, csp_timeout, &cmd, response);
	return ret;
}


int sunsensor_sample_get_temp_csp(uint8_t csp_node, int csp_timeout, uint8_t addr, uint16_t timeout_ms, gssb_csp_cmd_resp_t * response) {
	gssb_csp_cmd_t cmd;
	cmd.gssb_csp_cmd = CMD_SUN_SAMPLE_GET_TEMP;
	cmd.buff[0] = cmd.gssb_csp_cmd;
	cmd.addr = addr;
	cmd.timeout_ms = timeout_ms;
	cmd.tx_len = 1;
	cmd.rx_len = 2;

	int ret = gssb_get_csp(csp_node, csp_timeout, &cmd, response);
	return ret;
}
