#ifndef ADCS_PARAM_H_
#define ADCS_PARAM_H_

#include <stddef.h>
#include <param/param.h>
#include <adcs/adcs_types.h>
#include <adcs/adcs_param_types.h>
#include <adcs/adcs_telem.h>


/**
 * The PARAM ADCS define is used by libADCS to get/set variables
 * The value is fixed and you should therefore avoid using param table 7-18
 * anywhere else than libADCS.
 */
// SPHINX: TABLE CONFIG START
#define PARAM_ADCS_GNC		8
#define FRAM_FNO_ADCS_GNC	8
#define FRAM_FNO_ADCS_GNC_PERSIST		4
#define FRAM_FNO_ADCS_GNC_PERSIST_ADDR (4 * PARAM_MAX_FILESIZE)


#define PARAM_ADCS_PERIPH	9
#define FRAM_FNO_ADCS_PERIPH	9

// Fine sunsensor param structs (limited to 8)
#define PARAM_ADCS_FSS(i)	10+i
#define FRAM_FNO_ADCS_FSS(i)	10+i
// SPHINX: TABLE CONFIG END

/**
 * Define memory space
 */

#define ADCS_PARAM_GNC_MEMSIZE			0x400 - 5
#define ADCS_PARAM_PERIPH_MEMSIZE		0x400 - 5
#define ADCS_PARAM_FSS_MEMSIZE			0x400 - 5

#define ADCS_PARAM_FSS_ADD              offsetof(gs_adcs_config_fss_t,add)
#define ADCS_PARAM_FSS_NODE             offsetof(gs_adcs_config_fss_t,node)
#define ADCS_PARAM_FSS_Q(i)             offsetof(gs_adcs_config_fss_t,q) + ((i) * sizeof(float))
#define ADCS_PARAM_FSS_P(i)             offsetof(gs_adcs_config_fss_t,p) + ((i) * sizeof(float))
#define ADCS_PARAM_FSS_CVALID           offsetof(gs_adcs_config_fss_t,Cvalid)
#define ADCS_PARAM_FSS_CTHETA0(i)       offsetof(gs_adcs_config_fss_t,Ctheta) + ((i) * sizeof(float))
#define ADCS_PARAM_FSS_CTHETA1(i)       offsetof(gs_adcs_config_fss_t,Ctheta) + 27*sizeof(float) + ((i) * sizeof(float))
#define ADCS_PARAM_FSS_CTHETA2(i)       offsetof(gs_adcs_config_fss_t,Ctheta) + 27*2*sizeof(float) + ((i) * sizeof(float))
#define ADCS_PARAM_FSS_CPHI0(i)         offsetof(gs_adcs_config_fss_t,Cphi) + ((i) * sizeof(float))
#define ADCS_PARAM_FSS_CPHI1(i)         offsetof(gs_adcs_config_fss_t,Cphi) + 27*sizeof(float) + ((i) * sizeof(float))
#define ADCS_PARAM_FSS_CPHI2(i)         offsetof(gs_adcs_config_fss_t,Cphi) + 27*2*sizeof(float) + ((i) * sizeof(float))

#define ADCS_PARAM_ST_ST1ALIGN(i) 		offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, star) + offsetof(gs_adcs_config_star_t,st1align) + ((i) * sizeof(float))
#define ADCS_PARAM_ST_ST2ALIGN(i) 		offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, star) + offsetof(gs_adcs_config_star_t,st2align) + ((i) * sizeof(float))
#define ADCS_PARAM_ST_ENABLE(i) 		offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, star) + offsetof(gs_adcs_config_star_t,enable) + ((i) * sizeof(uint8_t))
#define ADCS_PARAM_ST_CONF		 		offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, star) + offsetof(gs_adcs_config_star_t,confidence)
#define ADCS_PARAM_MAG_OFFSET(i)		offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, mag) + offsetof(gs_adcs_config_mag_t,offset) + ((i) * sizeof(float))
#define ADCS_PARAM_MAG_SCALE(i)			offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, mag) + offsetof(gs_adcs_config_mag_t,scale) + ((i) * sizeof(float))
#define ADCS_PARAM_MAG_ROTATE(i)		offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, mag) + offsetof(gs_adcs_config_mag_t,rotate) + ((i) * sizeof(float))
#define ADCS_PARAM_MAG_TEMP(i)			offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, mag) + offsetof(gs_adcs_config_mag_t,temp) + ((i) * sizeof(float))
#define ADCS_PARAM_MAG_AXIS(i)			offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, mag) + offsetof(gs_adcs_config_mag_t,axis) + ((i) * sizeof(int8_t))
#define ADCS_PARAM_MAG_SIGN(i)			offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, mag) + offsetof(gs_adcs_config_mag_t,sign) + ((i) * sizeof(int8_t))
#define ADCS_PARAM_MAG_ENABLE(i)		offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, mag) + offsetof(gs_adcs_config_mag_t,enable) + ((i) * sizeof(int8_t))
#define ADCS_PARAM_MAG_GAIN				offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, mag) + offsetof(gs_adcs_config_mag_t,gain)
#define ADCS_PARAM_MAG_RATE				offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, mag) + offsetof(gs_adcs_config_mag_t,rate)
#define ADCS_PARAM_MAG_ADD				offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, mag) + offsetof(gs_adcs_config_mag_t,add)
#define ADCS_PARAM_MAG_MAXFIELD			offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, mag) + offsetof(gs_adcs_config_mag_t,maxfield)
#define ADCS_PARAM_EXTMAG_OFFSET(i)		offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, extmag) + offsetof(gs_adcs_config_mag_t,offset) + ((i) * sizeof(float))
#define ADCS_PARAM_EXTMAG_SCALE(i)		offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, extmag) + offsetof(gs_adcs_config_mag_t,scale) + ((i) * sizeof(float))
#define ADCS_PARAM_EXTMAG_ROTATE(i)		offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, extmag) + offsetof(gs_adcs_config_mag_t,rotate) + ((i) * sizeof(float))
#define ADCS_PARAM_EXTMAG_TEMP(i)		offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, extmag) + offsetof(gs_adcs_config_mag_t,temp) + ((i) * sizeof(float))
#define ADCS_PARAM_EXTMAG_AXIS(i)		offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, extmag) + offsetof(gs_adcs_config_mag_t,axis) + ((i) * sizeof(int8_t))
#define ADCS_PARAM_EXTMAG_SIGN(i)		offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, extmag) + offsetof(gs_adcs_config_mag_t,sign) + ((i) * sizeof(int8_t))
#define ADCS_PARAM_EXTMAG_ENABLE(i)		offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, extmag) + offsetof(gs_adcs_config_mag_t,enable) + ((i) * sizeof(int8_t))
#define ADCS_PARAM_EXTMAG_GAIN			offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, extmag) + offsetof(gs_adcs_config_mag_t,gain)
#define ADCS_PARAM_EXTMAG_RATE			offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, extmag) + offsetof(gs_adcs_config_mag_t,rate)
#define ADCS_PARAM_EXTMAG_ADD			offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, extmag) + offsetof(gs_adcs_config_mag_t,add)
#define ADCS_PARAM_EXTMAG_MAXFIELD		offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, extmag) + offsetof(gs_adcs_config_mag_t,maxfield)
#define ADCS_PARAM_GYRO_OFFSET(i)		offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, gyro) + offsetof(gs_adcs_config_gyro_t,offset) + ((i) * sizeof(float))
#define ADCS_PARAM_GYRO_SCALE(i)		offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, gyro) + offsetof(gs_adcs_config_gyro_t,scale) + ((i) * sizeof(float))
#define ADCS_PARAM_GYRO_TEMP(i)			offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, gyro) + offsetof(gs_adcs_config_gyro_t,temp) + ((i) * sizeof(float))
#define ADCS_PARAM_GYRO_AXIS(i)			offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, gyro) + offsetof(gs_adcs_config_gyro_t,axis) + ((i) * sizeof(int8_t))
#define ADCS_PARAM_GYRO_SIGN(i)			offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, gyro) + offsetof(gs_adcs_config_gyro_t,sign) + ((i) * sizeof(int8_t))
#define ADCS_PARAM_GYRO_ENABLE(i)		offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, gyro) + offsetof(gs_adcs_config_gyro_t,enable) + ((i) * sizeof(int8_t))
#define ADCS_PARAM_CSS_OFFSET(i)		offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, css) + offsetof(gs_adcs_config_css_t,offset) + ((i) * sizeof(float))
#define ADCS_PARAM_CSS_SCALE(i)			offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, css) + offsetof(gs_adcs_config_css_t,scale) + ((i) * sizeof(float))
#define ADCS_PARAM_CSS_TEMP(i)			offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, css) + offsetof(gs_adcs_config_css_t,temp) + ((i) * sizeof(float))
#define ADCS_PARAM_CSS_AXIS(i)			offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, css) + offsetof(gs_adcs_config_css_t,axis) + ((i) * sizeof(int8_t))
#define ADCS_PARAM_CSS_ENABLE(i)		offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, css) + offsetof(gs_adcs_config_css_t,enable) + ((i) * sizeof(int8_t))
#define ADCS_PARAM_FSS_NUM 				offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, fss) + offsetof(gs_adcs_config_fssgeneral_t,num)
#define ADCS_PARAM_FSS_DARKTH			offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, fss) + offsetof(gs_adcs_config_fssgeneral_t,darkth)
#define ADCS_PARAM_FSS_IDARKTH			offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, fss) + offsetof(gs_adcs_config_fssgeneral_t,idarkth)
#define ADCS_PARAM_FSS_BRIGHTTH			offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, fss) + offsetof(gs_adcs_config_fssgeneral_t,brightth)
#define ADCS_PARAM_GPS_ENABLE		    offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, gps) + offsetof(gs_adcs_config_gps_t,enable)
#define ADCS_PARAM_GPS_ON_DUR			offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, gps) + offsetof(gs_adcs_config_gps_t,onDur)
#define ADCS_PARAM_GPS_OFF_DUR			offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, gps) + offsetof(gs_adcs_config_gps_t,offDur)
#define ADCS_PARAM_GPS_PROP_DUR			offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, gps) + offsetof(gs_adcs_config_gps_t,propDur)
#define ADCS_PARAM_HTPA_NODE(i)			offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, htpa) + offsetof(gs_adcs_config_htpa_t,node) + ((i) * sizeof(uint8_t))
#define ADCS_PARAM_HTPA_ENABLE(i)		offsetof(gs_adcs_config_periph_t, sensor) + offsetof(gs_adcs_config_sensor_t, htpa) + offsetof(gs_adcs_config_htpa_t,enable) + ((i) * sizeof(uint8_t))

