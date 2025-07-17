/**
 * NanoCam Parameter Tables
 *
 * Copyright 2015 GomSpace ApS. All rights reserved.
 */

#ifndef __NANOCAM_PARAMS_H__
#define __NANOCAM_PARAMS_H__

#include <stddef.h>
#include <stdint.h>
#include <param/param_types.h>

/*** Configuration ***/
#define NANOCAM_RTABLE_STR_SIZE	0x60
struct __attribute__ ((packed)) nanocam_config_params {
	uint8_t csp_addr;
	uint8_t csp_gateway;
	char csp_rtable[NANOCAM_RTABLE_STR_SIZE];
	uint32_t can_bitrate;
	uint32_t i2c_bitrate;
	uint32_t kiss_bitrate;
};

#define NANOCAM_CSP_ADDR	offsetof(struct nanocam_config_params, csp_addr)
#define NANOCAM_CSP_GATEWAY	offsetof(struct nanocam_config_params, csp_gateway)
#define NANOCAM_CSP_RTABLE	offsetof(struct nanocam_config_params, csp_rtable)
#define NANOCAM_CAN_BITRATE	offsetof(struct nanocam_config_params, can_bitrate)
#define NANOCAM_I2C_BITRATE	offsetof(struct nanocam_config_params, i2c_bitrate)
#define NANOCAM_KISS_BITRATE	offsetof(struct nanocam_config_params, kiss_bitrate)

static const param_table_t nanocam_config_table[] = {
	{.name = "csp-addr",    .addr = NANOCAM_CSP_ADDR,    .type = PARAM_UINT8,  .size = sizeof(uint8_t)},
	{.name = "csp-gateway", .addr = NANOCAM_CSP_GATEWAY, .type = PARAM_UINT8,  .size = sizeof(uint8_t)},
	{.name = "csp-rtable",  .addr = NANOCAM_CSP_RTABLE,  .type = PARAM_STRING, .size = NANOCAM_RTABLE_STR_SIZE},
	{.name = "can-bitrate", .addr = NANOCAM_CAN_BITRATE, .type = PARAM_UINT32, .size = sizeof(uint32_t)},
	{.name = "i2c-bitrate", .addr = NANOCAM_I2C_BITRATE, .type = PARAM_UINT32, .size = sizeof(uint32_t)},
	{.name = "kiss-bitrate", .addr = NANOCAM_KISS_BITRATE, .type = PARAM_UINT32, .size = sizeof(uint32_t)},
};

#define NANOCAM_PARAM_CONFIG_SIZE	sizeof(struct nanocam_config_params)
static const int nanocam_config_count = sizeof(nanocam_config_table) / sizeof(nanocam_config_table[0]);

/*** Image ***/
struct __attribute__ ((packed)) nanocam_image_params {
	uint32_t exposure_us;
	uint8_t hist_skip;
	uint8_t gain_target;
	uint8_t gain_slack;
	uint8_t gain_iters;
	uint16_t gain_global;
	uint16_t gain_red;
	uint16_t gain_green;
	uint16_t gain_blue;
	uint16_t gain_max;
	uint8_t jpeg_qual;
	uint8_t jpeg_prog;
	uint8_t bayer_algo;
	uint8_t color_correct;
	uint8_t gamma_correct;
	uint8_t white_balance;
	float gamma;
	float gamma_break;
	float ccm_red[3];
	float ccm_green[3];
	float ccm_blue[3];
	uint8_t thumb_scale;
	bool led_en;
};

