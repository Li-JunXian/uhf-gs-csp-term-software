/**
 * @author Johan De Claville Christiansen
 * Copyright 2014 GomSpace ApS. All rights reserved.
 */

#ifndef GOMX3_ADCS_HK_H_
#define GOMX3_ADCS_HK_H_

#include <stdint.h>
#include <a3200_board.h>
#include <gomx3_adcs_hk_param.h>
#include <adcs/adcs_telem.h>
#include <hk_types.h>

/**
 * Here you must define all the parameter table index
 * numbers used by the houskeeping systems. Be careful
 * not to use numbers that are already in use on the
 * system.
 *
 * Also ensure that the max-param-table define is set
 * high enough to hold the highest table number.
 */
#define PARAM_HK_SETTING_ADCS	20
#define PARAM_INDEX_A3200_BOARD_ADCS 	24
/*
#define PARAM_ADCS_TELEM1		32
#define PARAM_ADCS_TELEM2		33
#define PARAM_ADCS_TELEM3		34
#define PARAM_ADCS_TELEM4		35
#define PARAM_ADCS_TELEM5		36
*/
#define ADCS_HK_NO_TABLES        7
#define ADCS_HK_NO_BEACONS       7

extern hk_table_t adcs_hk_tables[];
extern hk_beacon_t adcs_hk_beacons[];

#endif /* GOMX3_ADCS_HK_H_ */