#define ADCS_PARAM_MT_AM(i)             offsetof(gs_adcs_config_periph_t, act) + offsetof(gs_adcs_config_act_t, torquer) + offsetof(gs_adcs_config_torquer_t,am) + ((i) * sizeof(float))
#define ADCS_PARAM_MT_AXIS(i)           offsetof(gs_adcs_config_periph_t, act) + offsetof(gs_adcs_config_act_t, torquer) + offsetof(gs_adcs_config_torquer_t,axis) + ((i) * sizeof(int8_t))
#define ADCS_PARAM_MT_SIGN(i)           offsetof(gs_adcs_config_periph_t, act) + offsetof(gs_adcs_config_act_t, torquer) + offsetof(gs_adcs_config_torquer_t,sign) + ((i) * sizeof(int8_t))
#define ADCS_PARAM_MT_MAX(i)            offsetof(gs_adcs_config_periph_t, act) + offsetof(gs_adcs_config_act_t, torquer) + offsetof(gs_adcs_config_torquer_t,max) + ((i) * sizeof(int8_t))
#define ADCS_PARAM_MT_EFF(i)            offsetof(gs_adcs_config_periph_t, act) + offsetof(gs_adcs_config_act_t, torquer) + offsetof(gs_adcs_config_torquer_t,mt_dip_eff) + ((i) * sizeof(float))

#define ADCS_PARAM_MW_MAX_SPEED(i)      offsetof(gs_adcs_config_periph_t, act) + offsetof(gs_adcs_config_act_t, mw) + offsetof(gs_adcs_config_mw_t, max_speed)+ ((i) * sizeof(float))
#define ADCS_PARAM_MW_MAX_TORQUE(i)     offsetof(gs_adcs_config_periph_t, act) + offsetof(gs_adcs_config_act_t, mw) + offsetof(gs_adcs_config_mw_t, max_torque)+ ((i) * sizeof(float))
#define ADCS_PARAM_MW_INERTIA(i)        offsetof(gs_adcs_config_periph_t, act) + offsetof(gs_adcs_config_act_t, mw) + offsetof(gs_adcs_config_mw_t, inertia)+ ((i) * sizeof(float))
#define ADCS_PARAM_MW_INV_INERTIA(i)    offsetof(gs_adcs_config_periph_t, act) + offsetof(gs_adcs_config_act_t, mw) + offsetof(gs_adcs_config_mw_t, invInertia)+ ((i) * sizeof(float))
#define ADCS_PARAM_MW_LAYOUT(i)         offsetof(gs_adcs_config_periph_t, act) + offsetof(gs_adcs_config_act_t, mw) + offsetof(gs_adcs_config_mw_t, layout)+ ((i) * sizeof(float))
#define ADCS_PARAM_MW_INV_LAYOUT(i)     offsetof(gs_adcs_config_periph_t, act) + offsetof(gs_adcs_config_act_t, mw) + offsetof(gs_adcs_config_mw_t, invLayout)+ ((i) * sizeof(float))
#define ADCS_PARAM_MW_FAULT(i)          offsetof(gs_adcs_config_periph_t, act) + offsetof(gs_adcs_config_act_t, mw) + offsetof(gs_adcs_config_mw_t, fault)+ ((i) * sizeof(float))
#define ADCS_PARAM_MW_OVERRIDE          offsetof(gs_adcs_config_periph_t, act) + offsetof(gs_adcs_config_act_t, mw) + offsetof(gs_adcs_config_mw_t,mw_override)

#define ADCS_PARAM_SC_INERTIA(i)        offsetof(gs_adcs_config_gnc_t, sc) + offsetof(gs_adcs_config_sc_t,inertia)  + ((i) * sizeof(float))