#define NANOCAM_IMG_EXPOSURE       offsetof(struct nanocam_image_params, exposure_us)
#define NANOCAM_IMG_HIST_SKIP      offsetof(struct nanocam_image_params, hist_skip)
#define NANOCAM_IMG_GAIN_TARGET    offsetof(struct nanocam_image_params, gain_target)
#define NANOCAM_IMG_GAIN_SLACK     offsetof(struct nanocam_image_params, gain_slack)
#define NANOCAM_IMG_GAIN_ITERS     offsetof(struct nanocam_image_params, gain_iters)
#define NANOCAM_IMG_GAIN_GLOBAL    offsetof(struct nanocam_image_params, gain_global)
#define NANOCAM_IMG_GAIN_RED       offsetof(struct nanocam_image_params, gain_red)
#define NANOCAM_IMG_GAIN_GREEN     offsetof(struct nanocam_image_params, gain_green)
#define NANOCAM_IMG_GAIN_BLUE      offsetof(struct nanocam_image_params, gain_blue)
#define NANOCAM_IMG_GAIN_MAX       offsetof(struct nanocam_image_params, gain_max)
#define NANOCAM_IMG_JPEG_QUAL      offsetof(struct nanocam_image_params, jpeg_qual)
#define NANOCAM_IMG_JPEG_PROG      offsetof(struct nanocam_image_params, jpeg_prog)
#define NANOCAM_IMG_BAYER_ALGO     offsetof(struct nanocam_image_params, bayer_algo)
#define NANOCAM_IMG_COLOR_CORRECT  offsetof(struct nanocam_image_params, color_correct)
#define NANOCAM_IMG_GAMMA_CORRECT  offsetof(struct nanocam_image_params, gamma_correct)
#define NANOCAM_IMG_WHITE_BALANCE  offsetof(struct nanocam_image_params, white_balance)
#define NANOCAM_IMG_GAMMA          offsetof(struct nanocam_image_params, gamma)
#define NANOCAM_IMG_GAMMA_BREAK    offsetof(struct nanocam_image_params, gamma_break)
#define NANOCAM_IMG_CCM_RED(_i)    offsetof(struct nanocam_image_params, ccm_red[(_i)])
#define NANOCAM_IMG_CCM_GREEN(_i)  offsetof(struct nanocam_image_params, ccm_green[(_i)])
#define NANOCAM_IMG_CCM_BLUE(_i)   offsetof(struct nanocam_image_params, ccm_blue[(_i)])
#define NANOCAM_IMG_THUMB_SCALE    offsetof(struct nanocam_image_params, thumb_scale)
#define NANOCAM_IMG_LED_EN         offsetof(struct nanocam_image_params, led_en)

