#ifndef ADCS_PARAM_TYPES_H_
#define ADCS_PARAM_TYPES_H_
#include <inttypes.h>
#include "adcs_types.h"

/*
 * Configuration parameters for peripherals
 * sensors and actuators
 */
#define FSS_THETA_POINTS 6

typedef struct {
    uint8_t add;        /**< GSSB address */
    uint8_t node;       /**< CSP node, 0=localhost */
    float p[4];         /**< h, x0, y0, theta  */
    float q[4];         /**< Quaternion */
    uint8_t Cvalid;     /**< Used looktable 0=no, 1=yes */
    float Ctheta[81];   /**< Correction array */
    float Cphi[81];     /**< Correction array */
} gs_adcs_config_fss_t;

typedef struct {
    uint8_t num;        /**< number of fine sunsensors */
    float darkth;       /**< Sum of sensors darkness threshold */
    float idarkth;      /**< Indivudial sensor dark threshold */
    float brightth;     /**< Individual sensor threshold */
} gs_adcs_config_fssgeneral_t;

typedef struct {
    float offset[3];    /**< Magnetometer offset vector [x y z] */
    float scale[3];     /**< Magnetometer scale vector [x y z] */
    float rotate[9];    /**< Magnetometer scale rotation matrix */
    float temp[3];      /**< Magnetometer temperature calib [x y z] */
    float maxfield;     /**< maximum expected field strength in nT */
    int8_t axis[3];     /**< Magnetometer axis vector [x y z] */
    int8_t sign[3];     /**< Magnetometer sign vector [x y z] */
    int8_t enable[3];   /**< Magnetometer enable vector (0=disable, 1=enable) [+x +y +z] */
    int8_t gain;
    int8_t rate;
    int8_t add;
} gs_adcs_config_mag_t;

typedef struct {
    float scale[3];     /**< Gyro scaling [x y z] */
    float offset[3];    /**< Gyro offset [x y z]  */
    float temp[3];      /**< Gyro temperature calib [x y z] */
    int8_t sign[3];     /**< Gyro sign vector [x y z] */
    int8_t axis[3];     /**< Gyro axis vector [x y z] */
    int8_t enable[3];   /**< Gyro enable vector (0=disable, 1=enable) [+x +y +z] */
} gs_adcs_config_gyro_t;

typedef struct {
    float scale[6];     /**< Sunsensor scale vector [+x +y +z -x -y -z] */
    float offset[6];    /**< Sunsensor offset vector [+x +y +z -x -y -z] */
    float temp[6];      /**< Sunsensor temperature calib [+x +y +z -x -y -z] */
    int8_t axis[6];     /**< Sunsensor axis vector  [+x +y +z -x -y -z] */
    int8_t enable[6];   /**< Sunsensor enable vector (0=disable, 1=enable) [+x +y +z -x -y -z] */
} gs_adcs_config_css_t;

typedef struct {
    uint8_t node[2];    /**<htpa node */
    uint8_t enable[2];  /**< htpa enables */
} gs_adcs_config_htpa_t;

typedef struct {
    quaternion_t st1align;  /**< Alignment quaterion q_startracker->body */
    quaternion_t st2align;  /**< Alignment quaterion q_startracker->body */
    uint8_t enable[2];      /**< Startracker enables */
    float confidence;
} gs_adcs_config_star_t;

typedef struct {
    int8_t enable;      /** Power the GPS Unit. Always in idle or SGP4 mode, and based on onDur/offDur in J2 ephem mode*/
    float onDur;        /** [sec] continuous duration GPS powered. */
    float offDur;       /** [sec] continuous duration GPS unpowered. */
    float propDur;      /** [sec] GPS propagation duration. After this duration, the ephem_mode
            will switch to SGP4 until new GPS data is collected. */
} gs_adcs_config_gps_t;

