#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#include <command/command.h>

#include <io/gssb.h>

#include <util/crc32.h>
#include <util/hexdump.h>
#include <util/console.h>
#include <util/color_printf.h>

#include <csp/csp.h>

#include <util/error.h>

static uint8_t gssb_csp_node = 1;
static uint16_t gssb_csp_timeout = 1000;
static uint8_t i2c_addr = 5;
static uint16_t i2c_timeout_ms = 50;

int cmd_set_gssb_node(struct command_context *ctx) {

	if (ctx->argc == 2) {
		 gssb_node(atoi(ctx->argv[1]), 2000);
	} else if (ctx->argc == 3) {
		gssb_node(atoi(ctx->argv[1]), atoi(ctx->argv[2]));
	} else {	
		return CMD_ERROR_SYNTAX;
	}

	return CMD_ERROR_NONE;
}

int cmd_set_addr_csp(struct command_context *ctx) {

	if (ctx->argc == 2) {
		 gssb_dev(atoi(ctx->argv[1]), 50);
	} else if (ctx->argc == 3) {
		gssb_dev(atoi(ctx->argv[1]), atoi(ctx->argv[2]));
	} else {	
		return CMD_ERROR_SYNTAX;
	}

	return CMD_ERROR_NONE;
}

int cmd_gssb_ident_csp(struct command_context *ctx) {
	gssb_csp_ident_t resp;

	if (ctx->argc != 1)
		return CMD_ERROR_SYNTAX;

	if (gssb_ident(&resp) < 0)
		return CMD_ERROR_FAIL;

	if (resp.i2c_res != E_NO_ERR) {
		printf("GSSB device did not respond\r\n");
		return CMD_ERROR_FAIL;
	}

	printf("Version: \t%s\r\n", (char *) resp.version);
	printf("Device UUID: %"PRIu32"\r\n", resp.uuid);

	return CMD_ERROR_NONE;
}

int cmd_istage_settings_csp(struct command_context *ctx) {

	istage_burn_settings_t settings;
	
	if (ctx->argc == 1) {

		if (gssb_interstage_settings_get(&settings) < 0)
			return CMD_ERROR_FAIL;

		if (settings.status == 2) {
			printf("\r\nDeploy mode:\t\t ARMED AUTO\r\n");
		} else {
			printf("\r\nDeploy mode:\t\t NOT ARMED\r\n");
		}
		printf("Deploy delay from boot:\t %"PRIu16" s\r\n", settings.short_cnt_down);

		printf("\r\n-------- Deploy algorithm config ----------\r\n");
		printf("Knife on time:\t\t %i ms\r\n", settings.std_time_ms);
		printf("Increment:\t\t %i ms\r\n", settings.increment_ms);
		printf("Max repeats:\t\t %i\r\n", settings.max_repeat);
		printf("Repeat time:\t\t %i s\r\n", settings.rep_time_s);
		printf("Switch polarity:\t %"PRIu8"\r\n", settings.switch_polarity);
		printf("Settings locked:\t %"PRIu8"\r\n", settings.locked);

	/* Else set settings */
	} else if (ctx->argc == 8) {
		/* Check settings range */
		int tmp;
		tmp = atoi(ctx->argv[1]);
		if ((tmp > 65535) || (tmp < 0)) {
			color_printf(COLOR_RED, "Knife on time of %d out of range [0 - 65535]\r\n", tmp);
			return CMD_ERROR_SYNTAX;
		} else {
			settings.std_time_ms = (uint16_t)tmp;
		}

		tmp = atoi(ctx->argv[2]);
		if ((tmp > 65535) || (tmp < 0)) {
			color_printf(COLOR_RED, "Increment of %d out of range [0 - 65535]\r\n", tmp);
			return CMD_ERROR_SYNTAX;
		} else {
			settings.increment_ms = (uint16_t)tmp;
		}

		tmp = atoi(ctx->argv[3]);
		if ((tmp > 65535) || (tmp < 0)) {
			color_printf(COLOR_RED, "Auto deploy delay of %d out of range [0 - 65535]\r\n", tmp);
			return CMD_ERROR_SYNTAX;
		} else {
			settings.short_cnt_down = (uint16_t)tmp;
		}

		tmp = atoi(ctx->argv[4]);
		if ((tmp > 255) || (tmp < 0)) {
			color_printf(COLOR_RED, "Max repeats of %d out of range [0 - 255]\r\n", tmp);
			return CMD_ERROR_SYNTAX;
		} else {
			settings.max_repeat = (uint8_t)tmp;
		}

		tmp = atoi(ctx->argv[5]);
		if ((tmp > 255) || (tmp < 0)) {
			color_printf(COLOR_RED, "Time between repeats of %d out of range [0 - 255]\r\n", tmp);
			return CMD_ERROR_SYNTAX;
		} else {
			settings.rep_time_s = (uint8_t)tmp;
		}

		tmp = atoi(ctx->argv[6]);
		if ((tmp > 1) || (tmp < 0)) {
			color_printf(COLOR_RED, "Polarity can only be selected to 0 or 1 not %d\r\n", tmp);
			return CMD_ERROR_SYNTAX;
		} else {
			settings.switch_polarity = (uint8_t)tmp;
		}

		tmp = atoi(ctx->argv[7]);
		if ((tmp > 255) || (tmp < 0)) {
			color_printf(COLOR_RED, "Reboot deploy number of %d out of range [0 - 255]\r\n", tmp);
			return CMD_ERROR_SYNTAX;
		} else {
			settings.reboot_deploy_cnt = (uint8_t)tmp;
		}
		
		if (gssb_interstage_settings_set(&settings) < 0)
			return CMD_ERROR_FAIL;

	} else {
		return CMD_ERROR_SYNTAX;
	}

	return CMD_ERROR_NONE;
}