static const param_table_t nanocam_image_table[] = {
	{.name = "exposure-us",   .addr = NANOCAM_IMG_EXPOSURE,       .type = PARAM_UINT32, .size = sizeof(uint32_t)},
	{.name = "hist-skip",     .addr = NANOCAM_IMG_HIST_SKIP,      .type = PARAM_UINT8,  .size = sizeof(uint8_t)},
	{.name = "gain-target",   .addr = NANOCAM_IMG_GAIN_TARGET,    .type = PARAM_UINT8,  .size = sizeof(uint8_t)},
	{.name = "gain-slack",    .addr = NANOCAM_IMG_GAIN_SLACK,     .type = PARAM_UINT8,  .size = sizeof(uint8_t)},
	{.name = "gain-iters",    .addr = NANOCAM_IMG_GAIN_ITERS,     .type = PARAM_UINT8,  .size = sizeof(uint8_t)},
	{.name = "gain-global",	  .addr = NANOCAM_IMG_GAIN_GLOBAL,    .type = PARAM_UINT16, .size = sizeof(uint16_t)},
	{.name = "gain-red",	  .addr = NANOCAM_IMG_GAIN_RED,       .type = PARAM_UINT16, .size = sizeof(uint16_t)},
	{.name = "gain-green",	  .addr = NANOCAM_IMG_GAIN_GREEN,     .type = PARAM_UINT16, .size = sizeof(uint16_t)},
	{.name = "gain-blue",	  .addr = NANOCAM_IMG_GAIN_BLUE,      .type = PARAM_UINT16, .size = sizeof(uint16_t)},
	{.name = "gain-max",	  .addr = NANOCAM_IMG_GAIN_MAX,       .type = PARAM_UINT16, .size = sizeof(uint16_t)},
	{.name = "jpeg-qual",	  .addr = NANOCAM_IMG_JPEG_QUAL,      .type = PARAM_UINT8,  .size = sizeof(uint8_t)},
	{.name = "jpeg-prog",	  .addr = NANOCAM_IMG_JPEG_PROG,      .type = PARAM_BOOL,   .size = sizeof(uint8_t)},
	{.name = "bayer-algo",	  .addr = NANOCAM_IMG_BAYER_ALGO,     .type = PARAM_UINT8,  .size = sizeof(uint8_t)},
	{.name = "color-correct", .addr = NANOCAM_IMG_COLOR_CORRECT,  .type = PARAM_BOOL,   .size = sizeof(uint8_t)},
	{.name = "gamma-correct", .addr = NANOCAM_IMG_GAMMA_CORRECT,  .type = PARAM_BOOL,   .size = sizeof(uint8_t)},
	{.name = "white-balance", .addr = NANOCAM_IMG_WHITE_BALANCE,  .type = PARAM_BOOL,   .size = sizeof(uint8_t)},
	{.name = "gamma",         .addr = NANOCAM_IMG_GAMMA,          .type = PARAM_FLOAT,  .size = sizeof(float)},
	{.name = "gamma-break",   .addr = NANOCAM_IMG_GAMMA_BREAK,    .type = PARAM_FLOAT,  .size = sizeof(float)},
	{.name = "ccm-red",       .addr = NANOCAM_IMG_CCM_RED(0),     .type = PARAM_FLOAT,  .size = sizeof(float), .count = 3},
	{.name = "ccm-green",     .addr = NANOCAM_IMG_CCM_GREEN(0),   .type = PARAM_FLOAT,  .size = sizeof(float), .count = 3},
	{.name = "ccm-blue",      .addr = NANOCAM_IMG_CCM_BLUE(0),    .type = PARAM_FLOAT,  .size = sizeof(float), .count = 3},
	{.name = "thumb-scale",   .addr = NANOCAM_IMG_THUMB_SCALE,    .type = PARAM_UINT8,  .size = sizeof(uint8_t)},
	{.name = "led_en",        .addr = NANOCAM_IMG_LED_EN,         .type = PARAM_BOOL,   .size = sizeof(uint8_t)},
};

#define NANOCAM_PARAM_IMAGE_SIZE	sizeof(struct nanocam_image_params)
static const int nanocam_image_count = sizeof(nanocam_image_table) / sizeof(nanocam_image_table[0]);

/*** Sensor ***/
#define NANOCAM_SENSOR_CHIP_VERSION		0x00
#define NANOCAM_SENSOR_ROW_START		0x01
#define NANOCAM_SENSOR_COL_START		0x02
#define NANOCAM_SENSOR_ROW_SIZE			0x03
#define NANOCAM_SENSOR_COL_SIZE			0x04
#define NANOCAM_SENSOR_HORZ_BLANK		0x05
#define NANOCAM_SENSOR_VERT_BLANK		0x06
#define NANOCAM_SENSOR_OUTPUT_CTRL		0x07
#define NANOCAM_SENSOR_SH_WIDTH_UP		0x08
#define NANOCAM_SENSOR_SH_WIDTH			0x09
#define NANOCAM_SENSOR_PIXCLK_CTRL		0x0a
#define NANOCAM_SENSOR_FRAME_RST		0x0b
#define NANOCAM_SENSOR_SHUTTER_DELAY		0x0c
#define NANOCAM_SENSOR_RESET			0x0d
#define NANOCAM_SENSOR_READ_MODE1		0x1e
#define NANOCAM_SENSOR_READ_MODE2		0x20
#define NANOCAM_SENSOR_READ_MODE3		0x21
#define NANOCAM_SENSOR_ROW_ADDR_MODE		0x22
#define NANOCAM_SENSOR_COL_ADDR_MODE		0x23
#define NANOCAM_SENSOR_GREEN1_GAIN		0x2b
#define NANOCAM_SENSOR_BLUE_GAIN		0x2c
#define NANOCAM_SENSOR_RED_GAIN			0x2d
#define NANOCAM_SENSOR_GREEN2_GAIN		0x2e
#define NANOCAM_SENSOR_TESTDATA			0x32
#define NANOCAM_SENSOR_GLOBAL_GAIN		0x35
#define NANOCAM_SENSOR_BLACKLEVEL		0x49
#define NANOCAM_SENSOR_ROW_BLK_OFF		0x4b
#define NANOCAM_SENSOR_BLC_DELTA		0x5d
#define NANOCAM_SENSOR_CAL_THRES		0x5f
#define NANOCAM_SENSOR_GREEN1_OFFSET		0x60
#define NANOCAM_SENSOR_GREEN2_OFFSET		0x61
#define NANOCAM_SENSOR_BLC			0x62
#define NANOCAM_SENSOR_RED_OFFSET		0x63
#define NANOCAM_SENSOR_BLUE_OFFSET		0x64
#define NANOCAM_SENSOR_CHIP_ENABLE		0xf8
#define NANOCAM_SENSOR_CHIP_VERSION2		0xff