typedef struct {
    float am[3];            /**< Dipole moment of torquers */
    uint8_t axis[3];        /**< Torquer axis vector [x y z] */
    int8_t sign[3];         /**< Sign of torquers (+-1)[x y z] */
    uint8_t max[3];         /**< In percent [x y z]*/
    float mt_dip_eff[3];    /**< Torquer constant */
} gs_adcs_config_torquer_t;

typedef struct {
    float max_speed[4];     /**< Maximum speed of the momentum wheels */
    float max_torque[4];    /**< Maximum torque to be produced by the wheels */
    float inertia[4];       /**< Spin-axis inertia of each momentum wheel */
    float invInertia[4];    /**< Inverted spin-axis inertia of each momentum wheel */
    float layout[3 * 4];    /**< Layout matrix from MW orientation to body axes  */
    float invLayout[4 * 3]; /**< Pseudoinverse of the wheel layout */
    float fault[4 * 4];     /**< Wheel activation. 0 = deactivated, 1 = activated */
    uint8_t mw_override;    /**< Override command to force WDE on / off */
} gs_adcs_config_mw_t;

typedef struct {
    gs_adcs_config_mag_t mag;
    gs_adcs_config_mag_t extmag;
    gs_adcs_config_gyro_t gyro;
    gs_adcs_config_css_t css;
    gs_adcs_config_fssgeneral_t fss;
    gs_adcs_config_gps_t gps;
    gs_adcs_config_htpa_t htpa;
    gs_adcs_config_star_t star;
} gs_adcs_config_sensor_t;

typedef struct {
    gs_adcs_config_torquer_t torquer;
    gs_adcs_config_mw_t mw;
} gs_adcs_config_act_t;

typedef struct {
    gs_adcs_config_sensor_t sensor;
    gs_adcs_config_act_t act;
} gs_adcs_config_periph_t;

/*
 * Configuration parameters for adcs guidance, navigation and control
 * sensors and actuators
 */

typedef struct {
    float inertia[3];           /**< Satellite inertia along the three principal axes */
} gs_adcs_config_sc_t;

typedef struct {
    uint8_t acs_init;           /**< Desired control mode if possible */
    uint8_t ads_init;           /**< Desired determination mode if possible */
    uint8_t ephem_init;         /**< Desired ephemeris mode if possible */
} gs_adcs_config_mode_t;

typedef struct {
    uint16_t sampletime;        /**< The ADCS sample time in ms  */
    uint16_t kf_divider;        /**< How often should the UKF be run as a multiplum of ADCS iterations*/
    uint16_t qmethod_divider;   /**< How often should the q-method be run as a multiplum of ADCS iterations*/
    uint16_t ephem_divider;     /**< How often should the ephemeris calculations be run as a multiplum of ADCS iterations*/
    uint16_t bdot_divider;      /**< How often should the bdot be run as a multiplum of ADCS iterations */
    uint16_t ctrl_divider;      /**< How often should the tracking controller be run as a multiplum of ADCS iterations */
    uint16_t pwm_deadtime;      /**< How long the pwm should be quiet for during each run */
    uint16_t mt_calib_div;      /**< How often should the torquers be recalibrated due to temperature changes */
} gs_adcs_config_ts_t;

typedef struct {
    float tconst;               /**< coefficient of averaging for BDOT filter - shall be between 0 and 1 */
    float tconst2;              /**< coefficient of averaging for rate norm filter */
    float tsep;                 /**< Time seperation between fast and slow filters, must be larger than 1*/
    float threshold[3];         /**< actuation thresholds nT/s - for each axis */
    float torquerlimit[3];      /**< maximum actuation signal allowed on each torquer axis */
    uint8_t baxis;              /**< on which axis is the bias 0=x, 1=y, 2=z */
    uint8_t bstrength;          /**< 0-100 bias strength */
    uint8_t bsign;              /** direction, -1 or 1 */
    uint8_t magchoice;          /** 0=internal magnetometer, 1=External magnetometer */
    float rateref[3];           /** Reference rate to the bdot [mG/s] */
} gs_adcs_config_bdot_t;

