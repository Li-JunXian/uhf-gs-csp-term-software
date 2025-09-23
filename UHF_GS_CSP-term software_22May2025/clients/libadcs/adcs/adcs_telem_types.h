#ifndef ADCS_TELEM_TYPES_H_
#define ADCS_TELEM_TYPES_H_

#define MAX_NUM_CSS 6
#define MAX_NUM_FSS 8

/* Type def for quaternion type defaulting to unit quaterion*/
typedef struct {
	float q1;	// xi
	float q2;	// yj
	float q3;	// zk
	float q4;	// w (real part)
} quaternion_t;

/****************************************************************************************/
/**
 * Structure used to contain the raw sensor data along with the current status
 */
typedef struct {
    /* Internal Mag */
	float magnetometer[3]; 					/**< The magnetometer measurementes [mGauss] */
	uint8_t magnetometerValid; 				/**< True if the magnetometer was measured during this iteration */
	float extmagnetometer[3]; 				/**< The magnetometer measurementes [mGauss] */
	float extmagnetometer_temp; 			/**< The external magnetometer temperature [degC] */
	uint8_t extmagnetometerValid; 			/**< True if the magnetometer was measured during this iteration */

	/* Coarse Sun Sensor */
	float sunsensor[6]; 					/**< The sunsensor measurements [bits 0-1023] */
	uint16_t sunsensor_raw[6]; 				/**< The raw sunsensor measurements [raw bits 0-1023] */
	uint8_t sunsensorValid; 				/**< True if the sunsensors were measured during this iteration */
	float sunsensorTemperatures[6]; 		/**< The raw sunsensor temperature measurements [raw bits 0-1023]*/

	/* Gyroscope */
	float gyro[3]; 							/**< The gyro measurements [deg/s]*/
	float gyro_norm[3]; 					/**< Norm of gyro measurement taken at 50,200 and 500 s intervals */
	float gyro_trend[3]; 					/**< Trend of gyro measurement (derivative of norm measurements)*/
	float gyro_accumulated[3];				/**< Accumulated trend (integrated derivative of the norm)*/
	float gyro_temp; 						/**< Temperature of gyro */
	uint8_t gyroValid; 						/**< True if the gyros were measured during this iteration */

	/* Fine Sun Sensor */
	float fss[MAX_NUM_FSS][3]; 				/**< Vectors from Fine sunsensors*/
	float fss_body[MAX_NUM_FSS][3];		    /**< Vectors from Fine sunsensors (in sensor frame)*/
	uint16_t fss_raw[MAX_NUM_FSS][4]; 		/**< Raw data from Fine sunsensors*/
	float fss_temp[MAX_NUM_FSS]; 			/**< Fine sunsensor temperatures */
	uint8_t fssValid[MAX_NUM_FSS]; 			/**< True if Fine sunsensors are valid*/

	/* GPS */
	float gps_pos[3]; 						/**< The gps position [m] in ECEF*/
	float gps_vel[3]; 						/**< The gps velocity [m/s] in ECEF*/
	float gps_veldeviation[3]; 				/**< Std. dev. of the GPS measurement*/
	float gps_posdeviation[3]; 				/**< Std. dev. of the GPS measurement*/
	uint32_t gps_unix; 						/**< GPS Epoch time [unit]*/
	double gps_julian; 						/**< GPS Julian date */
	double gps_tdiff_jd; 					/**< Difference in time between GPS and system */
	uint32_t gps_tdiff_ut; 					/**< Difference in time between GPS and system */
	uint8_t gps_valid;						/**< True if the gps measurements were collected during this iteration */
	uint8_t gps_sat;
	uint8_t gps_satsol;
	double gps_lastpps_jdate;				/**< Latest PPS, Julian date */
	uint32_t gps_lastpps_unix;				/**< Latest PPS, Unixtime*/

	/* Star Tracker */
	quaternion_t st1_qis;					/**< startracker quaternion inerital -> sensor */
	quaternion_t st2_qis;					/**< startracker quaternion inerital -> sensor */
	float st1_confidence; 					/**< Confidence in star tracker solution */
	float st2_confidence; 					/**< Confidence in star tracker solution */
	int8_t st1_valid;						/**< startracker valid */
	int8_t st2_valid;						/**< startracker valid */
} gs_adcs_measurementdata_t;

