#ifndef GS_RTC_H_
#define GS_RTC_H_

#include <util/clock.h>

void rtc_get_time(timestamp_t * time);
void rtc_set_time(timestamp_t * time);

#endif /* GS_RTC_H_ */
