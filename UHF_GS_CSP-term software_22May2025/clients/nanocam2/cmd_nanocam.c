/**
 * NanoCam Client library
 *
 * Copyright 2015 GomSpace ApS. All rights reserved.
 */

#include <nanocam.h>

#include <inttypes.h>
#include <string.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <util/console.h>
#include <util/log.h>
#include <util/color_printf.h>
#include <gosh/getopt.h>

#include <csp/csp.h>
#include <csp/csp_endian.h>

/** Width of histogram */
#define HIST_WIDTH	50

/** Default timeout in ms */
static uint32_t cam_timeout = 5000;

/* Default store format is JPEG */
static uint8_t cam_format = NANOCAM_STORE_JPG;

static char *format_to_string(uint8_t format)
{
	switch (format) {
	case NANOCAM_STORE_RAW:
		return "RAW";
	case NANOCAM_STORE_BMP:
		return "BMP";
	case NANOCAM_STORE_JPG:
		return "JPG";
	case NANOCAM_STORE_DNG:
		return "DNG";
	case NANOCAM_STORE_RAW10:
		return "RAW10";
	default:
		return "unknown";
	}
}

static uint8_t string_to_format(const char *format)
{
	if (!strcasecmp(format, "raw")) {
		return NANOCAM_STORE_RAW;
	} else if (!strcasecmp(format, "bmp")) {
		return NANOCAM_STORE_BMP;
	} else if (!strcasecmp(format, "jpg")) {
		return NANOCAM_STORE_JPG;
	} else if (!strcasecmp(format, "dng")) {
		return NANOCAM_STORE_DNG;
	} else if (!strcasecmp(format, "raw10")) {
		return NANOCAM_STORE_RAW10;
	} else {
		return atoi(format);
	}
}

int cmd_cam_node(struct command_context *ctx)
{
	if (ctx->argc != 2)
		return CMD_ERROR_SYNTAX;

	nanocam_set_node(atoi(ctx->argv[1]));

	return CMD_ERROR_NONE;
}

int cmd_cam_set_timeout(struct command_context *ctx)
{
	if (ctx->argc < 2) {
		printf("Current timeout is %"PRIu32"\n", cam_timeout);
		return CMD_ERROR_NONE;
	}

	if (sscanf(command_args(ctx), "%"SCNu32, &cam_timeout) != 1)
		return CMD_ERROR_SYNTAX;

	printf("Timeout set to %"PRIu32"\n", cam_timeout);

	return CMD_ERROR_NONE;
}

int cmd_cam_format(struct command_context *ctx)
{
	if (ctx->argc < 2) {
		printf("Current format is %s (%hhu)\n",
		       format_to_string(cam_format), cam_format);
		return CMD_ERROR_NONE;
	}

	cam_format = string_to_format(ctx->argv[1]);

	printf("Format set to %s (%hhu)\n",
	       format_to_string(cam_format), cam_format);

	return CMD_ERROR_NONE;
}

static color_printf_t hist_bin_color(bool red, bool green, bool blue)
{
	if (red && green && blue)
		return COLOR_WHITE;

	if (red && green)
		return COLOR_YELLOW;
	if (red && blue)
		return COLOR_MAGENTA;
	if (green && blue)
		return COLOR_CYAN;

	if (red)
		return COLOR_RED;
	if (green)
		return COLOR_GREEN;
	if (blue)
		return COLOR_BLUE;

	return COLOR_NONE;
}

