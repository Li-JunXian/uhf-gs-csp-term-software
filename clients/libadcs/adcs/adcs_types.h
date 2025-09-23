/*
 * adcs_client_types.h
 *
 *  Created on: May 25, 2011
 *      Author: jal
 */

#ifndef ADCS_CLIENT_TYPES_H_
#define ADCS_CLIENT_TYPES_H_

#include <inttypes.h>
#include <adcs/adcs_telem_types.h>

typedef float adcs_float_t;

#ifndef true
# define false 0
# define true 1
#endif
typedef uint8_t bool_t;

#define MAX_ADCS_CONFIG_SIZE 210  // Maximum number of bytes allowed in a config struct
#define NR_OF_ADCS_LOG_TYPES 5


typedef struct  __attribute__ ((__packed__)) {
	uint8_t	mode; // 0=stop, 1=start, 2=restart
} adcs_startstop_t;


typedef struct  __attribute__ ((__packed__)) {
	uint8_t  state;
} adcs_state_t;


typedef struct __attribute__ ((__packed__)) {
	double jDate;
	bool_t	validTime;
	uint32_t looptime;
	uint32_t maxlooptime;
} adcs_time_t;


typedef struct __attribute__ ((__packed__)) {
	float tumblerate[3];
	float tumblenorm[2];
	float torquerduty[3];
	float mag[3];
	bool_t magvalid;
	bool_t detumbled;
	int32_t timeSinceBdot;
} adcs_bdot_data_t;


/* Actuator commands */

typedef struct {
	float pwm[3];
} adcs_act_torquers_t;

typedef struct {
	uint8_t enable[4];
	float speed[4];
	float torque[4];
} adcs_act_mw_get_status_t;

typedef struct {
	uint8_t mw;
	float torque;
	uint32_t Ts;
} adcs_act_mw_set_t;

typedef struct {
	uint8_t mw;
	uint16_t current;
	uint16_t time;
} adcs_act_mw_diag_t;

typedef struct {
	uint8_t mw;
	uint8_t enable;
} adcs_act_mw_enable_t;


/* 	GPS	 */

#define GPS_CSP_MAX_LEN	180

typedef struct __attribute__ ((__packed__))  {
	uint16_t prn;
	uint16_t glofreq;
	uint32_t ch_tr_status;
	double psr;
	float doppler;
	float c_no;
	float locktime;
	float psr_res;
	uint32_t reject;
	float psr_weigth;
} adcs_gps_trackstat_sat_t;

typedef struct __attribute__ ((__packed__))  {
	uint32_t sol_status;
	uint32_t pos_type;
	float cutoff;
	uint32_t num_of_chans;
	adcs_gps_trackstat_sat_t sat[20];
} adcs_gps_trackstat_t;

/* Novatel rxstatus log response */
typedef struct __attribute__ ((__packed__))  {
	uint32_t error;
	uint32_t nr_stats;
	uint32_t rxstat;
	uint32_t aux1stat;
	uint32_t aux2stat;
	uint32_t aux3stat;
} adcs_gps_rxstatus_t;

typedef struct __attribute__ ((__packed__)) {
	uint16_t length;
	uint16_t timeout;
	char data[GPS_CSP_MAX_LEN];
} gps_ascii_cmd_t;

typedef struct __attribute__ ((__packed__)) {
	uint16_t length;
	uint16_t timeout;
	char data[GPS_CSP_MAX_LEN];
} gps_trackstat_cmd_t;

typedef struct __attribute__((__packed__)) {
	uint8_t len;				// Number of bytes in data
	uint8_t data[GPS_CSP_MAX_LEN];	// Used for holding ascii command for GPS
} gps_csp_res_t;

typedef struct __attribute__((__packed__)) {
	uint8_t header[16];		// header size
	uint8_t data[160];	// can contain 4 satellites
} gps_csp_track_res_t;

typedef struct __attribute__((__packed__)) {
	float temperature;
} adcs_gps_temp_t;

typedef struct __attribute__((__packed__)) {
	float posxyz[3];
	float velxyz[3];
	double jdate;
	uint8_t satellitesSol;
	uint8_t satellitesTrack;
	float gpsVelDeviation[3]; 				/**< Std. dev. of the GPS measurement*/
	float gpsPosDeviation[3]; 				/**< Std. dev. of the GPS measurement*/
} adcs_gps_sample_t;

typedef struct  __attribute__ ((__packed__)) {
	float posxyz[3];						/**< Last sampled position [meter] */
	float velxyz[3];						/**< Last sampled velocity [meter/sec] */
	double jdate;							/**< JDate of sample */
	bool_t valid;								/**< Sample valid [true=1]*/
} adcs_gps_last_sample_t;

typedef struct  __attribute__ ((__packed__)) {
	double jdate;							/**< JDate of last PPS */
	uint32_t unixtime;						/**< Unixtime of last PPS */
	bool_t valid;								/**< Time valid [true=1] */
} adcs_gps_last_pps_t;

/* Basic house keeping commands */

// This strcut is used by the nanocam interface.
typedef struct __attribute__ ((packed)) {
	double jdate;		// Julian date
	float q[4];			// q_is
	float w[3];			// w_is
	float pos[3];		// ECI position
	uint8_t eclipse;	// Sat. in eclipse = 1
} adcs_hk_basic_t;

