/**
 * GomSpace Parameter System
 *
 * @author Johan De Claville Christiansen
 * Copyright 2014 GomSpace ApS. All rights reserved.
 */

#ifndef GOMX3_ADCS_HKPARAM_H_
#define GOMX3_ADCS_HKPARAM_H_

#include <stddef.h>
#include <stdint.h>
#include <param/param_types.h>

/**
 * Define memory space
 */
#define GOMX3_HK_INTV_ADCS_BOARD		0x01
#define GOMX3_HK_INTV_ADCS_TELEM1		0x02
#define GOMX3_HK_INTV_ADCS_TELEM2		0x03
#define GOMX3_HK_INTV_ADCS_TELEM3		0x04
#define GOMX3_HK_INTV_ADCS_TELEM4		0x05
#define GOMX3_HK_INTV_ADCS_TELEM5		0x06
#define GOMX3_HK_INTV_ADCS_TELEM6		0x07

#define GOMX3_HK_BCN_INTERVAL_ADCS(i)	0x10 + (i * 2)

#define GOMX3_HK_SIZE_ADCS				0x20

/**
 * Setup info about parameters
 */
static const param_table_t gomx3_hk_param_adcs[] = {

		{.name = "col_adcs_brd", 	.addr = GOMX3_HK_INTV_ADCS_BOARD,	.type = PARAM_UINT8,	.size = sizeof(uint8_t)},
		{.name = "col_adcs_tlm1", 	.addr = GOMX3_HK_INTV_ADCS_TELEM1,	.type = PARAM_UINT8,	.size = sizeof(uint8_t)},
		{.name = "col_adcs_tlm2", 	.addr = GOMX3_HK_INTV_ADCS_TELEM2,	.type = PARAM_UINT8,	.size = sizeof(uint8_t)},
		{.name = "col_adcs_tlm3", 	.addr = GOMX3_HK_INTV_ADCS_TELEM3,	.type = PARAM_UINT8,	.size = sizeof(uint8_t)},
		{.name = "col_adcs_tlm4", 	.addr = GOMX3_HK_INTV_ADCS_TELEM4,	.type = PARAM_UINT8,	.size = sizeof(uint8_t)},
		{.name = "col_adcs_tlm5", 	.addr = GOMX3_HK_INTV_ADCS_TELEM5,	.type = PARAM_UINT8,	.size = sizeof(uint8_t)},
		{.name = "col_adcs_tlm6", 	.addr = GOMX3_HK_INTV_ADCS_TELEM6,	.type = PARAM_UINT8,	.size = sizeof(uint8_t)},

		{.name = "bcn_interval", 	.addr = GOMX3_HK_BCN_INTERVAL_ADCS(0),	.type = PARAM_UINT16,	.size = sizeof(uint16_t), .count = 8},

};

#define GOMX3_HK_PARAM_COUNT_ADCS (sizeof(gomx3_hk_param_adcs) / sizeof(gomx3_hk_param_adcs[0]))

#endif /* GOMX3_ADCS_HKPARAM_H_ */
