/**
 *
 * @author Johan De Claville Christiansen
 * Copyright 2012 GomSpace ApS. All rights reserved.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>

#include <conf_cspterm.h>
#include <csp-term.h>

/* Parameter system */
#include <param/param.h>
#include <param/param_example_table.h>

/* Log system */
#include <log/log.h>

/* CSP */
#include <csp/csp.h>
#include <csp/interfaces/csp_if_kiss.h>
#include <csp/interfaces/csp_if_can.h>
#include <csp/interfaces/csp_if_zmqhub.h>
#include <csp/drivers/usart.h>

/* Drivers / Util */
#include <util/console.h>
#include <util/log.h>
#include <util/vmem.h>
#include <util/clock.h>
#include <util/timestamp.h>
#include <util/strtime.h>

/*Create own command */
#include <command/command.h>

#define isLeapYear(x) (((x % 4) == 0) && (((x % 100) != 0) || ((x % 400) == 0)))

char * get_time(char * my_str)
	{
	timestamp_t new_time;
	clock_get_time(&new_time);
	int64_t converted_time;
	converted_time = *(int64_t*)&new_time; //special conversion
	get_format_timestamp(my_str, converted_time+28800, 1970); //+8 hour to singapore time //linked to the function below
	return my_str;
	}

int get_format_timestamp(char *str, int64_t utime, int year_base) { //modified library time format function
	char sign = ' ';
	if(utime < 0) {
		sign = '-';
		utime = -utime;
	}
	int days[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
	int daysl[12] = {31,29,31,30,31,30,31,31,30,31,30,31};
	int *daysp;
	int year = year_base;
	int day = 1;
	int mon = 1;
	int hour = 0;
	int min = 0;
	int sec = 365*24*60*60;
	int tmp;
	//int ms = utime%1000;
	int s = (int)utime;

	// Count years
	tmp = isLeapYear(year) ? 366 : 365;
	sec = tmp*24*60*60;
	// Keep counting years for as long as there are more seconds left
	// in the pool than there seconds in the next coming year.
	while(s > sec) {
		s -= sec;
		year ++;
		tmp = isLeapYear(year) ? 366 : 365;
		sec = tmp*24*60*60;
	}
	sec = 31*24*60*60;
	daysp = isLeapYear(year) ? daysl : days;
	// Count months
	while(s > sec) {
		mon ++;
		s -= sec;
		sec = daysp[mon-1]*24*60*60;
	}
	day = s/(24*60*60);
	s -= day*24*60*60;
	day ++;
	hour = s/(60*60);
	s -= hour*60*60;
	min = s/60;
	s -= min*60;
	// If year base is the Unix Epoch Year then we have an absolute time/date
	if(year_base == 1970) {
		sprintf(str,"%02d-%02d-%4d %02d:%02d:%02d",day,mon,year,hour,min,s);
	}
	// Otherwise, we just count the years, months, days, hours, minutes, seconds....
	else {
		mon--;
		day--;
		if(year > 0)
			sprintf(str,"%c%d years %d months %d days %d hours %d minutes %d seconds",sign,year,mon,day,hour,min,s);
		else if(mon > 0)
			sprintf(str,"%c%d months %d days %d hours %d minutes %d seconds",sign,mon,day,hour,min,s);
		else if(day > 0)
			sprintf(str,"%c%d days %d hours %d minutes %d seconds",sign,day,hour,min,s);
		else if(hour > 0)
			sprintf(str,"%c%d hours %d minutes %d seconds",sign,hour,min,s);
		else if(min > 0)
			sprintf(str,"%c%d minutes %d seconds",sign,min,s);
		else
			sprintf(str,"%c%d seconds",sign,s);
	}
	return strlen(str);
}