/**
 * Actuator data
 */
typedef struct {
	float   torquerDutyCycle[3];            /**< The torquer duty cycle requested by controller (in Body frame) */
	float   torquerDutyCycleOut[3];         /**< The actual torquer duty cycle applied to torquers (limited and mapped) */
	float 	torquerTempScale[3]; 	        /**< Temperature compensation for the torquers */
	float	wheelTorque[4];                 /**< Assigned wheel torque */
	float	wheelMomentum[4];               /**< Assigned wheel momentum */
	float	wheelSpeed[4];                  /**< Assigned wheel speed */
	uint8_t wheelEnable[4];                 /**< Wheel status in WDE */
} gs_adcs_actuatordata_t;


/****************************************************************************************/
/**
 * Structure used to contain status for the ADCS
 */

/**
 * Control mode enumeration
 * @note: These types are used in libadcs too
 */
// SPHINX: CONTROL_MODE_BEGIN
typedef enum __attribute__ ((packed)) acs_mode_t{
	ACS_IDLE, 	        /**< 0 ACS idle mode */
	ACS_CONST, 	        /**< 1 ACS constant actuation mode */
	ACS_BDOT, 	        /**< 2 ACS Bdot mode, i.e. detumbling mode */
	ACS_NADIR, 	        /**< 3 ACS LQR Nadir pointing mode */
	ACS_SMC,	        /**< 4 ACS Torquer only Sliding Mode Controller */
	ACS_YSPIN,	        /**< 5 ACS Nadir Y spin / momentum bias mode */
	ACS_WHEELS,	        /**< 6 ACS Using 3 or 4 wheels */
	ACS_SM,		        /**< 7 ACS Safe Mode */
	ACS_INERTIAL, 	    /**< 8 ACS Inertial pointing mode */
	ACS_LOWPOWER,	    /**< 9 ACS Low Power mode, momentum transfer to one RW */
} acs_mode_t;
//SPHINX: CONTROL_MODE_END

/**
 * Attitude determination mode enumeration
 * @note: These types are used in libadcs too
 */
typedef enum __attribute__ ((packed)) ads_mode_t{
	ADS_IDLE, 	       	/**< ADS idle mode */
	ADS_SENSORS,		/**< Sensors only mode */
	ADS_QMETHOD,	    /**< ADS Q-method */
	ADS_KALMANFILTER,	/**< ADS kalman filter estimation mode */
} ads_mode_t;

/**
 * Ephemeris mode enumeration
 * @note: These types are used in libadcs too
 */
typedef enum __attribute__ ((packed)) ephem_mode_t{
	EPHEM_IDLE,		    /**< Ephemeris idle mode */
	EPHEM_SGP4,		    /**< Ephemeris sgp4 propagation */
	EPHEM_GPS		    /**< Epehmeris gps propagation */
} ephem_mode_t;

/**
 * Used at mode selection in the y-spin controller
 */
typedef enum __attribute__ ((packed)) yspin_mode_t {
    YSPIN_IDLE,                 /**< Idle mode	*/
    YSPIN_ECLIPSE,              /**< Satellite in Eclipse */
    YSPIN_ALIGN,                /**< Y axis alignment */
    YSPIN_SPINUP,               /**< Spin around Y axis */
    YSPIN_MWACTIVATION,         /**< Go from Y axis spin to MW ctrl */
    YSPIN_MWCTRL,               /**< MW spinning, satellite pointing */
} yspin_mode_t;

