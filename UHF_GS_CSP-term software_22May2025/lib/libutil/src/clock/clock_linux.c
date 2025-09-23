#include <inttypes.h>
#include <stdint.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/ioctl.h>
#include <linux/rtc.h>

#include <util/clock.h>

void clock_get_time(timestamp_t * time) {

	struct timespec now;
	clock_gettime(CLOCK_REALTIME, &now);

	time->tv_sec = now.tv_sec;
	time->tv_nsec = now.tv_nsec;

}

void clock_set_time(timestamp_t * time) {

	int fd;
	struct timespec now;
	struct tm *tm;

	now.tv_sec = time->tv_sec;
	now.tv_nsec = time->tv_nsec;

	/* Set system time */
	clock_settime(CLOCK_REALTIME, &now);

	/* Set RTC time */
	fd = open("/dev/rtc", O_RDONLY | O_CLOEXEC);
	if (fd < 0)
		return;

	tm = gmtime(&now.tv_sec);
	ioctl(fd, RTC_SET_TIME, tm);

	close(fd);

}

void clock_get_monotonic(timestamp_t * time) {

	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);

	time->tv_sec = now.tv_sec;
	time->tv_nsec = now.tv_nsec;
}

uint64_t clock_get_nsec(void) {

	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);

	return (uint64_t)now.tv_sec * NSEC_PER_SECOND + now.tv_nsec;
}
