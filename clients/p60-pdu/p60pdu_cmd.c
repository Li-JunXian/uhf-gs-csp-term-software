
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

#include <csp/csp.h>
#include <util/console.h>

#include <p60pdu.h>

int p60pdu_node = 3;
static uint32_t p60pdu_timeout = 5000;

static uint8_t hk_buf[512] = {};

int cmd_p60pdu_node(struct command_context * ctx) {
	if (ctx->argc != 2)
		return CMD_ERROR_SYNTAX;
	p60pdu_node = atoi(ctx->argv[1]);
	return CMD_ERROR_NONE;
}

int cmd_p60pdu_timeout(struct command_context *ctx) {
	if (ctx->argc < 2) {
		printf("Current timeout is %"PRIu32"\n", p60pdu_timeout);
		return CMD_ERROR_NONE;
	}
	if (sscanf(command_args(ctx), "%"SCNu32, &p60pdu_timeout) != 1)
		return CMD_ERROR_SYNTAX;
	printf("Timeout set to %"PRIu32"\n", p60pdu_timeout);
	return CMD_ERROR_NONE;
}

int cmd_p60pdu_hk(struct command_context * ctx) {
	param_index_t node_hk = {0};
	memset(hk_buf, 0, 512);
	node_hk.physaddr = hk_buf;
	if (!p60pdu_get_hk(&node_hk, p60pdu_node, p60pdu_timeout))
		return CMD_ERROR_FAIL;
	param_list(&node_hk, 1);
	return CMD_ERROR_NONE;
}

int cmd_p60pdu_gndwdt_clear(struct command_context * ctx) {
	if (!p60pdu_gndwdt_clear(p60pdu_node, p60pdu_timeout))
		return CMD_ERROR_FAIL;
	return CMD_ERROR_NONE;
}

command_t p60pdu_commands[] = {
	{
		.name = "node",
		.help = "Set node",
		.usage = "<node>",
		.handler = cmd_p60pdu_node,
	},
	{
		.name = "timeout",
		.help = "Set timeout in milliseconds",
		.usage = "<timeout>",
		.handler = cmd_p60pdu_timeout,
	},
	{
		.name = "hk",
		.help = "Get HK",
		.handler = cmd_p60pdu_hk,
	},
	{
		.name = "gndwdt_clear",
		.help = "Clear GND WDT",
		.handler = cmd_p60pdu_gndwdt_clear,
    },
};

command_t __root_command p60pdu_root_command[] = {
	{
		.name = "p60pdu",
		.help = "client: P60 PDU",
		.chain = INIT_CHAIN(p60pdu_commands),
	},
};

void cmd_p60pdu_setup(void) {
	command_register(p60pdu_root_command);
}