typedef enum __attribute__ ((packed)) lowpower_mode_t {
    LOWPOWER_IDLE,              /**< Idle mode	*/
    LOWPOWER_ALIGN,             /**< Rotate the satellite resulting in one RW is pointing in orbit normal */
    LOWPOWER_BIAS,              /**< Bias the angular momentum in the direction of the orbit normal */
    LOWPOWER_MOMENTUMTRANS,     /**< Transfer net angular momentum to wheel axis in orbit normal */
    LOWPOWER_UNICTRL,           /**< One wheel Pitch control */
} lowpower_mode_t;

typedef struct {
    /* IGRF status 0=OK, -1=Error */
    int8_t igrf;
    /* SGP4 status:      0 - OK
     *                  -1 - mean elements, ecc >= 1.0 or ecc < -0.001 or a < 0.95 er
     *                  -2 - mean motion less than 0.0
     *                  -3 - pert elements, ecc < 0.0  or  ecc > 1.0
     *                  -4 - semi-latus rectum < 0.0
     *                  -5 - epoch elements are sub-orbital
     *                  -6 - satellite has decayed */
    int8_t sgp4;
    /* J2 element status:0 - OK
     *                  -1 - input position vector not valid (below earth surface)
     *                  -2 - input velocity vector not valid (faster than earth surface or slower than 10,000 km)
     *                  -3 - J2 elements invalid because input too old */
    int8_t j2elem;
    /* J2 propagation status: 0 - OK
     *                  -1 - could not solve for true anomaly within specified iterations
     *                  -2 - output position vector not valid (below earth surface)
     *                  -3 - output velocity vector not valid (faster than earth surface or slower than 10,000 km) */
    int8_t j2prop;
    /* Time Error:      0=OK
     *                  -1=time is too early */
    int8_t time;
    /* Overall status:  0=OK
     *                  -1=ephemeris is NOT valid */
    int8_t status;
} gs_adcs_ephem_status_t;

typedef struct {
	/* Magnetometer status:         0=OK
	 *                              -1=mag read error
	 *                              -2=calib index error
	 *                              -3=value error*/
	int8_t mag;
	/* External magnetometer status: 0=OK
	 *                              -1=mag read error
	 *                              -2=calib index error
	 *                              -3=value error*/
	int8_t extmag;
	/* Coarse sunsensor status:     0=OK  */
	int8_t sun;
	/* Fine sunsensor status:       0=OK  */
	int8_t fss[MAX_NUM_FSS];
	/* Gyro status:                 0=OK
	 *                              -1=Not OK */
	int8_t gyro;
	/* GPS status:                  1=powered
	 *                              0=unpowered */
	int8_t gps;
	/* HTPA status:                 0=OK
	 *                              -1=Error*/
	int8_t htpa[2];
} gs_adcs_sensor_status_t;

typedef struct {
	/**< Current sensor status  */
	gs_adcs_sensor_status_t sensorStatus;
	/**< Current bdot status. 1:tumble2detumble  0:valid,  -1:no sample,  -2:no previous sample  -3:detumble2tumble */
	int8_t bdotStatus;
	/**< Current UKF status. 0=OK, -1=nan in estimated state, -2=nan in measurement, -3=wrong array sizes  */
	int8_t ukfStatus;
	/**< Current ephemeris status.  */
	gs_adcs_ephem_status_t ephemStatus;
	/**< Current parameter file status: 2=AutoWrittenFile, 1=UploadedFile, 0=Problem in uploaded file, using partly defaults */
	//int8_t paramStatus;
	/**< Current run status on ADCS. 0=stopped, 1=running, -1=dead*/
	int8_t runStatus;
	/**< Average ADCS loop time in ms (over 50 runs)*/
	uint16_t looptime;
	/**< Maximum ADCS loop time in ms (over the last 50 runs) */
	uint16_t maxlooptime;
	/**< Current status on controllers */
	int8_t ctrlStatus;
} adcs_status_t;

typedef struct {
	/**< adcs code points */
	uint8_t task;
	uint8_t main;
} adcs_debug_t;

typedef struct {
	/**< adcs timing points */
	uint32_t ephem_time;
	uint32_t ads_time;
	uint32_t acs_time;
	uint32_t sensor_time;
} adcs_timing_t;