typedef struct {
    float css_maxinit;          /**< Current maximum value for the sunsensor in the sunsensor max tracking algorithm */
    float css_absmax;           /**< Maximum permissible value for the sunsensor in the sunsensor max tracking algorithm */
    float css_th;               /**< Threshold for dark (eclipse) detection */
    float Q_quat;
    float Q_rate;
    float Q_magbias;
    float Q_gyrobias;
    float Q_dtorque;
    float R_css;
    float R_mag;
    float R_gyro;
    float R_fss;
    float R_st;
    float P_quat;
    float P_rate;
    float P_magbias;
    float P_gyrobias;
    float P_dtorque;
    uint8_t nochange_disable;   /**< Enables/Disables that no change in a sensor measurement will disable it in the meas update 0/1 */
    uint8_t enable_ctrltorque;  /**< Enables/Disables the use of control torque in UKF model 0/1 */
    uint8_t enable_fss;         /**< Enables/Disables the use of fss in the UKF 0/1 */
    uint8_t enable_startracker; /**< Enables/Disables the use of the Star Tracker in the UKF */
    uint8_t enable_maxcur;      /**< Enables/Disables the max-current algorithm 0/1 */
    uint8_t normmag;            /**< 0=Do not normalize mag, 1=normalize mag */
    uint8_t normmagbias;        /**< 0=Do not normalize mag bias, 1=normalize mag bias */
    uint8_t magchoice;          /**< 0=internal magnetometer, 1=External magnetometer */
    uint8_t intchoice;          /**< Integrator choice, 0=euler, 1=RK2, 2=RK4 */
    uint8_t kfchoice;           /**< 0=gyro model 1=dynamic model */
    float M_bias[3];            /**< Magnetic field bias (body axis) [A/Mm^2] */
    float min_angle;            /**< Minimum accepted angle between magnetic field and sun [deg] */
} gs_adcs_config_kf_t;

#define MAX_ADCS_TLE_LINE 70      // Length of TLE line
typedef struct {
    char tleline1[MAX_ADCS_TLE_LINE];
    char tleline2[MAX_ADCS_TLE_LINE];
    uint8_t teme2eci;           /**< 0=Do not do teme2eci conversion, 1=Do teme2eci conversion */
} gs_adcs_config_sgp4_t;

typedef struct {
    float sunmax;
    float sunth;
    uint8_t magchoice;          /** 0=internal magnetometer, 1=External magnetometer */
    uint8_t enablefss;          /**< Enables/Disables the use of fss in the qmethod 0/1 */
} gs_adcs_config_qmethod_t;

typedef struct {
    float detumbl2point;        /**< Threshold for going from detumbling mode to pointing mode */
    float point2detumbl;        /**< Threshold for going from pointing mode to detumbling mode */
    float const_ctrl[3];        /**< Constant dutycycle for the three magnetorquers */
    uint8_t through_eclipse;    /**< 1=Do control through eclipse, 0=do not control through eclipse*/
    uint16_t eclipse_delay;     /**< ADCS iterations to wait before trusting attitude estimate and start actuating. */
    float mt_smc[2];            /**< Sliding Mode Controller gain for MT, [Ks, Lambda]. */
    float mw_smc[3];            /**< Sliding Mode Controller gain for MW, [Ks, Lambda, epsilon]. */
    float mw_gain[3];           /**< Gain used by the wheel controller [Common, K1, K2]. */
    float mw_dumpgain;          /**< Momentum dunping sign and gain*/
    float mw_mw2trq;            /**< Option to reverse and scale mw 2 torquer assign */
    float euler_offset[3];      /**< Pointing offset between orbit- and body frame. */
    float rate_offset[3];       /**< Rate offset between orbit- and body frame */
    float ref_momentum[3];      /**< Body axis momentum reference [Nms] */
    float grond_station_list[9];/** List of ECEF coordinates of 3 ground stations [GS1] [GS2] [GS3] */
    float gso_list[9];          /**< Landmark ECEF [ECEF1 ECEF2 ECEF3]*/
    float gso_ant[9];           /**< Antenna offset in body frame Euler angles [Ant1 Ant2 Ant3]*/
    uint8_t en_dist_comp;       /**< Enabling of disturbance compensation in ctrl.*/
    uint8_t en_mwdump;          /**< Enable MW momentum dumping */
    uint8_t en_bbq[2];          /**< BBQ parameters, [Enable, Ref. speed]*/
    uint8_t en_gso[3];          /**< Enable/select which landmark to track, the antenna to use and coordinate system [gso, ant, coord] */
    uint8_t en_gstrack;         /**< Mode used to track ground stations when they are close and else fly RAM */
    uint8_t en_sunpoint;        /**< mode to change the reference vector to point at the sun */
} gs_adcs_config_ctrl_t;

