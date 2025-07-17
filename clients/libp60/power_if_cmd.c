
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

#include <csp/csp.h>
#include <util/console.h>

#include <power_if.h>

static uint8_t power_if_port = 10;
static uint32_t power_if_timeout = 5000;

int cmd_power_if_port(struct command_context * ctx) {
	if (ctx->argc < 2) {
		printf("Current port is %d\n", power_if_port);
		return CMD_ERROR_NONE;
	}
	power_if_port = atoi(ctx->argv[1]);
	return CMD_ERROR_NONE;
}

int cmd_power_if_timeout(struct command_context *ctx) {
	if (ctx->argc < 2) {
		printf("Current timeout is %"PRIu32"\n", power_if_timeout);
		return CMD_ERROR_NONE;
	}
	if (sscanf(command_args(ctx), "%"SCNu32, &power_if_timeout) != 1)
		return CMD_ERROR_SYNTAX;
	if (power_if_timeout > 30000) {
		printf("Timeout set to high, limited to 30000 ms\n");
		power_if_timeout = 30000;
	}
	printf("Timeout set to %"PRIu32"\n", power_if_timeout);
	return CMD_ERROR_NONE;
}

int cmd_power_if_set_get(struct command_context * ctx) {
	power_if_ch_status_t ch_status;

	if (ctx->argc < 3) {
		return CMD_ERROR_SYNTAX;
	}
	uint8_t node = atoi(ctx->argv[1]);
	memset(&ch_status, 0, sizeof(ch_status));
	strncpy(ch_status.name, ctx->argv[2], 7);
	ch_status.name[7] = 0;
	char * cmd = ctx->argv[0];
	if (!strcmp(cmd, "status") && (ctx->argc == 3)) {
		if (!power_if_cmd(node, power_if_port, power_if_timeout, POWER_IF_GET, &ch_status)) {
			return CMD_ERROR_FAIL;
		}
	} else {
		if (!strcmp(cmd, "on")) {
			ch_status.mode = 1;
		} else if (!strcmp(cmd, "off")) {
			ch_status.mode = 0;
		}
		ch_status.on_cnt = (ctx->argc > 3) ? atoi(ctx->argv[3]) : 0;
		ch_status.off_cnt = (ctx->argc > 4) ? atoi(ctx->argv[4]) : 0;
		if (!power_if_cmd(node, power_if_port, power_if_timeout, POWER_IF_SET, &ch_status)) {
			return CMD_ERROR_FAIL;
		}
	}
	printf("Node %u, Output channel '%s' (%u) is %s\r\n", node, ch_status.name, ch_status.ch_idx, (ch_status.mode ? "ON": "OFF"));
	printf("  ch_idx:     %u\r\n", ch_status.ch_idx);
	printf("  mode:       %u\r\n", ch_status.mode);
	printf("  on_cnt:     %u\r\n", ch_status.on_cnt);
	printf("  off_cnt:    %u\r\n", ch_status.off_cnt);
	printf("  cur_lu_lim: %u\r\n", ch_status.cur_lu_lim);
	printf("  cur_lim:    %u\r\n", ch_status.cur_lim);
	printf("  voltage:    %u\r\n", ch_status.voltage);
	printf("  current:    %d\r\n", ch_status.current);
	printf("  latchup:    %u\r\n", ch_status.latchup);

	return CMD_ERROR_NONE;
}

int cmd_power_if_list(struct command_context * ctx) {
	if (ctx->argc < 2) {
		return CMD_ERROR_SYNTAX;
	}
	uint8_t node = atoi(ctx->argv[1]);
	power_if_cmd_list_response_t ch_list;
	if (!power_if_cmd(node, power_if_port, power_if_timeout, POWER_IF_LIST, &ch_list)) {
		return CMD_ERROR_FAIL;
	}
	printf("ch name     status\r\n");
	for (uint8_t ch = 0; ch < ch_list.count; ch++) {
		printf("%2u %-8s %s\r\n", ch_list.list[ch].ch_idx, ch_list.list[ch].name, ch_list.list[ch].mode ? "ON": "OFF");
	}
	return CMD_ERROR_NONE;
}

command_t power_if_commands[] = {
	{
		.name = "port",
		.help = "Set power interface port (default is 10)",
		.usage = "<port>",
		.handler = cmd_power_if_port,
	},
	{
		.name = "timeout",
		.help = "Set power interface timeout in milliseconds",
		.usage = "<timeout>",
		.handler = cmd_power_if_timeout,
	},
	{
		.name = "status",
		.help = "Get power channel status",
		.usage = "<node> <channel>",
		.handler = cmd_power_if_set_get,
	},
	{
		.name = "on",
		.help = "Turn power channel on",
		.usage = "<node> <channel> [<on_cnt> <off_cnt>]",
		.handler = cmd_power_if_set_get,
	},
	{
		.name = "off",
		.help = "Turn power channel off",
		.usage = "<node> <channel> off [<on_cnt> <off_cnt>]",
		.handler = cmd_power_if_set_get,
	},
	{
		.name = "list",
		.help = "Get list power channels",
		.usage = "<node>",
		.handler = cmd_power_if_list,
	},
};

command_t __root_command power_if_root_command[] = {
	{
		.name = "power",
		.help = "Power interface commands",
		.chain = INIT_CHAIN(power_if_commands),
	}
};

void cmd_power_if_setup(void) {
	command_register(power_if_root_command);
}