/**
 * Current and desired determination and control modes
 */
/**
 * Used to determine mode selection within controllers
 */
typedef struct {
	yspin_mode_t yspin_mode;            /**<  Yspin control modes	*/
	lowpower_mode_t lowpower_mode;      /**<  Lowpower control modes	*/
}acs_submode_t;

typedef struct {
    acs_mode_t acs_mode;                /**< Current control mode */
    acs_mode_t desired_acs_mode;        /**< Desired control mode if possible */
    ads_mode_t ads_mode;                /**< Current determination mode */
    ads_mode_t desired_ads_mode;        /**< Desired determination mode if possible */
    ephem_mode_t desired_ephem_mode;    /**< Desired ephemeris mode if possible */
    ephem_mode_t ephem_mode;            /**< Current ephemeris mode */
    acs_submode_t acs_submode;          /**< Submode in Yspin and LP mode */
} adcs_mode_t;

typedef struct {
    float rate[2];
    float diffmag[3];
    float torquerduty[3];
    uint8_t detumbled;                  /**< ADCS Detumbled, 1=True */
} gs_adcs_bdot_data_t;

/****************************************************************************************/
// UKF data struct
//---------------------------------------------------------------------------
/**
 * Define maximum sizes for arrays in client/server packets
 */
#define UKF_MAX_NUM_X 13				// Number of error states - real part of quaternion is not included!
#define UKF_MAX_NUM_ERR 12				// Number of error states - real part of quaternion is not included!
#define UKF_MAX_NUM_Z 13				// z = [v_sun v_mag omega_gyro v_fss v_startrack]
#define UKF_MAX_NUM_SENS 40				// 6xsunsensor,3xmag,3xgyro,3*8*fss, 3xstartracker
#define MAX_NUM_SP (UKF_MAX_NUM_ERR*2+1)

// NOTE: All UKF vars are declared of MAX length, but may be used with smaller length depending on config.
typedef struct {
    // Statevector:
    // [q1 q2 q3 q4 w1 w2 w3 bm1 bm2 bm3 bg1 bg2 bg3]
    // [0  1  2  3  4  5  6  7   8   9   10  11  12 ]
    float EstimatedState[UKF_MAX_NUM_X];    // State vector estimated by UKF (X)
    float q[4];                             // Quaterion (duplicate from estimated state
    float w[3];                             // rate (duplicate from estimated state)
    float Pdiag[UKF_MAX_NUM_ERR];           // Covariance matrix diagonal
    float Xpred[UKF_MAX_NUM_X];             // Copy of predicted state
    float Zpred[UKF_MAX_NUM_Z];             // Copy of predicted measurement
    float sunsensorMax[6];                  // Current maximum value for the sunsensor max tracking algorithm
    uint8_t InEclipse;                      // UKF eclipse detection
    int8_t KFchoice;                        // Which KF is running ?
} GS_ADCS_UKF_Data_t;

// UKF measurement struct
//---------------------------------------------------------------------------
typedef struct {
    float Measurement[UKF_MAX_NUM_SENS];    // Sensor vector
    float MeasurementFilt[UKF_MAX_NUM_Z];   // Filtered sensorVector (Z) [sun1 sun2 sun3 mag1 mag2 mag3 g1 g2 g3 fss1 fss2 fss3]
    uint8_t Enable[UKF_MAX_NUM_Z];          // Enable vector for measurents (set to zero to disable sensor)
} GS_ADCS_UKF_Measurement_t;

// UKF input struct
//---------------------------------------------------------------------------
typedef struct {
    float CtrlTorques[3];                   // control torque vector
    float CtrlMomentum[3];                  // control momentum vector
    float Rate[3];                          // Rate measurement
} GS_ADCS_UKF_Inputs_t;

