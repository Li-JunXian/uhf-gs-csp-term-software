/**
 * @file predict.h
 */

#include <math.h>

typedef struct {
	double tsince, jul_epoch, jul_utc, eclipse_depth, sat_azi, sat_ele,
			sat_range, sat_range_rate, sat_lat, sat_lon, sat_alt, sat_vel,
			phase, sun_azi, sun_ele, daynum, fm, fk, age, aostime, lostime, ax,
			ay, az, rx, ry, rz, squint, alat, alon;
} sat_info_t;


/** set the TLE line 1 and 2 for the satellite
 *
 * @param line1: TLE line 1
 * @param line2: TLE line 2
 * @return
 */
int setTLE(char* line1, char* line2);

/**
 * set latitude, longitude and altitude for the ground station
 * @param lat
 * @param lon
 * @param alt
 */
void setStation(double lat, double lon, int alt);

/**
 * Get all info about the satellite lon,lat range etc.
 * @param info
 */
void getSatInfo(sat_info_t *info);

/**
 * Find next AOS
 * @return next_aos (not a unix timestamp)
 * to convert to a unix time stamp do:
 * unix_time = floor(86400.0*(3651.0+next_aos));
 */
double NextAOS();

/**
 * Set the time to calculate the sat info
 * @param time: A unix time stamp
 */
void set_calc_time(long time);

/**
 * Calculate the Doppler compensated frq
 * @param frq The radio frq
 * @param direction 1 = rx, 0 = tx
 * @return The Doppler compensated frq
 */
long int comp_dopp_frq(long int frq, int direction);

/**
 * Get azimuth angle to the sat
 * @return azimuth angle
 */
double get_azi(void);

/**
 * Get elevation angle to the sat
 * @return elevation angle
 */
double get_ele(void);

/**
 * Get satellite latitude
 * @return satellite latitude
 */
double get_satlat(void);

/**
 * Get satellite longtitude
 * @return satellite longtitude
 */
double get_satlon(void);

/**
 * Get satellite altitude
 * @return satellite altitude
 */
double get_satalt(void);
/**
 * Get satellite velocity
 * @return satellite velocity
 */
double get_satvel(void);

double get_sataos(void);

double get_satlos(void);

/** This function copies TLE data from PREDICT's sat structure
*	 to the SGP4 single dimensioned tle structure.
*	 Must be run once before doing calc.
*/
void PreCalc();

/**
 * Do the repetitive calculation of sat info
 */
void Calc();

/**
 * Check a TLE
 * @param line1
 * @param line2
 * @return 0 = ERR, 1 = OK
 */
char KepCheck(char *line1, char *line2);

/**
 * Find next AOS event
 * @return next_los (not a unix timestamp)
 * to convert to a unix time stamp do:
 * unix_time = floor(86400.0*(3651.0+next_los));
 */
double FindLOS2();

double FindAOS();