static const param_table_t nanocam_sensor_table[] = {
	{.name = "chip-version",  .addr = NANOCAM_SENSOR_CHIP_VERSION,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "row-start",	  .addr = NANOCAM_SENSOR_ROW_START,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "col-start",	  .addr = NANOCAM_SENSOR_COL_START,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "row-size",	  .addr = NANOCAM_SENSOR_ROW_SIZE,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "col-size",	  .addr = NANOCAM_SENSOR_COL_SIZE,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "horz-blank",	  .addr = NANOCAM_SENSOR_HORZ_BLANK,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "vert-blank",	  .addr = NANOCAM_SENSOR_VERT_BLANK,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "output-ctrl",	  .addr = NANOCAM_SENSOR_OUTPUT_CTRL,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "sh-width-up",	  .addr = NANOCAM_SENSOR_SH_WIDTH_UP,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "sh-width",	  .addr = NANOCAM_SENSOR_SH_WIDTH,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "pixclk-ctrl",	  .addr = NANOCAM_SENSOR_PIXCLK_CTRL,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "frame-rst",	  .addr = NANOCAM_SENSOR_FRAME_RST,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "shutter-delay", .addr = NANOCAM_SENSOR_SHUTTER_DELAY,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "reset",	  .addr = NANOCAM_SENSOR_RESET,		.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "read-mode1",	  .addr = NANOCAM_SENSOR_READ_MODE1,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "read-mode2",	  .addr = NANOCAM_SENSOR_READ_MODE2,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "read-mode3",	  .addr = NANOCAM_SENSOR_READ_MODE3,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "row-addr-mode", .addr = NANOCAM_SENSOR_ROW_ADDR_MODE,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "col-addr-mode", .addr = NANOCAM_SENSOR_COL_ADDR_MODE,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "green1-gain",	  .addr = NANOCAM_SENSOR_GREEN1_GAIN,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "blue-gain",	  .addr = NANOCAM_SENSOR_BLUE_GAIN,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "red-gain",	  .addr = NANOCAM_SENSOR_RED_GAIN,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "green2-gain",	  .addr = NANOCAM_SENSOR_GREEN2_GAIN,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "testdata",	  .addr = NANOCAM_SENSOR_TESTDATA,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "global-gain",	  .addr = NANOCAM_SENSOR_GLOBAL_GAIN,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "black-level",	  .addr = NANOCAM_SENSOR_BLACKLEVEL,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "row-blk-off",	  .addr = NANOCAM_SENSOR_ROW_BLK_OFF,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "blc-delta",	  .addr = NANOCAM_SENSOR_BLC_DELTA,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "cal-thres",	  .addr = NANOCAM_SENSOR_CAL_THRES,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "green1-offset", .addr = NANOCAM_SENSOR_GREEN1_OFFSET,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "green2-offset", .addr = NANOCAM_SENSOR_GREEN2_OFFSET,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "blc",		  .addr = NANOCAM_SENSOR_BLC,		.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "red-offset",	  .addr = NANOCAM_SENSOR_RED_OFFSET,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "blue-offset",	  .addr = NANOCAM_SENSOR_BLUE_OFFSET,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "chip-enable",	  .addr = NANOCAM_SENSOR_CHIP_ENABLE,	.type = PARAM_X16, .size = sizeof(uint16_t)},
	{.name = "chip-version2", .addr = NANOCAM_SENSOR_CHIP_VERSION2,	.type = PARAM_X16, .size = sizeof(uint16_t)},
};

