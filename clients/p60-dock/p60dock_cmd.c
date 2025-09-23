
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

#include <csp/csp.h>
#include <util/console.h>

#include <p60dock.h>

int p60dock_node = 4;
static uint32_t p60dock_timeout = 5000;

static uint8_t hk_buf[512] = {};

int cmd_p60dock_node(struct command_context * ctx) {
	if (ctx->argc < 2) {
		printf("Current node is %d\n", p60dock_node);
		return CMD_ERROR_NONE;
	}
	p60dock_node = atoi(ctx->argv[1]);
	return CMD_ERROR_NONE;
}

int cmd_p60dock_timeout(struct command_context *ctx) {
	if (ctx->argc < 2) {
		printf("Current timeout is %"PRIu32"\n", p60dock_timeout);
		return CMD_ERROR_NONE;
	}
	if (sscanf(command_args(ctx), "%"SCNu32, &p60dock_timeout) != 1)
		return CMD_ERROR_SYNTAX;
	printf("Timeout set to %"PRIu32"\n", p60dock_timeout);
	return CMD_ERROR_NONE;
}

int cmd_p60dock_hk(struct command_context * ctx) {
	param_index_t node_hk = {0};
	memset(hk_buf, 0, 512);
	node_hk.physaddr = hk_buf;
	if (!p60dock_get_hk(&node_hk, p60dock_node, p60dock_timeout))
		return CMD_ERROR_FAIL;
	param_list(&node_hk, 1);
	return CMD_ERROR_NONE;
}

int cmd_p60dock_gndwdt_clear(struct command_context * ctx) {
	if (!p60dock_gndwdt_clear(p60dock_node, p60dock_timeout))
		return CMD_ERROR_FAIL;
	return CMD_ERROR_NONE;
}

command_t p60dock_commands[] = {
	{
		.name = "node",
		.help = "Set node",
		.usage = "<node>",
		.handler = cmd_p60dock_node,
	},
	{
		.name = "timeout",
		.help = "Set timeout in milliseconds",
		.usage = "<timeout>",
		.handler = cmd_p60dock_timeout,
	},
	{
		.name = "hk",
		.help = "Get HK",
		.handler = cmd_p60dock_hk,
	},
	{
		.name = "gndwdt_clear",
		.help = "Clear GND WDT",
		.handler = cmd_p60dock_gndwdt_clear,
    }
};

command_t __root_command p60dock_root_command[] = {
	{
		.name = "p60dock",
		.help = "client: P60 Dock",
		.chain = INIT_CHAIN(p60dock_commands),
	},
};

void cmd_p60dock_setup(void) {
	command_register(p60dock_root_command);
}
