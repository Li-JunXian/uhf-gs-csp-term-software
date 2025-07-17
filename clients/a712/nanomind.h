/*
 * nanomind.h
 *
 *  Created on: 14/04/2010
 *      Author: oem
 */

#ifndef NANOMIND_H_
#define NANOMIND_H_

#include <stdint.h>
#include <util/timestamp.h>
#include <util/vermagic.h>

/** Default node address */
#define NODE_OBC				1

/** Beacon Downlink address and port */
#define OBC_HK_DOWN_NODE		10
#define OBC_HK_DOWN_PORT		14

/** Port numbers */
#define OBC_PORT_FTP				7
#define OBC_PORT_TIMESYNC			8
#define OBC_PORT_LOAD_IMG			9
#define OBC_PORT_JUMP				10
#define OBC_PORT_BOOT_CONF			11
#define OBC_PORT_FS_TO_FLASH		12
#define OBC_PORT_RAM_TO_ROM			13
#define OBC_PORT_LOGD				14
#define OBC_PORT_PARAM				15
#define OBC_PORT_HK					16
#define OBC_PORT_BOOT_COUNT			17

#define OBC_PORT_FP					18
#define OBC_PORT_CONN_LESS_TEST		19
#define OBC_PORT_ADCS				20

#define OBC_PORT_RSH				22

typedef struct obc_ram_to_rom_s {
	uint32_t size;
	uint32_t checksum;
	uint32_t src;
	uint32_t dst;
} obc_ram_to_rom_t;

void obc_set_node(uint8_t node);
void obc_timesync(timestamp_t * time, int timeout);
void obc_jump_ram(uint32_t addr);
void obc_load_image(const char * path);
void obc_boot_conf(uint32_t checksum, uint32_t boot_counts, const char * path);
void obc_boot_del(void);
void obc_fs_to_flash(uint32_t addr, const char * path);
void obc_ram_to_rom(uint32_t size, uint32_t checksum, uint32_t src, uint32_t dst);
int obc_boot_count_get(uint32_t *boot_count, int timeout);
int obc_boot_count_reset(uint32_t *boot_count, int timeout);

#endif /* NANOMIND_H_ */
