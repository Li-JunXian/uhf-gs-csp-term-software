#include <math.h>
#include <inttypes.h>
#include <io/bpx.h>
#include <util/console.h>
#include <util/log.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

bpx_config_t bpx_config = {0};

int cmd_bpx_counters_reset(struct command_context *ctx) {
	bpx_counters_reset();
	return CMD_ERROR_NONE;
}


int cmd_bpx_node_set(struct command_context *ctx) {
	if (ctx->argc != 2)
		return CMD_ERROR_SYNTAX;
	uint8_t node = atoi(ctx->argv[1]);
	bpx_node_set(node);
	return CMD_ERROR_NONE;
}

int cmd_bpx_hk_get(struct command_context *ctx) {
	bpx_hk_t hk;
	if (bpx_hk_get(&hk) < 0)
		return CMD_ERROR_FAIL;
	printf("Charge current        %"PRIu16" mA\r\n", hk.cur_charge);
	printf("Discharge current     %"PRIu16" mA\r\n", hk.cur_discharge);
	printf("Heater current        %"PRIu16" mA\r\n", hk.cur_heater);
	printf("Battery voltage       %"PRIu16" mV\r\n", hk.vbatt);
	printf("Battery temperature 1 %"PRId16" degC\r\n", hk.bat_temperature1);
	printf("Battery temperature 2 %"PRId16" degC\r\n", hk.bat_temperature2);
	printf("Battery temperature 3 %"PRId16" degC\r\n", hk.bat_temperature3);
	printf("Battery temperature 4 %"PRId16" degC\r\n", hk.bat_temperature4);
	printf("Bootcause %"PRIu8" \r\n", hk.bootcause);
	printf("Bootcount %"PRIu32" \r\n", hk.counter_boot);
	return CMD_ERROR_NONE;
}

int cmd_bpx_config_get(struct command_context *ctx) {
  bpx_config_get(&bpx_config);
  bpx_config_print(&bpx_config);
  return CMD_ERROR_NONE;
}
int cmd_bpx_config_print(struct command_context *ctx) {
	bpx_config_print(&bpx_config);
	return CMD_ERROR_NONE;
}
int cmd_bpx_config_set(struct command_context *ctx) {
	bpx_config_set(&bpx_config);
	return CMD_ERROR_NONE;
}
int cmd_bpx_config_restore(struct command_context *ctx) {
	bpx_config_cmd(1);
	return CMD_ERROR_NONE;
}
int cmd_bpx_config_edit(struct command_context *ctx) {

	int scan;
	int ret __attribute__((unused));

	printf("Edit BPX configuration\r\n");
	printf("Press <.> to skip\r\n");

	printf("Battheater mode: %u > ", bpx_config.battheater_mode);
	fflush(stdout);
	if ((ret = scanf("%u%*[^\r]", &scan)) == 1)
		bpx_config.battheater_mode = scan;
	printf(" [%u] > %u\r\n", ret, bpx_config.battheater_mode);
	ret = scanf("%*c");

	printf("Battheater LOW: %d > ", bpx_config.battheater_low);
	fflush(stdout);
	if ((ret = scanf("%d%*[^\r]", &scan)) == 1)
		bpx_config.battheater_low = scan;
	printf(" [%d] > %d\r\n", ret, bpx_config.battheater_low);
	ret = scanf("%*c");

	/* PPT mode */
	printf("Battheater HIGH: %d > ", bpx_config.battheater_high);
	fflush(stdout);
	if ((ret = scanf("%d%*[^\r]", &scan)) == 1)
		bpx_config.battheater_high = scan;
	printf(" [%d] > %d\r\n", ret, bpx_config.battheater_high);
	ret = scanf("%*c");

	return CMD_ERROR_NONE;

}



command_t __sub_command bpx_config_subcommands[] = {
	{
		.name = "get",
		.help = "Conf get",
		.handler = cmd_bpx_config_get,
	},{
		.name = "set",
		.help = "Conf set",
		.handler = cmd_bpx_config_set,
	},{
		.name = "edit",
		.help = "Edit local config",
		.handler = cmd_bpx_config_edit,
	},{
		.name = "print",
		.help = "Print local config",
		.handler = cmd_bpx_config_print,
	},{
		.name = "restore",
		.help = "Restore config from default",
		.handler = cmd_bpx_config_restore,
	}
};


command_t __sub_command bpx_subcommands[] = {
    {
		.name = "resetcount",
		.help = "Reset all counters to zero",
		.handler = cmd_bpx_counters_reset,
	},{
		.name = "conf",
		.help = "Configuration",
		.chain = INIT_CHAIN(bpx_config_subcommands),
	},{
		.name = "hk",
		.help = "Get HK values",
		.handler = cmd_bpx_hk_get,
	},{
		.name = "node",
		.help = "Set client BPX csp/i2c add",
		.usage = "<add>",
		.handler = cmd_bpx_node_set,
	}
};

command_t __root_command bpx_commands[] = {
	{
		.name = "bpx",
		.help = "client: BPX",
		.chain = INIT_CHAIN(bpx_subcommands),
	}
};

void cmd_bpx_setup(void) {
	command_register(bpx_commands);
}