/** Server commands */
typedef enum __attribute__ ((packed)) {
	/* General ADCS commands */
	ADCS_CMD_STARTSTOP = 10,			       	/**< Do a start/stop/restart of ADCS system		*/
	ADCS_CMD_GET_ADCS_STATE = 11,			/**< Get current ADCS states 				*/
	ADCS_CMD_SET_ADS_STATE = 12,				/**< Set desired ADS state 				*/
	ADCS_CMD_SET_ACS_STATE = 13 ,				/**< Set desired ACS state 				*/
	ADCS_CMD_SET_EPHEM_STATE = 14,			/**< Set desired EPHEM state 				*/

	/* Housekeeping commands */
	ADCS_CMD_HK_BASIC = 20,

	/* BDOT commands */
	ADCS_CMD_BDOT_FORCE_DETUMBLED = 25,		/**< Force BDOT filters into detumbled state	       	*/

	/* Pointing commands */
	ADCS_CMD_SET_RESET = 30,
	ADCS_CMD_SET_MINDRAG = 31,
	ADCS_CMD_SET_MAXDRAG = 32,
	ADCS_CMD_SET_RAM = 33,
	ADCS_CMD_SET_NADIR = 34,
	ADCS_CMD_SET_CUSTOM0 = 35,
	ADCS_CMD_SET_CUSTOM1 = 36,
	ADCS_CMD_SET_CUSTOM2 = 37,

	/* Tracking commands */
	ADCS_CMD_TRACK_RESET = 40,
	ADCS_CMD_TRACK_SUN = 41,
	ADCS_CMD_TRACK_MOON = 42,
	ADCS_CMD_TRACK_TARGET0 = 43,
	ADCS_CMD_TRACK_TARGET1 = 44,
	ADCS_CMD_TRACK_TARGET2 = 45,

	/*  GPS commands */
	ADCS_CMD_GPS_SAMPLE = 50,
	ADCS_CMD_GPS_TIME = 51,
	ADCS_CMD_GPS_RESET = 52,
	ADCS_CMD_GPS_ON = 53,					/**< Turn on the GPS */
	ADCS_CMD_GPS_OFF = 54,					/**< Turn off the GPS */
	ADCS_CMD_GPS_ASCII = 55,				/**< Ascii command interface to the GPS */
	ADCS_CMD_GPS_TRACKSTAT = 56,			/**< List and status of GPS satellites tracked */
	ADCS_CMD_GPS_RXSTAT = 57,				/**< Get reciever status */
	ADCS_CMD_GPS_TEMP = 58,					/**< Retrieve temperature from GPS */
	ADCS_CMD_GPS_ECUTOFF = 59, 				/**< Set the cutoff angle for recieving satellites */
	ADCS_CMD_GPS_GETPOS = 60,				/**< Get data from last GPS sample */
	ADCS_CMD_GPS_GETTIME = 61,				/**< Get the time from the last 1PPS */

	/* Sensor commands */
	ADCS_CMD_SENSOR_INIT = 70,			        /**< Perform an init on sensors 			*/
	ADCS_CMD_SENSOR_GET = 71,
	ADCS_CMD_SENSOR_CALIB = 72,

	/* Ephemeris commands */
	ADCS_CMD_EPHEM_NEW_TLE = 75,		  	        /**< Set new TLE from config				  */

	/* Actuator commands */
	ADCS_CMD_ACT_MW_GET_STAT = 80,			/**< Get current status of the wheels */
	ADCS_CMD_ACT_MW_SET = 81, 				/**< Set final speed and torque of a wheel*/
	ADCS_CMD_ACT_MW_EN = 82, 				/**< Enable/disable a wheel */
	ADCS_CMD_ACT_MW_ON = 83,					/**< Turn on the power switch for WDE */
	ADCS_CMD_ACT_MW_OFF = 84,				/**< Turn off the power switch for WDE */
	ADCS_CMD_ACT_MW_DIAG = 85,				/**< Run diagnostics on the wheels */

	ADCS_CMD_TRQ_GET = 90,
	ADCS_CMD_TRQ_SET = 91,

	ADCS_CMD_TRST_ON = 100,
	ADCS_CMD_TRST_OFF = 101,
	ADCS_CMD_TRST_PREHEAT = 102,
	ADCS_CMD_TRST_CONFIG = 103,
	ADCS_CMD_TRST_BURN = 104

} adcs_server_cmd_t;


/** Server reply types */
typedef enum __attribute__ ((packed)) {
	ADCS_REPLY_OK,						/**< No error 						*/
	ADCS_REPLY_NOENT, 					/**< No such entry 					*/
	ADCS_REPLY_INVAL, 					/**< Invalid argument 				*/
} adcs_server_reply_t;


/*
 * adcs server packet
 */
typedef struct  __attribute__ ((packed)) {
	adcs_server_cmd_t cmd;
	adcs_server_reply_t reply;
	union {

		union {
			adcs_startstop_t startstop;
			adcs_mode_t get_adcs_modes;
			adcs_state_t set_ads;
			adcs_state_t set_acs;
			adcs_state_t set_ephem;
		} state;

		union {
			adcs_hk_basic_t basic;
		} hk;

		union {
			adcs_act_torquers_t trq;
		} trq;

		union {
			adcs_act_mw_get_status_t mw_status;
			adcs_act_mw_set_t mw_set;
			adcs_act_mw_enable_t mw_enable;
			adcs_act_mw_diag_t mw_diag;
		} mw;

		union {
			gps_ascii_cmd_t cmd;
			adcs_float_t temperature;
			adcs_float_t cutoff_angle;
			adcs_gps_sample_t sample;
			adcs_gps_rxstatus_t rxstat;
			adcs_gps_last_sample_t lastsample;
			adcs_gps_last_pps_t lastpps;
		} gps;
	};
} adcs_server_packet_t;

#define ADCS_SERVER_PACKET_SIZE(t)			\
		(sizeof(((adcs_server_packet_t *) 0)->cmd)   +	\
				sizeof(((adcs_server_packet_t *) 0)->reply) +	\
				sizeof(t))

#endif /* ADCS_CLIENT_TYPES_H_ */
