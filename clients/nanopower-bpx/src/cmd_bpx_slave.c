#include <inttypes.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <io/bpx.h>
#include <csp/csp.h>
#include <csp/csp_endian.h>

#include <dev/i2c.h>
#include <util/error.h>
#include <util/console.h>
#include <util/log.h>


static int slave_node = NODE_BPX;
// Defined in cmd_bpx
extern bpx_config_t bpx_config;
extern int cmd_bpx_config_edit(struct command_context *ctx);


int bpx_slave_node(struct command_context *ctx) {

	char * args = command_args(ctx);
	unsigned int node;
	if (sscanf(args, "%u", &node) != 1)
		return CMD_ERROR_SYNTAX;
	printf("Setting BPX slave node address to %u\r\n", node);
	slave_node = node;

	return CMD_ERROR_NONE;
}

int bpx_slave_ping(struct command_context *ctx) {

	i2c_frame_t * frame;
	frame = csp_buffer_get(I2C_MTU);
	frame->dest = slave_node;
	frame->data[0] = CSP_PING; // Ping port
	frame->data[1] = 0x55;
	frame->len = 2;
	frame->len_rx = 3;
	frame->retries = 0;

	if (i2c_send(0, frame, 0) != E_NO_ERR) {
		csp_buffer_free(frame);
		return CMD_ERROR_NOMEM;
	}

	if (i2c_receive(0, &frame, 200) == E_NO_ERR) {
		printf("Received a reply!\r\n");
	} else {
		printf("No reply\r\n");
	}

	csp_buffer_free(frame);
	return CMD_ERROR_NONE;

}

int bpx_slave_reboot(struct command_context *ctx) {

	i2c_frame_t * frame;
	frame = csp_buffer_get(I2C_MTU);
	frame->dest = slave_node;
	frame->data[0] = CSP_REBOOT; // Ping port
	frame->data[1] = 0x80;
	frame->data[2] = 0x07;
	frame->data[3] = 0x80;
	frame->data[4] = 0x07;
	frame->len = 5;
	frame->len_rx = 0;
	frame->retries = 0;

	if (i2c_send(0, frame, 0) != E_NO_ERR) {
		csp_buffer_free(frame);
		return CMD_ERROR_NOMEM;
	}

	printf("Sent reboot command\r\n");
	return CMD_ERROR_NONE;
}

int bpx_slave_hk(struct command_context *ctx) {

	printf("Requesting BPX hk data\r\n");
	bpx_hk_t * hk;

	i2c_frame_t * frame;
	frame = csp_buffer_get(I2C_MTU);
	frame->dest = slave_node;
	frame->data[0] = BPX_PORT_HK; // Ping port
	frame->len = 1;
	frame->len_rx = 2 + (uint8_t) sizeof(bpx_hk_t);
	frame->retries = 0;

	if (i2c_send(0, frame, 0) != E_NO_ERR) {
		csp_buffer_free(frame);
		return CMD_ERROR_NOMEM;
	}

	if (i2c_receive(0, &frame, 200) != E_NO_ERR)
		return CMD_ERROR_FAIL;

	hk = (bpx_hk_t *)&frame->data[2];

	hk->cur_charge = csp_ntoh16(hk->cur_charge);
	hk->cur_discharge = csp_ntoh16(hk->cur_discharge);
	hk->cur_heater = csp_ntoh16(hk->cur_heater);
	hk->vbatt = csp_ntoh16(hk->vbatt);
	hk->bat_temperature1 = csp_ntoh16(hk->bat_temperature1);
	hk->bat_temperature2 = csp_ntoh16(hk->bat_temperature2);
	hk->bat_temperature3 = csp_ntoh16(hk->bat_temperature3);
	hk->bat_temperature4 = csp_ntoh16(hk->bat_temperature4);
	hk->counter_boot = csp_ntoh32(hk->counter_boot);

	printf("Charge current        %"PRIu16" mA\r\n", hk->cur_charge);
	printf("Discharge current     %"PRIu16" mA\r\n", hk->cur_discharge);
	printf("Heater current        %"PRIu16" mA\r\n", hk->cur_heater);
	printf("Battery voltage       %"PRIu16" mV\r\n", hk->vbatt);
	printf("Battery temperature 1 %"PRId16" degC\r\n", hk->bat_temperature1);
	printf("Battery temperature 2 %"PRId16" degC\r\n", hk->bat_temperature2);
	printf("Battery temperature 3 %"PRId16" degC\r\n", hk->bat_temperature3);
	printf("Battery temperature 4 %"PRId16" degC\r\n", hk->bat_temperature4);
	printf("Bootcause %"PRIu8" \r\n", hk->bootcause);
	printf("Bootcount %"PRIu32" \r\n", hk->counter_boot);

	csp_buffer_free(frame);
	return CMD_ERROR_NONE;

}

/***************************************************************/