#define ADCS_PARAM_MODE_ACSINIT			offsetof(gs_adcs_config_gnc_t, mode) + offsetof(gs_adcs_config_mode_t,acs_init)
#define ADCS_PARAM_MODE_ADSINIT			offsetof(gs_adcs_config_gnc_t, mode) + offsetof(gs_adcs_config_mode_t,ads_init)
#define ADCS_PARAM_MODE_EPHEMINIT		offsetof(gs_adcs_config_gnc_t, mode) + offsetof(gs_adcs_config_mode_t,ephem_init)

#define ADCS_PARAM_TS_SAMPLETIME		offsetof(gs_adcs_config_gnc_t, ts) + offsetof(gs_adcs_config_ts_t,sampletime)
#define ADCS_PARAM_TS_KF_DIV			offsetof(gs_adcs_config_gnc_t, ts) + offsetof(gs_adcs_config_ts_t,kf_divider)
#define ADCS_PARAM_TS_QMETHOD_DIV		offsetof(gs_adcs_config_gnc_t, ts) + offsetof(gs_adcs_config_ts_t,qmethod_divider)
#define ADCS_PARAM_TS_EPHEM_DIV			offsetof(gs_adcs_config_gnc_t, ts) + offsetof(gs_adcs_config_ts_t,ephem_divider)
#define ADCS_PARAM_TS_BDOT_DIV			offsetof(gs_adcs_config_gnc_t, ts) + offsetof(gs_adcs_config_ts_t,bdot_divider)
#define ADCS_PARAM_TS_CTRL_DIV			offsetof(gs_adcs_config_gnc_t, ts) + offsetof(gs_adcs_config_ts_t,ctrl_divider)
#define ADCS_PARAM_TS_PWM_DEAD			offsetof(gs_adcs_config_gnc_t, ts) + offsetof(gs_adcs_config_ts_t,pwm_deadtime)
#define ADCS_PARAM_TS_MT_CAL_DIV		offsetof(gs_adcs_config_gnc_t, ts) + offsetof(gs_adcs_config_ts_t,mt_calib_div)

#define ADCS_PARAM_BDOT_TCONST			offsetof(gs_adcs_config_gnc_t, bdot) + offsetof(gs_adcs_config_bdot_t,tconst)
#define ADCS_PARAM_BDOT_TCONST2			offsetof(gs_adcs_config_gnc_t, bdot) + offsetof(gs_adcs_config_bdot_t,tconst2)
#define ADCS_PARAM_BDOT_TSEP			offsetof(gs_adcs_config_gnc_t, bdot) + offsetof(gs_adcs_config_bdot_t,tsep)
#define ADCS_PARAM_BDOT_THRESHOLD(i)	offsetof(gs_adcs_config_gnc_t, bdot) + offsetof(gs_adcs_config_bdot_t,threshold) + ((i) * sizeof(float))
#define ADCS_PARAM_BDOT_TLIMIT(i)		offsetof(gs_adcs_config_gnc_t, bdot) + offsetof(gs_adcs_config_bdot_t,torquerlimit) + ((i) * sizeof(float))
#define ADCS_PARAM_BDOT_BAXIS			offsetof(gs_adcs_config_gnc_t, bdot) + offsetof(gs_adcs_config_bdot_t,baxis)
#define ADCS_PARAM_BDOT_BSIGN			offsetof(gs_adcs_config_gnc_t, bdot) + offsetof(gs_adcs_config_bdot_t,bsign)
#define ADCS_PARAM_BDOT_BSTRENGTH		offsetof(gs_adcs_config_gnc_t, bdot) + offsetof(gs_adcs_config_bdot_t,bstrength)
#define ADCS_PARAM_BDOT_MAGCHOICE		offsetof(gs_adcs_config_gnc_t, bdot) + offsetof(gs_adcs_config_bdot_t,magchoice)
#define ADCS_PARAM_BDOT_RATEREF(i)		offsetof(gs_adcs_config_gnc_t, bdot) + offsetof(gs_adcs_config_bdot_t,rateref) + ((i) * sizeof(float))

#define ADCS_PARAM_KF_CSS_MAXINIT		offsetof(gs_adcs_config_gnc_t, kf) + offsetof(gs_adcs_config_kf_t,css_maxinit)
#define ADCS_PARAM_KF_CSS_ABSMAX		offsetof(gs_adcs_config_gnc_t, kf) + offsetof(gs_adcs_config_kf_t,css_absmax)
#define ADCS_PARAM_KF_CSS_TH			offsetof(gs_adcs_config_gnc_t, kf) + offsetof(gs_adcs_config_kf_t,css_th)
#define ADCS_PARAM_KF_Q_QUAT			offsetof(gs_adcs_config_gnc_t, kf) + offsetof(gs_adcs_config_kf_t,Q_quat) 
#define ADCS_PARAM_KF_Q_RATE			offsetof(gs_adcs_config_gnc_t, kf) + offsetof(gs_adcs_config_kf_t,Q_rate) 
#define ADCS_PARAM_KF_Q_GYROBIAS		offsetof(gs_adcs_config_gnc_t, kf) + offsetof(gs_adcs_config_kf_t,Q_gyrobias) 
#define ADCS_PARAM_KF_Q_MAGBIAS		offsetof(gs_adcs_config_gnc_t, kf) + offsetof(gs_adcs_config_kf_t,Q_magbias) 
#define ADCS_PARAM_KF_Q_DTORQUE		offsetof(gs_adcs_config_gnc_t, kf) + offsetof(gs_adcs_config_kf_t,Q_dtorque) 
#define ADCS_PARAM_KF_R_CSS			offsetof(gs_adcs_config_gnc_t, kf) + offsetof(gs_adcs_config_kf_t,R_css) 
#define ADCS_PARAM_KF_R_MAG			offsetof(gs_adcs_config_gnc_t, kf) + offsetof(gs_adcs_config_kf_t,R_mag) 
#define ADCS_PARAM_KF_R_GYRO			offsetof(gs_adcs_config_gnc_t, kf) + offsetof(gs_adcs_config_kf_t,R_gyro) 
#define ADCS_PARAM_KF_R_FSS			offsetof(gs_adcs_config_gnc_t, kf) + offsetof(gs_adcs_config_kf_t,R_fss)
#define ADCS_PARAM_KF_R_ST			offsetof(gs_adcs_config_gnc_t, kf) + offsetof(gs_adcs_config_kf_t,R_st)
#define ADCS_PARAM_KF_P_QUAT			offsetof(gs_adcs_config_gnc_t, kf) + offsetof(gs_adcs_config_kf_t,P_quat) 
#define ADCS_PARAM_KF_P_RATE			offsetof(gs_adcs_config_gnc_t, kf) + offsetof(gs_adcs_config_kf_t,P_rate) 
#define ADCS_PARAM_KF_P_MAGBIAS		offsetof(gs_adcs_config_gnc_t, kf) + offsetof(gs_adcs_config_kf_t,P_magbias) 
#define ADCS_PARAM_KF_P_GYROBIAS		offsetof(gs_adcs_config_gnc_t, kf) + offsetof(gs_adcs_config_kf_t,P_gyrobias) 
#define ADCS_PARAM_KF_P_DTORQUE		offsetof(gs_adcs_config_gnc_t, kf) + offsetof(gs_adcs_config_kf_t,P_dtorque) 
#define ADCS_PARAM_KF_NOCHANGE			offsetof(gs_adcs_config_gnc_t, kf) + offsetof(gs_adcs_config_kf_t,nochange_disable)
#define ADCS_PARAM_KF_CTRLTORQUE		offsetof(gs_adcs_config_gnc_t, kf) + offsetof(gs_adcs_config_kf_t,enable_ctrltorque)
#define ADCS_PARAM_KF_FSS			offsetof(gs_adcs_config_gnc_t, kf) + offsetof(gs_adcs_config_kf_t,enable_fss)
#define ADCS_PARAM_KF_ST			offsetof(gs_adcs_config_gnc_t, kf) + offsetof(gs_adcs_config_kf_t,enable_startracker)
#define ADCS_PARAM_KF_MAXCUR			offsetof(gs_adcs_config_gnc_t, kf) + offsetof(gs_adcs_config_kf_t,enable_maxcur)
#define ADCS_PARAM_KF_NORMMAG			offsetof(gs_adcs_config_gnc_t, kf) + offsetof(gs_adcs_config_kf_t,normmag)
#define ADCS_PARAM_KF_NORMMAGBIAS		offsetof(gs_adcs_config_gnc_t, kf) + offsetof(gs_adcs_config_kf_t,normmagbias)
#define ADCS_PARAM_KF_MAGCHOICE		offsetof(gs_adcs_config_gnc_t, kf) + offsetof(gs_adcs_config_kf_t,magchoice)
#define ADCS_PARAM_KF_INTCHOICE		offsetof(gs_adcs_config_gnc_t, kf) + offsetof(gs_adcs_config_kf_t,intchoice)
#define ADCS_PARAM_KF_KFCHOICE		offsetof(gs_adcs_config_gnc_t, kf) + offsetof(gs_adcs_config_kf_t,kfchoice)
#define ADCS_PARAM_KF_M_BIAS(i)		        offsetof(gs_adcs_config_gnc_t, kf) + offsetof(gs_adcs_config_kf_t,M_bias) + ((i) * sizeof(float))
#define ADCS_PARAM_KF_MINANGLE		        offsetof(gs_adcs_config_gnc_t, kf) + offsetof(gs_adcs_config_kf_t,min_angle)