int cmd_cam_snap(struct command_context *ctx)
{
	/* Argument parsing */
	char *colors = "rgb";
	int c, remain, index, result;
	unsigned int i, j;
	bool histogram = true;

	cam_snap_t snap;
	cam_snap_reply_t reply;

	snap.count = 1;
	snap.width = 0;
	snap.height = 0;
	snap.delay = 0;
	snap.flags = 0;
	snap.format = NANOCAM_STORE_JPG;

	/* Histogram print */
	float pct;
	unsigned int r_blocks, g_blocks, b_blocks, bin_max = 0;
	bool r, g, b, show_r, show_g, show_b;

	while ((c = gosh_getopt(ctx, "ad:h:in:rstx")) != -1) {
		switch (c) {
		case 'a':
			snap.flags |= NANOCAM_SNAP_FLAG_AUTO_GAIN;
			break;
		case 'd':
			snap.delay = atoi(ctx->optarg);
			break;
		case 'h':
			colors = ctx->optarg;
			break;
		case 'i':
			snap.flags |= NANOCAM_SNAP_FLAG_STORE_TAGS;
			break;
		case 'n':
			snap.count = atoi(ctx->optarg);
			break;
		case 'r':
			snap.flags |= NANOCAM_SNAP_FLAG_STORE_RAM;
			break;
		case 's':
			snap.flags |= NANOCAM_SNAP_FLAG_STORE_FLASH;
			break;
		case 't':
			snap.flags |= NANOCAM_SNAP_FLAG_STORE_THUMB;
			break;
		case 'x':
			histogram = false;
			snap.flags |= NANOCAM_SNAP_FLAG_NOHIST;
			break;
		case '?':
			return CMD_ERROR_SYNTAX;
		}
	}

	remain = ctx->argc - ctx->optind;
	index = ctx->optind;

	if (remain > 0)
		snap.width = atoi(ctx->argv[index]);
	if (remain > 1)
		snap.height = atoi(ctx->argv[index + 1]);

	if (snap.width || snap.height) {
		printf("Snapping %hhux%hhu image, timeout %"PRIu32"\n",
		       snap.width, snap.height, cam_timeout);
	} else {
		printf("Snapping full-size image, timeout %"PRIu32"\n", cam_timeout);
	}

	result = nanocam_snap(&snap, &reply, cam_timeout);
	if (result < 0) {
		printf("Snap error: %d\n", result);
		return CMD_ERROR_FAIL;
	}

	printf("Snap result: %d\n", result);

	if (!histogram)
		return CMD_ERROR_NONE;

	/* Brightness */
	color_printf(COLOR_WHITE, "Brightness\n");
	color_printf(COLOR_NONE,  "  All   : %u%% (min/max/peak/avg %03hhu/%03hhu/%03hhu/%03hhu)\n",
		     reply.light_avg[0] * 100 / UINT8_MAX,
		     reply.light_min[0], reply.light_max[0],
		     reply.light_peak[0], reply.light_avg[0]);
	color_printf(COLOR_RED,   "  Red   : %u%% (min/max/peak/avg %03hhu/%03hhu/%03hhu/%03hhu)\n",
		     reply.light_avg[1] * 100 / UINT8_MAX,
		     reply.light_min[1], reply.light_max[1],
		     reply.light_peak[1], reply.light_avg[1]);
	color_printf(COLOR_GREEN, "  Green : %u%% (min/max/peak/avg %03hhu/%03hhu/%03hhu/%03hhu)\n",
		     reply.light_avg[2] * 100 / UINT8_MAX,
		     reply.light_min[2], reply.light_max[2],
		     reply.light_peak[2], reply.light_avg[2]);
	color_printf(COLOR_BLUE,  "  Blue  : %u%% (min/max/peak/avg %03hhu/%03hhu/%03hhu/%03hhu)\n",
		     reply.light_avg[3] * 100/ UINT8_MAX,
		     reply.light_min[3], reply.light_max[3],
		     reply.light_peak[3], reply.light_avg[3]);

	/* Histogram */
	color_printf(COLOR_WHITE, "Histogram\n");

	show_r = strchr(colors, 'r');
	show_g = strchr(colors, 'g');
	show_b = strchr(colors, 'b');

	/* Find max bin count */
	for (i = 0; i < NANOCAM_SNAP_HIST_BINS; i++) {
		for (j = 0; j < NANOCAM_SNAP_COLORS; j++) {
			if (reply.hist[j][i] > bin_max)
				bin_max = reply.hist[j][i];
		}
	}

	color_printf(COLOR_WHITE, "    Min +");
	for (j = 0; j < HIST_WIDTH + 1; j++)
		color_printf(COLOR_WHITE, "-");
	color_printf(COLOR_WHITE, "+\n");

	for (i = 0; i < NANOCAM_SNAP_HIST_BINS; i++) {
		/* Overall brightness */
		pct = (float)reply.hist[0][i] * 100 / UINT8_MAX;
		printf(" %5.1f%% |", pct);

		/* Per-color brightness (round-up and scale to max bin count) */
		r_blocks = (reply.hist[1][i] * HIST_WIDTH + bin_max - 1) / bin_max;
		g_blocks = (reply.hist[2][i] * HIST_WIDTH + bin_max - 1) / bin_max;
		b_blocks = (reply.hist[3][i] * HIST_WIDTH + bin_max - 1) / bin_max;

		/* Show bar */
		for (j = 0; j < HIST_WIDTH; j++) {
			r = (j < r_blocks) && show_r;
			g = (j < g_blocks) && show_g;
			b = (j < b_blocks) && show_b;
			if (r || g || b)
				color_printf(hist_bin_color(r, g, b), "#");
			else
				printf(" ");
		}

		printf(" | %03u-%03u\n",
		       i * 256 / NANOCAM_SNAP_HIST_BINS,
		       i * 256 / NANOCAM_SNAP_HIST_BINS + 15);
	}

	color_printf(COLOR_WHITE, "    Max +");
	for (j = 0; j < HIST_WIDTH + 1; j++)
		color_printf(COLOR_WHITE, "-");
	color_printf(COLOR_WHITE, "+\n");

	return CMD_ERROR_NONE;
}