int cmd_istage_reboot_csp(struct command_context *ctx) {

	if (ctx->argc != 1)
		return CMD_ERROR_SYNTAX;


	if (gssb_reboot() <= 0)
		return CMD_ERROR_FAIL;

	return CMD_ERROR_NONE;
}

int cmd_istage_deploy_csp(struct command_context *ctx) {

	if (ctx->argc != 1)
		return CMD_ERROR_SYNTAX;

	if (gssb_istage_deploy() <= 0)
		return CMD_ERROR_FAIL;

	return CMD_ERROR_NONE;
}

int cmd_istage_arm_csp(struct command_context *ctx) {

	if (ctx->argc != 2)
		return CMD_ERROR_SYNTAX;
	
	if (gssb_istage_arm(atoi(ctx->argv[1])) <= 0)
		return CMD_ERROR_FAIL;

	return CMD_ERROR_NONE;
}

int cmd_istage_state_csp(struct command_context *ctx) {

	if (ctx->argc != 2)
		return CMD_ERROR_SYNTAX;

	if (gssb_istage_set_state(atoi(ctx->argv[1])) <= 0)
		return CMD_ERROR_FAIL;

	return CMD_ERROR_NONE;
}

int cmd_istage_sensors_csp(struct command_context *ctx) {
	gssb_istage_sensors_t sensors;

	if (ctx->argc != 1)
		return CMD_ERROR_SYNTAX;


	if (gssb_istage_sensors(&sensors) <= 0)
		return CMD_ERROR_FAIL;

	if (sensors.i2c_res != E_NO_ERR) {
		printf("GSSB device did not respond\r\n");
		return CMD_ERROR_FAIL;
	}
	
	printf("Panel Temperature: %d\r\n", (int) sensors.panel_temp/10);
	printf("Coarse Sunsensor: %d\r\n", sensors.sun_voltage);

	return CMD_ERROR_NONE;
}

int cmd_istage_status_csp(struct command_context *ctx) {
	gssb_istage_status_t status;
	char *state_str;

	if (ctx->argc != 1)
		return CMD_ERROR_SYNTAX;

	if (gssb_interstage_status(&status) <= 0)
		return CMD_ERROR_FAIL;

	if (status.i2c_res != E_NO_ERR) {
		printf("GSSB device did not respond\r\n");
		return CMD_ERROR_FAIL;
	}

	switch (status.state) {
		case 0:
			state_str = "Not armed";
			break;
		case 1:
			state_str = "Armed for manual deploy";
			break;
		case 2:
			state_str = "Armed for automatical deploy";
			break;
		case 3:
		case 4:
			state_str = "Deploying";
			break;
		default:
			state_str = "Unknown state";
			break;
	};


	printf("Current state:\t\t\t\t %s\r\n", state_str);
	if (status.release_status == 1) 
		color_printf(COLOR_GREEN, "Antenna state:\t\t\t\t %s\r\n", "Released");
	else
		color_printf(COLOR_RED, "Antenna state:\t\t\t\t %s\r\n", "Not released");
	printf("\r\n");

	printf("Delay till deploy:\t\t\t %"PRIu16" s\r\n", status.deploy_in_s);
	printf("Number of attemps since boot:\t\t %"PRIu8"\r\n", status.number_of_deploys);
	printf("Knife that will be use in next deploy:\t %"PRIu8"\r\n", status.active_knife);
	printf("Total deploy attemps:\t\t\t %"PRIu16"\r\n", status.total_number_of_deploys);
	printf("Reboot deploy cnt:\t\t\t %"PRIu8"\r\n", status.reboot_deploy_cnt);


	return CMD_ERROR_NONE;
}


int cmd_sunsensor_read_csp(struct command_context *ctx) {
	gssb_csp_cmd_resp_t resp;
	uint16_t sun[4];
	int i;

	if (ctx->argc != 1)
		return CMD_ERROR_SYNTAX;

	if (sunsensor_sample_get_data_csp(gssb_csp_node, gssb_csp_timeout, i2c_addr, i2c_timeout_ms, &resp) <= 0)
		return CMD_ERROR_FAIL;

	if (resp.return_code != E_NO_ERR) {
		printf("I2C err\r\n");
		return CMD_ERROR_FAIL;
	}

	for (i=0; i <= 7; i += 2) {
		sun[i/2] = (resp.buff[i] << 8) + resp.buff[i+1];
	}

	printf("\r\n ---- Values ----\r\n");
	for (i=0; i < 4; i++) {
		printf("Data %d: %d\r\n", i, sun[i]);
	}

	return CMD_ERROR_NONE;
}