#define ADCS_PARAM_QMETHOD_SUNMAX		offsetof(gs_adcs_config_gnc_t, qmethod) + offsetof(gs_adcs_config_qmethod_t,sunmax)
#define ADCS_PARAM_QMETHOD_SUNTH		offsetof(gs_adcs_config_gnc_t, qmethod) + offsetof(gs_adcs_config_qmethod_t,sunth)
#define ADCS_PARAM_QMETHOD_MAGCHOICE		offsetof(gs_adcs_config_gnc_t, qmethod) + offsetof(gs_adcs_config_qmethod_t,magchoice)
#define ADCS_PARAM_QMETHOD_FSS			offsetof(gs_adcs_config_gnc_t, qmethod) + offsetof(gs_adcs_config_qmethod_t,enablefss)

#define ADCS_PARAM_CTRL_DETUMBL2POINT       offsetof(gs_adcs_config_gnc_t, ctrl) + offsetof(gs_adcs_config_ctrl_t,detumbl2point)
#define ADCS_PARAM_CTRL_POINT2DETUMBL       offsetof(gs_adcs_config_gnc_t, ctrl) + offsetof(gs_adcs_config_ctrl_t,point2detumbl)
#define ADCS_PARAM_CTRL_CONST_CTRL(i)       offsetof(gs_adcs_config_gnc_t, ctrl) + offsetof(gs_adcs_config_ctrl_t,const_ctrl) + ((i) * sizeof(float))
#define ADCS_PARAM_CTRL_MT_SMC(i)           offsetof(gs_adcs_config_gnc_t, ctrl) + offsetof(gs_adcs_config_ctrl_t,mt_smc) + ((i) * sizeof(float))
#define ADCS_PARAM_CTRL_MW_SMC(i)           offsetof(gs_adcs_config_gnc_t, ctrl) + offsetof(gs_adcs_config_ctrl_t,mw_smc)+ ((i) * sizeof(float))
#define ADCS_PARAM_CTRL_TH_ECLIPSE          offsetof(gs_adcs_config_gnc_t, ctrl) + offsetof(gs_adcs_config_ctrl_t,through_eclipse)
#define ADCS_PARAM_CTRL_ECLIPSE_DELAY       offsetof(gs_adcs_config_gnc_t, ctrl) + offsetof(gs_adcs_config_ctrl_t,eclipse_delay)
#define ADCS_PARAM_CTRL_POINT_OF(i)         offsetof(gs_adcs_config_gnc_t, ctrl) + offsetof(gs_adcs_config_ctrl_t,euler_offset) + ((i) * sizeof(float))
#define ADCS_PARAM_CTRL_RATE_OF(i)          offsetof(gs_adcs_config_gnc_t, ctrl) + offsetof(gs_adcs_config_ctrl_t,rate_offset) + ((i) * sizeof(float))
#define ADCS_PARAM_CTRL_MW_GAIN(i)          offsetof(gs_adcs_config_gnc_t, ctrl) + offsetof(gs_adcs_config_ctrl_t,mw_gain) + ((i) * sizeof(float))
#define ADCS_PARAM_CTRL_MW_DUMPGAIN         offsetof(gs_adcs_config_gnc_t, ctrl) + offsetof(gs_adcs_config_ctrl_t,mw_dumpgain)
#define ADCS_PARAM_CTRL_MW2TRQ              offsetof(gs_adcs_config_gnc_t, ctrl) + offsetof(gs_adcs_config_ctrl_t,mw_mw2trq)
#define ADCS_PARAM_CTRL_REF_MOMENT(i)       offsetof(gs_adcs_config_gnc_t, ctrl) + offsetof(gs_adcs_config_ctrl_t,ref_momentum)+ ((i) * sizeof(float))
#define ADCS_PARAM_CTRL_GSO(i)              offsetof(gs_adcs_config_gnc_t, ctrl) + offsetof(gs_adcs_config_ctrl_t,gso_list) + ((i) * sizeof(float))
#define ADCS_PARAM_CTRL_GS(i)               offsetof(gs_adcs_config_gnc_t, ctrl) + offsetof(gs_adcs_config_ctrl_t,grond_station_list) + ((i) * sizeof(float))
#define ADCS_PARAM_CTRL_GSO_ANT(i)          offsetof(gs_adcs_config_gnc_t, ctrl) + offsetof(gs_adcs_config_ctrl_t,gso_ant) + ((i) * sizeof(float))
#define ADCS_PARAM_CTRL_DIST_COMP_EN        offsetof(gs_adcs_config_gnc_t, ctrl) + offsetof(gs_adcs_config_ctrl_t,en_dist_comp)
#define ADCS_PARAM_CTRL_MWDUMP_EN           offsetof(gs_adcs_config_gnc_t, ctrl) + offsetof(gs_adcs_config_ctrl_t,en_mwdump)
#define ADCS_PARAM_CTRL_GSTRACK_EN		    offsetof(gs_adcs_config_gnc_t, ctrl) + offsetof(gs_adcs_config_ctrl_t,en_gstrack)
#define ADCS_PARAM_CTRL_BBQ_EN(i)			offsetof(gs_adcs_config_gnc_t, ctrl) + offsetof(gs_adcs_config_ctrl_t,en_bbq) + ((i) * sizeof(uint8_t))
#define ADCS_PARAM_CTRL_GSO_EN(i)			offsetof(gs_adcs_config_gnc_t, ctrl) + offsetof(gs_adcs_config_ctrl_t,en_gso) + ((i) * sizeof(uint8_t))
#define ADCS_PARAM_CTRL_SUNPOINT_EN			offsetof(gs_adcs_config_gnc_t, ctrl) + offsetof(gs_adcs_config_ctrl_t,en_sunpoint)

