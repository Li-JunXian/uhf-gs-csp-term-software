
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

#include <csp/csp.h>
#include <util/console.h>

#include <p60acu.h>

static int p60acu_node = 2;
static uint32_t p60acu_timeout = 5000;

static uint8_t hk_buf[512] = {};

int cmd_p60acu_node(struct command_context * ctx) {
	if (ctx->argc != 2) {
		printf("Current node is %u\n", p60acu_node);
		return CMD_ERROR_NONE;
	}
	p60acu_node = atoi(ctx->argv[1]);
	return CMD_ERROR_NONE;
}

int cmd_p60acu_timeout(struct command_context *ctx) {
	if (ctx->argc < 2) {
		printf("Current timeout is %"PRIu32"\n", p60acu_timeout);
		return CMD_ERROR_NONE;
	}
	if (sscanf(command_args(ctx), "%"SCNu32, &p60acu_timeout) != 1)
		return CMD_ERROR_SYNTAX;
	printf("Timeout set to %"PRIu32"\n", p60acu_timeout);
	return CMD_ERROR_NONE;
}

int cmd_p60acu_hk(struct command_context * ctx) {
	param_index_t node_hk = {0};
	memset(hk_buf, 0, 512);
	node_hk.physaddr = hk_buf;
	if (!p60acu_get_hk(&node_hk, p60acu_node, p60acu_timeout))
		return CMD_ERROR_FAIL;
	param_list(&node_hk, 1);
	return CMD_ERROR_NONE;
}

int cmd_p60acu_gndwdt_clear(struct command_context * ctx) {
	if (!p60acu_gndwdt_clear(p60acu_node, p60acu_timeout))
		return CMD_ERROR_FAIL;
	return CMD_ERROR_NONE;
}

command_t p60acu_commands[] = {
	{
		.name = "node",
		.help = "Set P60 ACU node address",
		.usage = "<node>",
		.handler = cmd_p60acu_node,
	},
	{
		.name = "timeout",
		.help = "Set timeout in milliseconds",
		.usage = "<timeout>",
		.handler = cmd_p60acu_timeout,
	},
	{
		.name = "hk",
		.help = "Retrieve and list all housekeeping parameters",
		.handler = cmd_p60acu_hk,
	},
	{
		.name = "gndwdt_clear",
		.help = "Clear GND WDT",
		.handler = cmd_p60acu_gndwdt_clear,
    },
};

command_t __root_command p60_root_command[] = {
	{
		.name = "p60acu",
		.help = "client: P60 ACU",
		.chain = INIT_CHAIN(p60acu_commands),
	},
};

void cmd_p60acu_setup(void) {
	command_register(p60_root_command);
}