#define NANOCAM_PARAM_SENSOR_SIZE	(0xff * sizeof(uint16_t))
static const int nanocam_sensor_count = sizeof(nanocam_sensor_table) / sizeof(nanocam_sensor_table[0]);

/*** Tags ***/
struct __attribute__ ((packed)) nanocam_tag_params {
	bool tag_adcs;
	uint8_t tag_adcs_host;
	uint8_t tag_adcs_port;
	uint16_t tag_adcs_timeout;
};

#define NANOCAM_TAG_ADCS          offsetof(struct nanocam_tag_params, tag_adcs)
#define NANOCAM_TAG_ADCS_HOST     offsetof(struct nanocam_tag_params, tag_adcs_host)
#define NANOCAM_TAG_ADCS_PORT     offsetof(struct nanocam_tag_params, tag_adcs_port)
#define NANOCAM_TAG_ADCS_TIMEOUT  offsetof(struct nanocam_tag_params, tag_adcs_timeout)

static const param_table_t nanocam_tag_table[] = {
	{.name = "tag-adcs",	  .addr = NANOCAM_TAG_ADCS,         .type = PARAM_BOOL,   .size = sizeof(uint8_t)},
	{.name = "tag-adcs-host", .addr = NANOCAM_TAG_ADCS_HOST,    .type = PARAM_UINT8,  .size = sizeof(uint8_t)},
	{.name = "tag-adcs-port", .addr = NANOCAM_TAG_ADCS_PORT,    .type = PARAM_UINT8,  .size = sizeof(uint8_t)},
	{.name = "tag-adcs-tout", .addr = NANOCAM_TAG_ADCS_TIMEOUT, .type = PARAM_UINT16, .size = sizeof(uint16_t)},
};

#define NANOCAM_PARAM_TAG_SIZE	sizeof(struct nanocam_tag_params)
static const int nanocam_tag_count = sizeof(nanocam_tag_table) / sizeof(nanocam_tag_table[0]);

/*** Telemetry ***/
struct __attribute__ ((packed)) nanocam_telem_params {
	/* Persistent counters */
	uint32_t boot_count;
	uint32_t image_count;
	/* System info */
	uint64_t unixtime;
	uint32_t uptime;
	uint8_t loads[3];
	uint32_t freeram;
	uint16_t procs;
	/* Analog inputs */
	int16_t temp1;
	int16_t temp2;
	uint16_t icore;
	uint16_t iddr;
	uint16_t ivcc;
	uint16_t vddcore;
	uint16_t vddioddr;
	uint16_t vcc;
};