int cmd_bpx_slave_config_get(struct command_context *ctx) {
	printf("Requesting BPX Config\r\n");

	i2c_frame_t * frame;
	frame = csp_buffer_get(I2C_MTU);
	frame->dest = slave_node;
	frame->data[0] = BPX_PORT_CONFIG_GET;
	frame->len = 1;
	frame->len_rx = 2 + (uint8_t) sizeof(bpx_config_t);
	frame->retries = 0;

	if (i2c_send(0, frame, 0) != E_NO_ERR) {
		csp_buffer_free(frame);
		return CMD_ERROR_NOMEM;
	}

	if (i2c_receive(0, &frame, 20) != E_NO_ERR)
		return CMD_ERROR_FAIL;

	memcpy(&bpx_config, &frame->data[2], sizeof(bpx_config_t));
	bpx_config_print(&bpx_config);
	return CMD_ERROR_NONE;
}


int cmd_bpx_slave_config_print(struct command_context *ctx) {
	bpx_config_print(&bpx_config);
	return CMD_ERROR_NONE;
}

int cmd_bpx_slave_config_set(struct command_context *ctx) {
	printf("Setting BPX Config\r\n");

	i2c_frame_t * frame;
	frame = csp_buffer_get(I2C_MTU);
	frame->dest = slave_node;
	frame->data[0] = BPX_PORT_CONFIG_SET;
	frame->len = 1 + (uint8_t) sizeof(bpx_config_t);
	frame->len_rx = 2;
	frame->retries = 0;

	memcpy(&frame->data[1], &bpx_config, sizeof(bpx_config_t));
	if (i2c_send(0, frame, 0) != E_NO_ERR) {
		csp_buffer_free(frame);
		return CMD_ERROR_NOMEM;
	}

	if (i2c_receive(0, &frame, 200) != E_NO_ERR)
		return CMD_ERROR_FAIL;

	return CMD_ERROR_NONE;
}

int cmd_bpx_slave_config_restore(struct command_context *ctx) {

	i2c_frame_t * frame;
	frame = csp_buffer_get(I2C_MTU);
	frame->dest = slave_node;
	frame->data[0] = BPX_PORT_CONFIG_CMD;
	frame->data[1] = 1;
	frame->len = 2;
	frame->len_rx = 0;
	frame->retries = 0;

	if (i2c_send(0, frame, 0) != E_NO_ERR) {
		csp_buffer_free(frame);
		return CMD_ERROR_NOMEM;
	}

	return CMD_ERROR_NONE;
}

int cmd_bpx_slave_config_edit(struct command_context *ctx) {

	cmd_bpx_config_edit(NULL);
	return CMD_ERROR_NONE;

}



/*********************************************************/

int bpx_slave_reset_persistent(struct command_context *ctx) {

	uint8_t magic = 0x42;

	i2c_frame_t * frame;
	frame = csp_buffer_get(I2C_MTU);
	frame->dest = slave_node;
	frame->data[0] = BPX_PORT_RESET_COUNTERS;
	frame->data[1] = (uint8_t) magic;
	frame->len = 2;
	frame->len_rx = 0;
	frame->retries = 0;

	if (i2c_send(0, frame, 0) != E_NO_ERR) {
		csp_buffer_free(frame);
		return CMD_ERROR_NOMEM;
	}

	return CMD_ERROR_NONE;
}

command_t __sub_command bpx_slave_config_subcommands[] = {
	{
		.name = "get",
		.help = "Conf get",
		.handler = cmd_bpx_slave_config_get,
	},{
		.name = "set",
		.help = "Conf set",
		.handler = cmd_bpx_slave_config_set,
	},{
		.name = "edit",
		.help = "Edit local config",
		.handler = cmd_bpx_slave_config_edit,
	},{
		.name = "print",
		.help = "Print local config",
		.handler = cmd_bpx_slave_config_print,
	},{
		.name = "restore",
		.help = "Restore config from default",
		.handler = cmd_bpx_slave_config_restore,
	}
};





command_t __sub_command bpx_slavecommands[] = {
	{
		.name = "node",
		.help = "Set slave BPX address in host table",
		.handler = bpx_slave_node,
	},{
		.name = "hk",
		.help = "Get slave HK",
		.handler = bpx_slave_hk,
	},{
		.name = "ping",
		.help = "Slave ping",
		.handler = bpx_slave_ping,
	},{
		.name = "reboot",
		.help = "Slave reboot",
		.handler = bpx_slave_reboot,
	},{
		.name = "resetcount",
		.help = "Set counters = 0",
		.handler = bpx_slave_reset_persistent,
	},{
		.name = "conf",
		.help = "Configuration",
		.chain = INIT_CHAIN(bpx_slave_config_subcommands),
	}
};

command_t __root_command bpxslave_commands[] = {
	{
		.name = "bpxslave",
		.help = "client: BPX slave",
		.chain = INIT_CHAIN(bpx_slavecommands),
	}
};

void cmd_bpxslave_setup(void) {
	command_register(bpxslave_commands);
}