#define ADCS_PARAM_REF_RAM(i)               offsetof(gs_adcs_config_gnc_t, set) + offsetof(gs_adcs_config_reference_t,ram) + ((i) * sizeof(float))
#define ADCS_PARAM_REF_NADIR(i)             offsetof(gs_adcs_config_gnc_t, set) + offsetof(gs_adcs_config_reference_t,nadir) + ((i) * sizeof(float))
#define ADCS_PARAM_REF_MINDRAG(i)           offsetof(gs_adcs_config_gnc_t, set) + offsetof(gs_adcs_config_reference_t,mindrag) + ((i) * sizeof(float))
#define ADCS_PARAM_REF_MAXDRAG(i)           offsetof(gs_adcs_config_gnc_t, set) + offsetof(gs_adcs_config_reference_t,maxdrag) + ((i) * sizeof(float))
#define ADCS_PARAM_REF_CUSTOM0(i)           offsetof(gs_adcs_config_gnc_t, set) + offsetof(gs_adcs_config_reference_t,custom0) + ((i) * sizeof(float))
#define ADCS_PARAM_REF_CUSTOM1(i)           offsetof(gs_adcs_config_gnc_t, set) + offsetof(gs_adcs_config_reference_t,custom1) + ((i) * sizeof(float))
#define ADCS_PARAM_REF_CUSTOM2(i)           offsetof(gs_adcs_config_gnc_t, set) + offsetof(gs_adcs_config_reference_t,custom2) + ((i) * sizeof(float))

#define ADCS_PARAM_TGT_TARGET0(i)           offsetof(gs_adcs_config_gnc_t, set) + offsetof(gs_adcs_config_reference_t,target0) + ((i) * sizeof(float))
#define ADCS_PARAM_TGT_TARGET1(i)           offsetof(gs_adcs_config_gnc_t, set) + offsetof(gs_adcs_config_reference_t,target1) + ((i) * sizeof(float))
#define ADCS_PARAM_TGT_TARGET2(i)           offsetof(gs_adcs_config_gnc_t, set) + offsetof(gs_adcs_config_reference_t,target2) + ((i) * sizeof(float))

#define ADCS_PARAM_YSPIN_DUMPGAIN(i)			offsetof(gs_adcs_config_gnc_t, yspin) + offsetof(gs_adcs_config_yspin_t, dumpgain)+ ((i) * sizeof(float))
#define ADCS_PARAM_YSPIN_MWGAIN(i)			offsetof(gs_adcs_config_gnc_t, yspin) + offsetof(gs_adcs_config_yspin_t, mwgain)+ ((i) * sizeof(float))
#define ADCS_PARAM_YSPIN_MTGAIN(i)			offsetof(gs_adcs_config_gnc_t, yspin) + offsetof(gs_adcs_config_yspin_t, mtgain)+ ((i) * sizeof(float))
#define ADCS_PARAM_YSPIN_REF_RATE			offsetof(gs_adcs_config_gnc_t, yspin) + offsetof(gs_adcs_config_yspin_t, ref_rate)
#define ADCS_PARAM_YSPIN_MOMENT				offsetof(gs_adcs_config_gnc_t, yspin) + offsetof(gs_adcs_config_yspin_t, ref_mnt)
#define ADCS_PARAM_YSPIN_TRANS(i)			offsetof(gs_adcs_config_gnc_t, yspin) + offsetof(gs_adcs_config_yspin_t, trans)+ ((i) * sizeof(float))
#define ADCS_PARAM_YSPIN_PD_CTRL(i)			offsetof(gs_adcs_config_gnc_t, yspin) + offsetof(gs_adcs_config_yspin_t, pd_ctrl)+ ((i) * sizeof(float))

#define ADCS_PARAM_SGP4_TLE1			offsetof(gs_adcs_config_gnc_t, sgp4) + offsetof(gs_adcs_config_sgp4_t,tleline1)
#define ADCS_PARAM_SGP4_TLE2			offsetof(gs_adcs_config_gnc_t, sgp4) + offsetof(gs_adcs_config_sgp4_t,tleline2)
#define ADCS_PARAM_SGP4_TEME2ECI		offsetof(gs_adcs_config_gnc_t, sgp4) + offsetof(gs_adcs_config_sgp4_t,teme2eci)

#define ADCS_PARAM_WORK_INV_INERTIA(i)	offsetof(gs_adcs_config_gnc_t, work) + offsetof(gs_adcs_config_work_t,inv_inertia) + ((i) * sizeof(float))
#define ADCS_PARAM_WORK_MAG_ROTTRANS(i)	offsetof(gs_adcs_config_gnc_t, work) + offsetof(gs_adcs_config_work_t,mag_rot_trans) + ((i) * sizeof(float))
#define ADCS_PARAM_WORK_EXTMAG_ROTTRANS(i)	offsetof(gs_adcs_config_gnc_t, work) + offsetof(gs_adcs_config_work_t,extmag_rot_trans) + ((i) * sizeof(float))
#define ADCS_PARAM_WORK_TORQUER_SCALE	offsetof(gs_adcs_config_gnc_t, work) + offsetof(gs_adcs_config_work_t,torquer_scale)
#define ADCS_PARAM_WORK_TORQUER_SAT(i)	offsetof(gs_adcs_config_gnc_t, work) + offsetof(gs_adcs_config_work_t,torquer_sat) + ((i) * sizeof(uint8_t))


static const param_table_t adcs_param_fss[] = {
		{.name = "fss_add",	.addr = ADCS_PARAM_FSS_ADD, 							.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "fss_node",.addr = ADCS_PARAM_FSS_NODE, 							.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "fss_q", 	.addr = ADCS_PARAM_FSS_Q(0),							.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 4},
		{.name = "fss_P",	.addr = ADCS_PARAM_FSS_P(0),		   			        .type = PARAM_FLOAT, 	.size = sizeof(float), .count = 4},
		{.name = "fss_Cvalid",	.addr = ADCS_PARAM_FSS_CVALID, 						.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "fss_Ctheta0", 	.addr = ADCS_PARAM_FSS_CTHETA0(0),					.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 27},
		{.name = "fss_Ctheta1", 	.addr = ADCS_PARAM_FSS_CTHETA1(0),					.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 27},
		{.name = "fss_Ctheta2", 	.addr = ADCS_PARAM_FSS_CTHETA2(0),					.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 27},
		{.name = "fss_Cphi0", 	.addr = ADCS_PARAM_FSS_CPHI0(0),						.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 27},
		{.name = "fss_Cphi1", 	.addr = ADCS_PARAM_FSS_CPHI1(0),						.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 27},
		{.name = "fss_Cphi2", 	.addr = ADCS_PARAM_FSS_CPHI2(0),						.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 27},
};