/****************************************************************************************/
// Ephemeris
//---------------------------------------------------------------------------
typedef struct {
    double julianDate;
    float scRECI[3];            // [m] Space craft pos in ECI
    float scVECI[3];            // [m/s] Space craft vel in ECI
    float sunECI[3];            // Sun vector
    float magECI[3];            // Magnetic field [nT]
    float dmagECI[3];           // Derivative of Magnetic field [nT]
    quaternion_t rotationIE;    // Rotation from ECI to ECEF frame
    int timeSinceEclipse;       // Number of samples since satellite came out of eclipse - if 0 then we're in eclipse. Max value: 10.000
    quaternion_t rotationIO;    // Rotation from ECI to orbit frame
    quaternion_t rotationIL;    // Rotation from ECI to LVLH frame
    float rateIO[3];            // Rotation rate from ECI to orbit frame
    float rateIL[3];            // Rotation rate from ECI to LVLH frame
} gs_adcs_ephem_t;

// defines the orbit elements at a given epoch
typedef struct {
    double epoch;               // [julianDate] epoch
    float pos_ECI[3];           // [km] ECI position vector translated from GPS used to generate the elements
    float vel_ECI[3];           // [km] ECI position vector translated from GPS used to generate the elements
    float ecc;                  // eccentricity
    float a;                    // [km] semi-major axis
    float n;                    // [rad/s] mean motion
    float p;                    // [km] parameter
    float incl;                 // [rad] inclination
    float raan;                 // [rad] right ascension of the ascending node
    float argPer;               // [rad] argument of perigee
    float trueAnom;             // [rad] true anomaly
    float eccAnom;              // [rad] eccentric anomaly
    float meanAnom;	            // [rad] mean anomaly
} gs_adcs_elem_t;



/****************************************************************************************/
// CTRL
//---------------------------------------------------------------------------
// Type def for controller
typedef struct {
    quaternion_t ref_q;         // Reference quaternion
    quaternion_t err_q;         // Error quaternion
    quaternion_t ierr_q;        // Integral error quaternion
    float err_rate[3];          // Error rate [rad/s}
    float M[3];                 // Control signal, magnetic moment [Am^2]
    float mw_torque[4];         // Control signal, Wheel torque
    float mw_speed[4];          // Wheel speed
    float mw_momentum[4];       // Control signal, Wheel momentum
    float ref_rate[3];          // Y-Thomson spin rate reference
    float euler_offset[3];      //RAM mode euler rotation offset [x y z]
    float offset_err[3];        // Ram error i Euler angles
    float bodyTorque[3];        // Torque at bodyframe axes
    float bodyMomentum[3];      // Momentum at bodyframe axes
} gs_adcs_ctrl_t;
/****************************************************************************************/

typedef struct {
    int16_t raw0[16 * 2];
    int16_t raw1[16 * 2];
    int16_t ptat;
    int16_t ptat_comp;
} gs_adcs_htpa_t;


/****************************************************************************************/


/**
 * Telemetry tables
 */
typedef struct {
    gs_adcs_measurementdata_t sensordata;
    gs_adcs_actuatordata_t actdata;
} gs_adcs_telem1_t;

typedef struct {
    adcs_mode_t mode;
    adcs_status_t status;
    gs_adcs_bdot_data_t bdot;
    adcs_debug_t debug;
} gs_adcs_telem2_t;

typedef struct {
    GS_ADCS_UKF_Data_t ukfdata;
    GS_ADCS_UKF_Measurement_t ukfmeas;
    GS_ADCS_UKF_Inputs_t ukfinput;
} gs_adcs_telem3_t;

typedef struct {
    gs_adcs_ephem_t ephem;
    gs_adcs_elem_t elem;
} gs_adcs_telem4_t;

typedef struct {
    gs_adcs_ctrl_t ctrldata;
} gs_adcs_telem5_t;

typedef struct {
    gs_adcs_htpa_t htpa0;
    gs_adcs_htpa_t htpa1;
} gs_adcs_telem6_t;

typedef struct {
    adcs_timing_t timing;
} gs_adcs_telem7_t;

#endif /* ADCS_TELEM_TYPES_H_ */