int cmd_cam_store(struct command_context *ctx)
{
	cam_store_t store;
	cam_store_reply_t reply;
	char filename[40];
	uint8_t scale = 0;

	memset(filename, 0, sizeof(filename));

	if (ctx->argc > 1)
		strncpy(filename, ctx->argv[1], sizeof(filename) - 1);
	if (ctx->argc > 2)
		scale = atoi(ctx->argv[2]);

	memcpy(store.filename, filename, sizeof(store.filename));
	store.filename[sizeof(store.filename) - 1] = '\0';
	store.format = cam_format;
	store.flags = 0;
	store.scale = scale;

	int result = nanocam_store(&store, &reply, cam_timeout);
	if (result < 0) {
		printf("Nanocam store error: %"PRId8"\n", result);
		return CMD_ERROR_FAIL;
	}

	printf("Result %"PRIu8"\n", reply.result);
	printf("Image 0x%08"PRIx32" %"PRIu32"\n",
	       reply.image_ptr, reply.image_size);

	return CMD_ERROR_NONE;
}

int cmd_cam_read(struct command_context *ctx)
{
	int ret;
	uint8_t reg;
	uint16_t value;
	char *args = command_args(ctx);

	if (sscanf(args, "%hhx", &reg) != 1)
		return CMD_ERROR_SYNTAX;

	ret = nanocam_reg_read(reg, &value, cam_timeout);
	if (ret < 0) {
		printf("Failed to contact camera: %d\n", ret);
		return CMD_ERROR_FAIL;
	}

	printf("Register 0x%02hhx value 0x%04hx\r\n", reg, value);

	return CMD_ERROR_NONE;
}

int cmd_cam_write(struct command_context *ctx)
{
	int ret;
	uint8_t reg;
	uint16_t value;
	char *args = command_args(ctx);

	if (sscanf(args, "%hhx %hx", &reg, &value) != 2)
		return CMD_ERROR_SYNTAX;

	ret = nanocam_reg_write(reg, value, cam_timeout);
	if (ret < 0) {
		printf("Failed to contact camera: %d\n", ret);
		return CMD_ERROR_FAIL;
	}

	printf("Register 0x%02hhx value 0x%04hx\r\n", reg, value);

	return CMD_ERROR_NONE;
}