/* Persistent counters */
#define NANOCAM_BOOT_COUNT	offsetof(struct nanocam_telem_params, boot_count)
#define NANOCAM_IMAGE_COUNT	offsetof(struct nanocam_telem_params, image_count)
/* System info */
#define NANOCAM_UNIXTIME	offsetof(struct nanocam_telem_params, unixtime)
#define NANOCAM_UPTIME		offsetof(struct nanocam_telem_params, uptime)
#define NANOCAM_LOADS(i)	(offsetof(struct nanocam_telem_params, loads) + sizeof(uint8_t) * (i))
#define NANOCAM_FREERAM		offsetof(struct nanocam_telem_params, freeram)
#define NANOCAM_PROCS		offsetof(struct nanocam_telem_params, procs)
/* Analog inputs */
#define NANOCAM_TEMP1		offsetof(struct nanocam_telem_params, temp1)
#define NANOCAM_TEMP2		offsetof(struct nanocam_telem_params, temp2)
#define NANOCAM_ICORE		offsetof(struct nanocam_telem_params, icore)
#define NANOCAM_IDDR		offsetof(struct nanocam_telem_params, iddr)
#define NANOCAM_IVCC		offsetof(struct nanocam_telem_params, ivcc)
#define NANOCAM_VDDCORE		offsetof(struct nanocam_telem_params, vddcore)
#define NANOCAM_VDDIODDR	offsetof(struct nanocam_telem_params, vddioddr)
#define NANOCAM_VCC		offsetof(struct nanocam_telem_params, vcc)

static const param_table_t nanocam_telem_table[] = {
	/* Persistent counters */
	{.name = "boot-count", .addr = NANOCAM_BOOT_COUNT, .type = PARAM_UINT32, .size = sizeof(uint32_t), .flags = PARAM_F_PERSIST},
	{.name = "image-count", .addr = NANOCAM_IMAGE_COUNT, .type = PARAM_UINT32, .size = sizeof(uint32_t), .flags = PARAM_F_PERSIST},
	/* System info */
	{.name = "unixtime", .addr = NANOCAM_UNIXTIME, .type = PARAM_UINT64, .size = sizeof(uint64_t)},
	{.name = "uptime",   .addr = NANOCAM_UPTIME,   .type = PARAM_UINT32, .size = sizeof(uint32_t)},
	{.name = "loads",    .addr = NANOCAM_LOADS(0), .type = PARAM_UINT8,  .size = sizeof(uint8_t), .count = 3},
	{.name = "freeram",  .addr = NANOCAM_FREERAM,  .type = PARAM_UINT32, .size = sizeof(uint32_t)},
	{.name = "procs",    .addr = NANOCAM_PROCS,    .type = PARAM_UINT16, .size = sizeof(uint16_t)},
	/* Analog inputs */
	{.name = "temp1",    .addr = NANOCAM_TEMP1,    .type = PARAM_INT16,  .size = sizeof(int16_t)},
	{.name = "temp2",    .addr = NANOCAM_TEMP2,    .type = PARAM_INT16,  .size = sizeof(int16_t)},
	{.name = "icore",    .addr = NANOCAM_ICORE,    .type = PARAM_UINT16, .size = sizeof(uint16_t)},
	{.name = "iddr",     .addr = NANOCAM_IDDR,     .type = PARAM_UINT16, .size = sizeof(uint16_t)},
	{.name = "ivcc",     .addr = NANOCAM_IVCC,     .type = PARAM_UINT16, .size = sizeof(uint16_t)},
	{.name = "vddcore",  .addr = NANOCAM_VDDCORE,  .type = PARAM_UINT16, .size = sizeof(uint16_t)},
	{.name = "vddioddr", .addr = NANOCAM_VDDIODDR, .type = PARAM_UINT16, .size = sizeof(uint16_t)},
	{.name = "vcc",      .addr = NANOCAM_VCC,      .type = PARAM_UINT16, .size = sizeof(uint16_t)},
};

#define NANOCAM_PARAM_TELEM_SIZE	sizeof(struct nanocam_telem_params)
#define NANOCAM_TELEM_COUNT 		sizeof(nanocam_telem_table) / sizeof(nanocam_telem_table[0])

static const int nanocam_telem_count = sizeof(nanocam_telem_table) / sizeof(nanocam_telem_table[0]);

#endif /* __NANOCAM_PARAMS_H__ */