int cmd_sunsensor_temp_csp(struct command_context *ctx) {
	gssb_csp_cmd_resp_t resp;
	float temp;

	if (ctx->argc != 1)
		return CMD_ERROR_SYNTAX;
	
	if (sunsensor_sample_get_temp_csp(gssb_csp_node, gssb_csp_timeout, i2c_addr, i2c_timeout_ms, &resp) <= 0)
		return CMD_ERROR_FAIL;


	if (resp.return_code != E_NO_ERR) {
		printf("I2C err\r\n");
		return CMD_ERROR_FAIL;
	}

	/* if neg temp else */
	if (resp.buff[0] & 0x80) {
		temp = ((~((resp.buff[0] << 8) + resp.buff[1] - 1)) >> 2 ) * -0.03125;
	} else {
		temp = (((resp.buff[0] << 8) + resp.buff[1]) >> 2 ) * 0.03125;
	}

	printf("Temp: %f\r\n", temp);

	return CMD_ERROR_NONE;
}


int cmd_sunsensor_temp1_csp(struct command_context *ctx) {
	gssb_csp_cmd_resp_t resp;
	float temp;

	if (ctx->argc != 1)
		return CMD_ERROR_SYNTAX;

	if (sunsensor_sample_get_temp_csp(gssb_csp_node, gssb_csp_timeout, i2c_addr, i2c_timeout_ms, &resp) <= 0)
		return CMD_ERROR_FAIL;


	if (resp.return_code != E_NO_ERR) {
		printf("I2C err\r\n");
		return CMD_ERROR_FAIL;
	}

	/* if neg temp else */
	if (resp.buff[0] & 0x80) {
		temp = ((~((resp.buff[0] << 8) + resp.buff[1] - 1)) >> 2 ) * -0.03125;
	} else {
		temp = (((resp.buff[0] << 8) + resp.buff[1]) >> 2 ) * 0.03125;
	}

	printf("Temp: %f \r\n", temp);

	return CMD_ERROR_NONE;
}


command_t __sub_command interstage_csp_subcommands[] = {
	{
		.name = "sensors",
		.help = "Read sensors",
		.handler = cmd_istage_sensors_csp,
	},{
		.name = "status",
		.help = "Status",
		.handler = cmd_istage_status_csp,
	},{
		.name = "arm",
		.help = "Interstage arm",
		.handler = cmd_istage_arm_csp,
		.usage = "<1 = arm, 0 = disarm>",
	},{
		.name = "conf",
		.help = "Set or get configuration. To get give no arguments",
		.handler = cmd_istage_settings_csp,
		.usage = "<knife ON [ms]> <increment [ms]> <auto deploy delay [s]> <max repeat [#]> <repeat time [s]> <switch polarity> <deploy boot cnt>",
	},{
		.name = "state",
		.help = "Set current state",
		.handler = cmd_istage_state_csp,
		.usage = "<State 0=NOT ARMED, 1=ARMED MANUAL>",
	},{
		.name = "deploy",
		.help = "Manual trigger deploy",
		.handler = cmd_istage_deploy_csp,
	},{
		.name = "reboot",
		.help = "Soft reboot",
		.handler = cmd_istage_reboot_csp,
	},
};

command_t __sub_command sunsensor_csp_subcommands[] = {
	{
		.name = "read",
		.help = "Read sunsensor values",
		.handler = cmd_sunsensor_read_csp,
	},{
		.name = "temp",
		.help = "Read sunsensor temperature",
		.handler = cmd_sunsensor_temp_csp,
	},{
		.name = "temp1",
		.help = "Read sunsensor temperature1",
		.handler = cmd_sunsensor_temp1_csp,
	},
};
command_t  __sub_command gssb_csp_subcommands[] = {
	{
		.name = "node",
		.help = "CSP node with gssb bus",
		.handler = cmd_set_gssb_node,
		.usage = "<node> [timeout]",
	},{
		.name = "addr",
		.help = "GSSB device address",
		.handler = cmd_set_addr_csp,
		.usage = "<addr> [timeout]",
	},{
		.name = "ident",
		.help = "Get device version and uuid",
		.handler = cmd_gssb_ident_csp,
	},{
		.name = "interstage",
		.help = "Interstage commands",
		.chain = INIT_CHAIN(interstage_csp_subcommands),
	},{
		.name = "sunsensor",
		.help = "Sunsensor commands",
		.chain = INIT_CHAIN(sunsensor_csp_subcommands),
	},


};

command_t __root_command gssb_csp_commands[] = {
	{
		.name = "gssbcsp",
		.help = "client: GSSB",
		.chain = INIT_CHAIN(gssb_csp_subcommands),
	}
};

void cmd_gssb_devices_setup_csp(void) {
	command_register(gssb_csp_commands);
}