typedef struct {
    float nadir[3];             /**< 3-2-1 Euler rotation from satellite to orbit */
    float ram[3];               /**< 3-2-1 Euler rotation from satellite to orbit */
    float mindrag[3];           /**< 3-2-1 Euler rotation from satellite to orbit */
    float maxdrag[3];           /**< 3-2-1 Euler rotation from satellite to orbit */
    float custom0[3];           /**< 3-2-1 Euler rotation from satellite to orbit */
    float custom1[3];           /**< 3-2-1 Euler rotation from satellite to orbit */
    float custom2[3];           /**< 3-2-1 Euler rotation from satellite to orbit */
    float target0[3];           /**< ECEF Landmark to track. [x y z] meter */
    float target1[3];           /**< ECEF Landmark to track. [x y z] meter */
    float target2[3];           /**< ECEF Landmark to track. [x y z] meter */
} gs_adcs_config_reference_t;

typedef struct {
    float dumpgain[3];          /**< Gain for the momentum dumping [Common, Quaternion, Angular velocity] */
    float mwgain[3];            /**< Gains for the momentumwheel [Kx, Ky, Kz] */
    float mtgain[4];            /**< Gains for the magnetorquers [Kd, Ks] */
    float ref_rate;             /**< Target rate at initial spin with torquers */
    float ref_mnt;              /**< The desired momentum for the momentum wheel */
    float trans[6];             /**< Transition values  */
    float pd_ctrl[2];           /**< Magnetorquer PD control gain [k_p k_d]*/
} gs_adcs_config_yspin_t;

// These are parameter you are not allowed to set, the satellite code will calculate them.
typedef struct {
    float inv_inertia[3];       /**< The inverse of the satellites inertias - defined for ease of computation */
    float mag_rot_trans[9];     /**< Magnetometer scale rotation matrix (transposed) */
    float extmag_rot_trans[9];  /**< Magnetometer scale rotation matrix (transposed) */
    float torquer_scale;        /**< Scale factor for for torquer duty cycle, necessary because of pwm dead time */
    uint8_t torquer_sat[3];     /**< Saturation value of torquers */
} gs_adcs_config_work_t;

typedef struct {
    gs_adcs_config_mode_t mode;
    gs_adcs_config_ts_t ts;
    gs_adcs_config_sc_t sc;
    gs_adcs_config_bdot_t bdot;
    gs_adcs_config_sgp4_t sgp4;
    gs_adcs_config_kf_t kf;
    gs_adcs_config_qmethod_t qmethod;
    gs_adcs_config_yspin_t yspin;
    gs_adcs_config_work_t work;
    gs_adcs_config_ctrl_t ctrl;
    gs_adcs_config_reference_t set;
} gs_adcs_config_gnc_t;

/**
 * Configuration struct, which holds all basic configuration parameters for the GS_ADCS system
 */
typedef struct {
    gs_adcs_config_fss_t *fss[MAX_NUM_FSS];
    gs_adcs_config_periph_t *periph;
    gs_adcs_config_gnc_t *gnc;
} gs_adcs_config_t;

extern gs_adcs_config_t gs_adcs_config;

#endif /* ADCS_PARAM_TYPES_H_ */