static const param_table_t adcs_param_periph[] = {
		{.name = "mag_offset",		.addr = ADCS_PARAM_MAG_OFFSET(0),					.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 3},
		{.name = "mag_scale",		.addr = ADCS_PARAM_MAG_SCALE(0),					.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 3},
		{.name = "mag_temp",		.addr = ADCS_PARAM_MAG_TEMP(0),						.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 3},
		{.name = "mag_rotate",		.addr = ADCS_PARAM_MAG_ROTATE(0),					.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 9},
		{.name = "mag_axis",		.addr = ADCS_PARAM_MAG_AXIS(0),						.type = PARAM_INT8, 	.size = sizeof(int8_t), .count = 3},
		{.name = "mag_sign",		.addr = ADCS_PARAM_MAG_SIGN(0),						.type = PARAM_INT8, 	.size = sizeof(int8_t), .count = 3},
		{.name = "mag_enable",		.addr = ADCS_PARAM_MAG_ENABLE(0),					.type = PARAM_INT8, 	.size = sizeof(int8_t), .count = 3},
		{.name = "mag_gain",		.addr = ADCS_PARAM_MAG_GAIN,						.type = PARAM_INT8, 	.size = sizeof(int8_t)},
		{.name = "mag_rate",		.addr = ADCS_PARAM_MAG_RATE,						.type = PARAM_INT8, 	.size = sizeof(int8_t)},
		{.name = "mag_add",			.addr = ADCS_PARAM_MAG_ADD,							.type = PARAM_INT8, 	.size = sizeof(int8_t)},
		{.name = "mag_max",			.addr = ADCS_PARAM_MAG_MAXFIELD,					.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "extmag_offset",	.addr = ADCS_PARAM_EXTMAG_OFFSET(0),				.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 3},
		{.name = "extmag_scale",	.addr = ADCS_PARAM_EXTMAG_SCALE(0),					.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 3},
		{.name = "extmag_temp",		.addr = ADCS_PARAM_EXTMAG_TEMP(0),					.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 3},
		{.name = "extmag_rotate",	.addr = ADCS_PARAM_EXTMAG_ROTATE(0),				.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 9},
		{.name = "extmag_axis",		.addr = ADCS_PARAM_EXTMAG_AXIS(0),					.type = PARAM_INT8, 	.size = sizeof(int8_t), .count = 3},
		{.name = "extmag_sign",		.addr = ADCS_PARAM_EXTMAG_SIGN(0),					.type = PARAM_INT8, 	.size = sizeof(int8_t), .count = 3},
		{.name = "extmag_enable",	.addr = ADCS_PARAM_EXTMAG_ENABLE(0),				.type = PARAM_INT8, 	.size = sizeof(int8_t), .count = 3},
		{.name = "extmag_gain",		.addr = ADCS_PARAM_EXTMAG_GAIN,						.type = PARAM_INT8, 	.size = sizeof(int8_t)},
		{.name = "extmag_rate",		.addr = ADCS_PARAM_EXTMAG_RATE,						.type = PARAM_INT8, 	.size = sizeof(int8_t)},
		{.name = "extmag_add",		.addr = ADCS_PARAM_EXTMAG_ADD,						.type = PARAM_INT8, 	.size = sizeof(int8_t)},
		{.name = "extmag_max",		.addr = ADCS_PARAM_EXTMAG_MAXFIELD,					.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "gyro_offset",		.addr = ADCS_PARAM_GYRO_OFFSET(0),					.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 3},
		{.name = "gyro_scale",		.addr = ADCS_PARAM_GYRO_SCALE(0),					.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 3},
		{.name = "gyro_temp",		.addr = ADCS_PARAM_GYRO_TEMP(0),					.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 3},
		{.name = "gyro_axis",		.addr = ADCS_PARAM_GYRO_AXIS(0),					.type = PARAM_INT8, 	.size = sizeof(int8_t), .count = 3},
		{.name = "gyro_sign",		.addr = ADCS_PARAM_GYRO_SIGN(0),					.type = PARAM_INT8, 	.size = sizeof(int8_t), .count = 3},
		{.name = "gyro_enable",		.addr = ADCS_PARAM_GYRO_ENABLE(0),					.type = PARAM_INT8, 	.size = sizeof(int8_t), .count = 3},
		{.name = "css_offset",		.addr = ADCS_PARAM_CSS_OFFSET(0),					.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 6},
		{.name = "css_scale",		.addr = ADCS_PARAM_CSS_SCALE(0),					.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 6},
		{.name = "css_temp",		.addr = ADCS_PARAM_CSS_TEMP(0),						.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 6},
		{.name = "css_axis",		.addr = ADCS_PARAM_CSS_AXIS(0),						.type = PARAM_INT8, 	.size = sizeof(int8_t), .count = 6},
		{.name = "css_enable",		.addr = ADCS_PARAM_CSS_ENABLE(0),					.type = PARAM_INT8, 	.size = sizeof(int8_t), .count = 6},
		{.name = "fss_num",			.addr = ADCS_PARAM_FSS_NUM,							.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "fss_darkth",		.addr = ADCS_PARAM_FSS_DARKTH,						.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "fss_idarkth",		.addr = ADCS_PARAM_FSS_IDARKTH,						.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "fss_brightth",	.addr = ADCS_PARAM_FSS_BRIGHTTH,					.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "gps_enable",		.addr = ADCS_PARAM_GPS_ENABLE,						.type = PARAM_INT8, 	.size = sizeof(int8_t)},
		{.name = "gps_on_dur",		.addr = ADCS_PARAM_GPS_ON_DUR,						.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "gps_off_dur",		.addr = ADCS_PARAM_GPS_OFF_DUR,						.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "gps_prop_dur",	.addr = ADCS_PARAM_GPS_PROP_DUR,					.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "st1q",			.addr = ADCS_PARAM_ST_ST1ALIGN(0),					.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 4},
		{.name = "st2q",			.addr = ADCS_PARAM_ST_ST2ALIGN(0),					.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 4},
		{.name = "st_enable",		.addr = ADCS_PARAM_ST_ENABLE(0),					.type = PARAM_UINT8, 	.size = sizeof(uint8_t), .count = 2},
		{.name = "st_conf",			.addr = ADCS_PARAM_ST_CONF,							.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "htpa_node",		.addr = ADCS_PARAM_HTPA_NODE(0),					.type = PARAM_UINT8, 	.size = sizeof(uint8_t), .count = 2},
		{.name = "htpa_enable",		.addr = ADCS_PARAM_HTPA_ENABLE(0),					.type = PARAM_UINT8, 	.size = sizeof(uint8_t), .count = 2},
		{.name = "torquer_am",		.addr = ADCS_PARAM_MT_AM(0),						.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 3},
		{.name = "torquer_axis",	.addr = ADCS_PARAM_MT_AXIS(0),						.type = PARAM_UINT8, 	.size = sizeof(uint8_t), .count = 3},
		{.name = "torquer_sign",	.addr = ADCS_PARAM_MT_SIGN(0),						.type = PARAM_INT8, 	.size = sizeof(int8_t), .count = 3},
		{.name = "torquer_max",		.addr = ADCS_PARAM_MT_MAX(0),						.type = PARAM_UINT8, 	.size = sizeof(uint8_t), .count = 3},
		{.name = "torquer_eff",		.addr = ADCS_PARAM_MT_EFF(0),						.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 3},
		{.name = "mw_max_speed",	.addr = ADCS_PARAM_MW_MAX_SPEED(0),					.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 4},
		{.name = "mw_max_torque",	.addr = ADCS_PARAM_MW_MAX_TORQUE(0),				.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 4},
		{.name = "mw_inertia",		.addr = ADCS_PARAM_MW_INERTIA(0),					.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 4},
		{.name = "mw_inv_inertia",	.addr = ADCS_PARAM_MW_INV_INERTIA(0),				.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 4},
		{.name = "mw_layout",		.addr = ADCS_PARAM_MW_LAYOUT(0),					.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 12},
		{.name = "mw_inv_layout",	.addr = ADCS_PARAM_MW_INV_LAYOUT(0),				.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 12},
		{.name = "mw_fault",		.addr = ADCS_PARAM_MW_FAULT(0),						.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 16},
		{.name = "mw_wde_over",		.addr = ADCS_PARAM_MW_OVERRIDE,						.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
};


static const param_table_t adcs_param_gnc[] = {
		{.name = "inertia",				.addr = ADCS_PARAM_SC_INERTIA(0),			.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 3},
		{.name = "mode_acsinit",		.addr = ADCS_PARAM_MODE_ACSINIT,			.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "mode_adsinit",		.addr = ADCS_PARAM_MODE_ADSINIT,			.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "mode_epheminit",		.addr = ADCS_PARAM_MODE_EPHEMINIT,			.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "ts_sampletime",		.addr = ADCS_PARAM_TS_SAMPLETIME,			.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
		{.name = "ts_qmethod_div",		.addr = ADCS_PARAM_TS_QMETHOD_DIV,			.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
		{.name = "ts_ephem_div",		.addr = ADCS_PARAM_TS_EPHEM_DIV,			.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
		{.name = "ts_kf_div",			.addr = ADCS_PARAM_TS_KF_DIV,				.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
		{.name = "ts_bdot_div",			.addr = ADCS_PARAM_TS_BDOT_DIV,				.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
		{.name = "ts_ctrl_div",			.addr = ADCS_PARAM_TS_CTRL_DIV,				.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
		{.name = "ts_pwm_dead",			.addr = ADCS_PARAM_TS_PWM_DEAD,				.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
		{.name = "ts_mt_cal_div",		.addr = ADCS_PARAM_TS_MT_CAL_DIV,			.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
		{.name = "bdot_tconst",			.addr = ADCS_PARAM_BDOT_TCONST,				.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "bdot_tconst2",		.addr = ADCS_PARAM_BDOT_TCONST2,			.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "bdot_tsep",			.addr = ADCS_PARAM_BDOT_TSEP,				.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "bdot_threshold",		.addr = ADCS_PARAM_BDOT_THRESHOLD(0),		.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 3},
		{.name = "bdot_tlimit",			.addr = ADCS_PARAM_BDOT_TLIMIT(0),			.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 3},
		{.name = "bdot_baxis",			.addr = ADCS_PARAM_BDOT_BAXIS,				.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "bdot_bsign",			.addr = ADCS_PARAM_BDOT_BSIGN,				.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "bdot_bstrength",		.addr = ADCS_PARAM_BDOT_BSTRENGTH,			.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "bdot_mag",			.addr = ADCS_PARAM_BDOT_MAGCHOICE,			.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "bdot_rateref",		.addr = ADCS_PARAM_BDOT_RATEREF(0),			.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 3},
		{.name = "kf_cssmaxinit",		.addr = ADCS_PARAM_KF_CSS_MAXINIT,			.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "kf_cssabsmax",		.addr = ADCS_PARAM_KF_CSS_ABSMAX,			.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "kf_cssth",			.addr = ADCS_PARAM_KF_CSS_TH,				.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "kf_Q_quat",			.addr = ADCS_PARAM_KF_Q_QUAT,				.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "kf_Q_rate",			.addr = ADCS_PARAM_KF_Q_RATE,				.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "kf_Q_magbias",		.addr = ADCS_PARAM_KF_Q_MAGBIAS,			.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "kf_Q_gyrobias",		.addr = ADCS_PARAM_KF_Q_GYROBIAS,			.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "kf_Q_dtorque",		.addr = ADCS_PARAM_KF_Q_DTORQUE,			.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "kf_R_mag",			.addr = ADCS_PARAM_KF_R_MAG,				.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "kf_R_gyro",			.addr = ADCS_PARAM_KF_R_GYRO,				.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "kf_R_css",			.addr = ADCS_PARAM_KF_R_CSS,				.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "kf_R_fss",			.addr = ADCS_PARAM_KF_R_FSS,				.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "kf_R_st",				.addr = ADCS_PARAM_KF_R_ST,					.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "kf_P_quat",			.addr = ADCS_PARAM_KF_P_QUAT,				.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "kf_P_rate",			.addr = ADCS_PARAM_KF_P_RATE,				.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "kf_P_magbias",		.addr = ADCS_PARAM_KF_P_MAGBIAS,			.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "kf_P_gyrobias",		.addr = ADCS_PARAM_KF_P_GYROBIAS,			.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "kf_P_dtorque",		.addr = ADCS_PARAM_KF_P_DTORQUE,			.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "kf_nochange",			.addr = ADCS_PARAM_KF_NOCHANGE,				.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "kf_ctrltorque",		.addr = ADCS_PARAM_KF_CTRLTORQUE,			.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "kf_fss",				.addr = ADCS_PARAM_KF_FSS,					.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "kf_st",				.addr = ADCS_PARAM_KF_ST,					.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "kf_maxcur",			.addr = ADCS_PARAM_KF_MAXCUR,				.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "kf_normmag",			.addr = ADCS_PARAM_KF_NORMMAG,				.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "kf_normmagb"	,		.addr = ADCS_PARAM_KF_NORMMAGBIAS,			.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "kf_mag",				.addr = ADCS_PARAM_KF_MAGCHOICE,			.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "kf_int",				.addr = ADCS_PARAM_KF_INTCHOICE,			.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "kf_choice",			.addr = ADCS_PARAM_KF_KFCHOICE,				.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "kf_M_bias",		    .addr = ADCS_PARAM_KF_M_BIAS(0),			.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 3},
		{.name = "kf_minangle",		    .addr = ADCS_PARAM_KF_MINANGLE,				.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "qm_sunmax",			.addr = ADCS_PARAM_QMETHOD_SUNMAX,			.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "qm_sunth",			.addr = ADCS_PARAM_QMETHOD_SUNTH,			.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "qm_mag",				.addr = ADCS_PARAM_QMETHOD_MAGCHOICE,		.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "qm_fss",				.addr = ADCS_PARAM_QMETHOD_FSS,				.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "sgp4_tle1",			.addr = ADCS_PARAM_SGP4_TLE1,				.type = PARAM_STRING, 	.size = MAX_ADCS_TLE_LINE, .flags = PARAM_F_PERSIST},
		{.name = "sgp4_tle2",			.addr = ADCS_PARAM_SGP4_TLE2,				.type = PARAM_STRING, 	.size = MAX_ADCS_TLE_LINE, .flags = PARAM_F_PERSIST},
		{.name = "sgp4_teme2eci",		.addr = ADCS_PARAM_SGP4_TEME2ECI,			.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "ctrl_d2p",			.addr = ADCS_PARAM_CTRL_DETUMBL2POINT,		.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "ctrl_p2d",			.addr = ADCS_PARAM_CTRL_POINT2DETUMBL,		.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "ctrl_const",			.addr = ADCS_PARAM_CTRL_CONST_CTRL(0),		.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 3},
		{.name = "ctrl_th_eclip",		.addr = ADCS_PARAM_CTRL_TH_ECLIPSE,			.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "ctrl_eclipdly",		.addr = ADCS_PARAM_CTRL_ECLIPSE_DELAY,		.type = PARAM_UINT16, 	.size = sizeof(uint16_t)},
		{.name = "ctrl_mt_smc",			.addr = ADCS_PARAM_CTRL_MT_SMC(0),			.type = PARAM_FLOAT, 	.size = sizeof(float), .count=2},
		{.name = "ctrl_mw_smc",			.addr = ADCS_PARAM_CTRL_MW_SMC(0),			.type = PARAM_FLOAT, 	.size = sizeof(float), .count=3},
		{.name = "ctrl_point_of",		.addr = ADCS_PARAM_CTRL_POINT_OF(0),		.type = PARAM_FLOAT, 	.size = sizeof(float), .count=3, .flags = PARAM_F_PERSIST},
		{.name = "ctrl_rate_of",		.addr = ADCS_PARAM_CTRL_RATE_OF(0),			.type = PARAM_FLOAT, 	.size = sizeof(float), .count=3},
		{.name = "ctrl_mw_gain",		.addr = ADCS_PARAM_CTRL_MW_GAIN(0),			.type = PARAM_FLOAT, 	.size = sizeof(float), .count=3},
		{.name = "ctrl_ref_momen",		.addr = ADCS_PARAM_CTRL_REF_MOMENT(0),		.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 3},
		{.name = "ctrl_gs_list",		.addr = ADCS_PARAM_CTRL_GS(0),				.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 9},
		{.name = "ctrl_gso_list",		.addr = ADCS_PARAM_CTRL_GSO(0),				.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 9},
		{.name = "ctrl_gso_ant",		.addr = ADCS_PARAM_CTRL_GSO_ANT(0),			.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 9},
		{.name = "ctrl_en_dist",		.addr = ADCS_PARAM_CTRL_DIST_COMP_EN,		.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "ctrl_en_mwdump",		.addr = ADCS_PARAM_CTRL_MWDUMP_EN,			.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "ctrl_dumpgain",		.addr = ADCS_PARAM_CTRL_MW_DUMPGAIN,		.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "ctrl_mw2trq",			.addr = ADCS_PARAM_CTRL_MW2TRQ,				.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "ctrl_en_bbq",			.addr = ADCS_PARAM_CTRL_BBQ_EN(0),			.type = PARAM_UINT8, 	.size = sizeof(uint8_t), .count = 2},
		{.name = "ctrl_en_gso",			.addr = ADCS_PARAM_CTRL_GSO_EN(0),			.type = PARAM_UINT8, 	.size = sizeof(uint8_t), .count = 3},
		{.name = "ctrl_en_gstrk",       .addr = ADCS_PARAM_CTRL_GSTRACK_EN,			.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
		{.name = "ctrl_en_sun",         .addr = ADCS_PARAM_CTRL_SUNPOINT_EN,		.type = PARAM_UINT8, 	.size = sizeof(uint8_t)},
        {.name = "ref_nadir",           .addr = ADCS_PARAM_REF_NADIR(0),            .type = PARAM_FLOAT,    .size = sizeof(float), .count = 3},
        {.name = "ref_ram",             .addr = ADCS_PARAM_REF_RAM(0),              .type = PARAM_FLOAT,    .size = sizeof(float), .count = 3},
        {.name = "ref_mindrag",         .addr = ADCS_PARAM_REF_MINDRAG(0),          .type = PARAM_FLOAT,    .size = sizeof(float), .count = 3},
        {.name = "ref_maxdrag",         .addr = ADCS_PARAM_REF_MAXDRAG(0),          .type = PARAM_FLOAT,    .size = sizeof(float), .count = 3},
        {.name = "ref_custom0",         .addr = ADCS_PARAM_REF_CUSTOM0(0),          .type = PARAM_FLOAT,    .size = sizeof(float), .count = 3},
        {.name = "ref_custom1",         .addr = ADCS_PARAM_REF_CUSTOM1(0),          .type = PARAM_FLOAT,    .size = sizeof(float), .count = 3},
        {.name = "ref_custom2",         .addr = ADCS_PARAM_REF_CUSTOM2(0),          .type = PARAM_FLOAT,    .size = sizeof(float), .count = 3},
        {.name = "ref_target0",         .addr = ADCS_PARAM_TGT_TARGET0(0),          .type = PARAM_FLOAT,    .size = sizeof(float), .count = 3},
        {.name = "ref_target1",         .addr = ADCS_PARAM_TGT_TARGET1(0),          .type = PARAM_FLOAT,    .size = sizeof(float), .count = 3},
        {.name = "ref_target2",         .addr = ADCS_PARAM_TGT_TARGET2(0),          .type = PARAM_FLOAT,    .size = sizeof(float), .count = 3},
		{.name = "yspin_dumpgain",		.addr = ADCS_PARAM_YSPIN_DUMPGAIN(0),		.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 3},
		{.name = "yspin_mwgain",		.addr = ADCS_PARAM_YSPIN_MWGAIN(0),			.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 3},
		{.name = "yspin_mtgain",		.addr = ADCS_PARAM_YSPIN_MTGAIN(0),			.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 4},
		{.name = "yspin_ref_rate",		.addr = ADCS_PARAM_YSPIN_REF_RATE,			.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "yspin_ref_mnt",		.addr = ADCS_PARAM_YSPIN_MOMENT,			.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "yspin_trans",			.addr = ADCS_PARAM_YSPIN_TRANS(0),			.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 6},
		{.name = "yspin_pd_ctrl",		.addr = ADCS_PARAM_YSPIN_PD_CTRL(0),		.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 2},
		{.name = "work_iinertia",		.addr = ADCS_PARAM_WORK_INV_INERTIA(0),		.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 3},
		{.name = "work_mrt",			.addr = ADCS_PARAM_WORK_MAG_ROTTRANS(0),	.type = PARAM_FLOAT, 	.size = sizeof(float), .count = 9},
		{.name = "work_tq_scale",		.addr = ADCS_PARAM_WORK_TORQUER_SCALE,		.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "work_tq_sat",			.addr = ADCS_PARAM_WORK_TORQUER_SAT(0),		.type = PARAM_UINT8, 	.size = sizeof(uint8_t), .count = 3},
};



#define ADCS_PARAM_PERIPH_COUNT (sizeof(adcs_param_periph) / sizeof(adcs_param_periph[0]))
#define ADCS_PARAM_GNC_COUNT (sizeof(adcs_param_gnc) / sizeof(adcs_param_gnc[0]))
#define ADCS_PARAM_FSS_COUNT (sizeof(adcs_param_fss) / sizeof(adcs_param_fss[0]))

#endif /* ADCS_PARAM_H_ */
