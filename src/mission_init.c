/**
 * Mission Library specifics
 */

#include <stdio.h>

#include <hk_service.h>
#include <hk_persist.h>
#include <hk_types.h>

#define HK_NO_TABLES   1
#define HK_NO_BEACONS  1

/** Define your HK tables here */
hk_table_t hk_tables[HK_NO_TABLES] = {
	{
		.node = 1,
		.tableid = 0,
		.memid = 21,
		.table = NULL,
		.count = 0,
		.size = 0,
		.nodename = "obc",
		.param_interval = 10,
	}
};

int hk_tables_count = HK_NO_TABLES;

/**
 * Define your beacon formats here
 */
static const uint16_t hk_beacon_format_0[] = {

	/** Fill in your beacon definition here */
	0x8000 | 21,
	0,
};

/**
 * List your beacon formats here
 */
hk_beacon_t hk_beacons[HK_NO_BEACONS] = {
	{
		.addrs = hk_beacon_format_0,
		.count = sizeof(hk_beacon_format_0) / sizeof(hk_beacon_format_0[0]),
		.param_interval = 0,
		.node = 1,
		.type = 0
	},
};

int hk_beacons_count = HK_NO_BEACONS;