static void cam_list_cb(int seq, cam_list_element_t *elm)
{
	if (!elm) {
		printf("No images to list\n");
	} else {
		uint32_t time_sec = elm->time / 1000000000;
		printf("[%02u] F:%"PRIu8", T:%"PRIu32" 0x%08"PRIx32" %"PRIu32"\n",
		       seq, elm->format, time_sec, elm->ptr, elm->size);
	}
}

int cmd_cam_list(struct command_context *ctx)
{
	int ret;

	ret = nanocam_img_list(cam_list_cb, cam_timeout);
	if (ret < 0) {
		printf("Image list failed\n");
		return CMD_ERROR_FAIL;
	}

	return CMD_ERROR_NONE;
}

int cmd_cam_list_flush(struct command_context *ctx)
{
	int ret;

	ret = nanocam_img_list_flush(cam_timeout);
	if (ret < 0) {
		printf("Image list flush failed\n");
		return CMD_ERROR_FAIL;
	}

	return CMD_ERROR_NONE;
}

int cmd_cam_focus(struct command_context *ctx)
{
	uint32_t af;

	int result = nanocam_focus(0, &af, cam_timeout);
	if (result < 0) {
		printf("Nanocam focus error: %"PRId8"\n", result);
		return CMD_ERROR_FAIL;
	}

	printf("Result %"PRIu32"\n", af);

	return CMD_ERROR_NONE;
}

int cmd_cam_recoverfs(struct command_context *ctx)
{
	int timeout = cam_timeout;

	/* It takes a while to recreate the file system, so
	 * use a minimum timeout of 120s */
	if (timeout < 120000) {
		printf("warning: adjusting timeout to 120s\n");
		timeout = 120000;
	}

	int result = nanocam_recoverfs(timeout);
	if (result < 0) {
		printf("Nanocam recoverfs error: %"PRId8"\n", result);
		return CMD_ERROR_FAIL;
	}

	return CMD_ERROR_NONE;
}

command_t __sub_command cam_subcommands[] = {
	{
		.name = "node",
		.help = "Set CAM address in host table",
		.usage = "<node>",
		.handler = cmd_cam_node,
	},{
		.name = "timeout",
		.help = "Set timeout",
		.usage = "<time>",
		.handler = cmd_cam_set_timeout,
	},{
		.name = "format",
		.help = "Set store format (0=raw, 1=bmp, 2=jpg)",
		.usage = "<format>",
		.handler = cmd_cam_format,
	},{
		.name = "snap",
		.help = "Snap picture",
		.usage = "[-a][-d <delay>][-h <color>][-i][-n <count>][-r][-s][-t][-x]",
		.handler = cmd_cam_snap,
	},{
		.name = "store",
		.help = "Store picture",
		.usage = "[path] [scale-down]",
		.handler = cmd_cam_store,
	},{
		.name = "read",
		.help = "Read sensor register",
		.usage = "<reg-hex>",
		.handler = cmd_cam_read,
	},{
		.name = "write",
		.help = "Write sensor register",
		.usage = "<reg-hex> <value-hex>",
		.handler = cmd_cam_write,
	},{
		.name = "list",
		.help = "List RAM pictures",
		.handler = cmd_cam_list,
	},{
		.name = "flush",
		.help = "Flush RAM pictures",
		.handler = cmd_cam_list_flush,
	},{
		.name = "focus",
		.help = "Run focus assist algorithm",
		.handler = cmd_cam_focus,
	},{
		.name = "recoverfs",
		.help = "Recreate data file system",
		.handler = cmd_cam_recoverfs,
	}
};

command_t __root_command cam_commands[] = {
	{
		.name = "cam",
		.help = "client: CAM",
		.chain = INIT_CHAIN(cam_subcommands),
	}
};
