#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

#include <FreeRTOS.h>
#include <task.h>

#include <util/clock.h>
#include <util/gs_rtc.h>
#include <conf_util.h>

/* Local time offset */
static timestamp_t utc_offset = {0, 0};

void clock_get_time(timestamp_t * time) {

	/* Get system clock */
	clock_get_monotonic(time);

#if RTC_ENABLED
	if (utc_offset.tv_sec == 0 && utc_offset.tv_nsec == 0) {
		/* If no utc_offset has been set, try to get it from RTC */
		timestamp_t rtc_time;
		rtc_get_time(&rtc_time);

		/* Offset = RTC - TickTime */
		timestamp_diff(&rtc_time, time);
		timestamp_copy(&rtc_time, &utc_offset);
	}
#endif

	/* Add offset */
	timestamp_add(time, &utc_offset);

}

void clock_set_time(timestamp_t * utc_time) {

	/* We also store a backup of the UTC time in the RTC, but this is low precision
	 * So it's only used once at bootup to give an initial time in case no
	 * network time sync node is present. */
#if RTC_ENABLED
	rtc_set_time(utc_time);
#endif

	/* Get system clock */
	timestamp_t tick;
	clock_get_monotonic(&tick);

	/* Offfset = UTC - TickTime */
	timestamp_diff(utc_time, &tick);
	timestamp_copy(utc_time, &utc_offset);

}

void clock_get_monotonic(timestamp_t * time) {

	uint64_t nsec = clock_get_nsec();

	/* Return current tick-timer value */
	time->tv_sec = nsec / 1000000000;
	time->tv_nsec = nsec % 1000000000;

}
