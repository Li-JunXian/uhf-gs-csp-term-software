#ifndef ADCS_TELEM_H_
#define ADCS_TELEM_H_

#include <adcs/adcs_telem_types.h>

#define PARAM_ADCS_TELEM1			32
#define PARAM_ADCS_TELEM2			33
#define PARAM_ADCS_TELEM3			34
#define PARAM_ADCS_TELEM4			35
#define PARAM_ADCS_TELEM5			36
#define PARAM_ADCS_TELEM6			37
#define PARAM_ADCS_TELEM7			38


//#define ADCS_FIXED_PARAM_ADD


#ifndef  ADCS_FIXED_PARAM_ADD
/**
 * TODO:
 * Offsetof does not work unless the datastructure is packed!
 * But, if we pack sensordata, the system will crash because of unaligned data access inside of libadcs
 * So for now, the fix was to run rparam download to get all the addresses and them enter them manually.
 */
#define ADCS_TELEM1_MAG(i)					(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, magnetometer) + sizeof(float) * i)
#define ADCS_TELEM1_MAG_VALID				(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, magnetometerValid))
#define ADCS_TELEM1_EXTMAG(i)				(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, extmagnetometer) + sizeof(float) * i)
#define ADCS_TELEM1_EXTMAG_TEMP				(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, extmagnetometer_temp))
#define ADCS_TELEM1_EXTMAG_VALID			(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, extmagnetometerValid))
#define ADCS_TELEM1_SUNS(i)					(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, sunsensor) + sizeof(float) * i)
#define ADCS_TELEM1_SUNS_RAW(i)				(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, sunsensor_raw) + sizeof(uint16_t) * i)
#define ADCS_TELEM1_SUNS_VALID				(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, sunsensorValid))
#define ADCS_TELEM1_SUNS_TEMP(i)			(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, sunsensorTemperatures) + sizeof(float) * i)
#define ADCS_TELEM1_GYRO(i)					(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, gyro) + sizeof(float) * i)
#define ADCS_TELEM1_GYRO_NORM(i)			(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, gyro_norm) + sizeof(float) * i)
#define ADCS_TELEM1_GYRO_TREND(i)			(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, gyro_trend) + sizeof(float) * i)
#define ADCS_TELEM1_GYRO_TEMP				(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, gyro_temp))
#define ADCS_TELEM1_GYRO_VALID				(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, gyroValid))
#define ADCS_TELEM1_ST1(i)					(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, st1_qis) + sizeof(float) * i)
#define ADCS_TELEM1_ST1_VALID				(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, st1_valid))
#define ADCS_TELEM1_ST2(i)					(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, st2_qis) + sizeof(float) * i)
#define ADCS_TELEM1_ST2_VALID				(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, st2_valid))
#define ADCS_TELEM1_FSS(i, j)				(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, fss) +  (sizeof(float)*i*3) + (sizeof(float)*j))
#define ADCS_TELEM1_FSS_BODY(i, j)			(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, fss_body) + (sizeof(float)*i*3) + (sizeof(float)*j))
#define ADCS_TELEM1_FSS_RAW(i, j)			(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, fss_raw) + (sizeof(uint16_t)*i*4) + (sizeof(uint16_t)*j))
#define ADCS_TELEM1_FSS_TEMP(i)				(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, fss_temp) + sizeof(float) * i)
#define ADCS_TELEM1_FSS_VALID(i)			(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, fssValid) + sizeof(uint8_t) * i)
#define ADCS_TELEM1_GPS_POS(i)				(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, gps_pos) + sizeof(float) * i)
#define ADCS_TELEM1_GPS_VEL(i)				(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, gps_vel) + sizeof(float) * i)
#define ADCS_TELEM1_GPS_VEL_DEV(i)			(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, gps_veldeviation) + sizeof(float) * i)
#define ADCS_TELEM1_GPS_POS_DEV(i)			(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, gps_posdeviation) + sizeof(float) * i)
#define ADCS_TELEM1_GPS_EPOCH				(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, gps_unix))
#define ADCS_TELEM1_GPS_JULIAN				(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, gps_julian))
#define ADCS_TELEM1_GPS_VALID				(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, gps_valid))
#define ADCS_TELEM1_GPS_TDIFF_JD			(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, gps_tdiff_jd))
#define ADCS_TELEM1_GPS_TDIFF_UT			(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, gps_tdiff_ut))
#define ADCS_TELEM1_GPS_SAT					(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, gps_sat))
#define ADCS_TELEM1_GPS_SATSOL				(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, gps_satsol))
#define ADCS_TELEM1_GPS_PPS_UT				(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, gps_lastpps_unix))
#define ADCS_TELEM1_GPS_PPS_JD				(offsetof(gs_adcs_telem1_t, sensordata) + offsetof(gs_adcs_measurementdata_t, gps_lastpps_jdate))

#define ADCS_TELEM1_TORQUERDUTY(i)			(offsetof(gs_adcs_telem1_t, actdata) + offsetof(gs_adcs_actuatordata_t, torquerDutyCycle) + sizeof(float) * i)
#define ADCS_TELEM1_TORQUERCALIB(i)			(offsetof(gs_adcs_telem1_t, actdata) + offsetof(gs_adcs_actuatordata_t, torquerTempScale) + sizeof(float) * i)
#define ADCS_TELEM1_WHEELTORQUE(i)			(offsetof(gs_adcs_telem1_t, actdata) + offsetof(gs_adcs_actuatordata_t, wheelTorque) + sizeof(float) * i)
#define ADCS_TELEM1_WHEELMOMENTUM(i)		(offsetof(gs_adcs_telem1_t, actdata) + offsetof(gs_adcs_actuatordata_t, wheelMomentum) + sizeof(float) * i)
#define ADCS_TELEM1_WHEELSPEED(i)			(offsetof(gs_adcs_telem1_t, actdata) + offsetof(gs_adcs_actuatordata_t, wheelSpeed) + sizeof(float) * i)
#define ADCS_TELEM1_WHEELENABLE(i)			(offsetof(gs_adcs_telem1_t, actdata) + offsetof(gs_adcs_actuatordata_t, wheelEnable) + sizeof(uint8_t) * i)

#else
#define ADCS_TELEM1_MAG(i)					(0x0000 + (sizeof(float) * i)             )
#define ADCS_TELEM1_MAG_VALID				(0x000C                                   )
#define ADCS_TELEM1_EXTMAG(i)				(0x0010 + (sizeof(float) * i)             )
#define ADCS_TELEM1_EXTMAG_TEMP				(0x001C                                   )
#define ADCS_TELEM1_EXTMAG_VALID			(0x0020                                   )
#define ADCS_TELEM1_SUNS(i)					(0x0024 + (sizeof(float) * i)             )
#define ADCS_TELEM1_SUNS_RAW(i)				(0x003C + (sizeof(uint16_t) * i)          )
#define ADCS_TELEM1_SUNS_VALID				(0x0048                                   )
#define ADCS_TELEM1_SUNS_TEMP(i)			(0x004C + (sizeof(float) * i)             )
#define ADCS_TELEM1_GYRO(i)					(0x0064 + (sizeof(float) * i)             )
#define ADCS_TELEM1_GYRO_NORM(i)			(0x0070 + (sizeof(float) * i)             )
#define ADCS_TELEM1_GYRO_TREND(i)			(0x007C + (sizeof(float) * i)             )
#define ADCS_TELEM1_GYRO_TEMP				(0x0088                                   )
#define ADCS_TELEM1_GYRO_VALID				(0x008C                                   )
#define ADCS_TELEM1_FSS(i, j)				(0x0090 + (4*i*3) + (4*j))
#define ADCS_TELEM1_FSS_BODY(i, j)			(0x00F0 + (4*i*3) + (4*j))
#define ADCS_TELEM1_FSS_RAW(i, j)			(0x0150 + (2*i*4) + (2*j))
#define ADCS_TELEM1_FSS_TEMP(i)				(0x0190 + (sizeof(float) * i)             )
#define ADCS_TELEM1_FSS_VALID(i)			(0x01B0 + (sizeof(uint8_t) * i)           )
#define ADCS_TELEM1_GPS_POS(i)				(0x01B8 + (sizeof(float) * i)             )
#define ADCS_TELEM1_GPS_VEL(i)				(0x01C4 + (sizeof(float) * i)             )
#define ADCS_TELEM1_GPS_VEL_DEV(i)			(0x01DC + (sizeof(float) * i)             )
#define ADCS_TELEM1_GPS_POS_DEV(i)			(0x01D0 + (sizeof(float) * i)             )
#define ADCS_TELEM1_GPS_EPOCH				(0x01E8                                   )
#define ADCS_TELEM1_GPS_JULIAN				(0x01F0                                   )
#define ADCS_TELEM1_GPS_VALID				(0x01F8                                   )
#define ADCS_TELEM1_GPS_SAT					(0x01F9                                   )
#define ADCS_TELEM1_GPS_SATSOL				(0x01FA                                   )
#define ADCS_TELEM1_TORQUERDUTY(i)			(0x01FC + (sizeof(float) * i)             )
#define ADCS_TELEM1_TORQUERCALIB(i)			(0x0214 + (sizeof(float) * i)             )
#define ADCS_TELEM1_WHEELTORQUE(i)			(0x0220 + (sizeof(float) * i)             )
#define ADCS_TELEM1_WHEELMOMENTUM(i)		(0x0230 + (sizeof(float) * i)             )
#define ADCS_TELEM1_WHEELSPEED(i)			(0x0240 + (sizeof(float) * i)             )
#define ADCS_TELEM1_WHEELENABLE(i)			(0x0250 + (sizeof(uint8_t) * i)             )
#endif

#ifndef  ADCS_FIXED_PARAM_ADD
#define ADCS_TELEM2_ACS_MODE				(offsetof(gs_adcs_telem2_t, mode) + offsetof(adcs_mode_t, acs_mode))
#define ADCS_TELEM2_ACS_DMODE			    (offsetof(gs_adcs_telem2_t, mode) + offsetof(adcs_mode_t, desired_acs_mode))
#define ADCS_TELEM2_ADS_MODE				(offsetof(gs_adcs_telem2_t, mode) + offsetof(adcs_mode_t, ads_mode))
#define ADCS_TELEM2_ADS_DMODE				(offsetof(gs_adcs_telem2_t, mode) + offsetof(adcs_mode_t, desired_ads_mode))
#define ADCS_TELEM2_EPHEM_MODE				(offsetof(gs_adcs_telem2_t, mode) + offsetof(adcs_mode_t, ephem_mode))
#define ADCS_TELEM2_EPHEM_DMODE				(offsetof(gs_adcs_telem2_t, mode) + offsetof(adcs_mode_t, desired_ephem_mode))
#define ADCS_TELEM2_YSPIN_MODE				(offsetof(gs_adcs_telem2_t, mode) + offsetof(adcs_mode_t, acs_submode) + offsetof(acs_submode_t,yspin_mode))
#define ADCS_TELEM2_STA_MAG				(offsetof(gs_adcs_telem2_t, status) + offsetof(adcs_status_t, sensorStatus) + offsetof(gs_adcs_sensor_status_t, mag))
#define ADCS_TELEM2_STA_EXTMAG			(offsetof(gs_adcs_telem2_t, status) + offsetof(adcs_status_t, sensorStatus) + offsetof(gs_adcs_sensor_status_t, extmag))
#define ADCS_TELEM2_STA_CSS			    (offsetof(gs_adcs_telem2_t, status) + offsetof(adcs_status_t, sensorStatus) + offsetof(gs_adcs_sensor_status_t, sun))
#define ADCS_TELEM2_STA_FSS(i)			(offsetof(gs_adcs_telem2_t, status) + offsetof(adcs_status_t, sensorStatus) + offsetof(gs_adcs_sensor_status_t, fss)+ sizeof(int8_t) * i)
#define ADCS_TELEM2_STA_GYRO		    (offsetof(gs_adcs_telem2_t, status) + offsetof(adcs_status_t, sensorStatus) + offsetof(gs_adcs_sensor_status_t, gyro))
#define ADCS_TELEM2_STA_GPS			    (offsetof(gs_adcs_telem2_t, status) + offsetof(adcs_status_t, sensorStatus) + offsetof(gs_adcs_sensor_status_t, gps))
#define ADCS_TELEM2_STA_HTPA(i)		    (offsetof(gs_adcs_telem2_t, status) + offsetof(adcs_status_t, sensorStatus) + offsetof(gs_adcs_sensor_status_t, htpa) + sizeof(int8_t) * i)

#define ADCS_TELEM2_STA_BDOT		    (offsetof(gs_adcs_telem2_t, status) + offsetof(adcs_status_t, bdotStatus) )
#define ADCS_TELEM2_STA_UKF		        (offsetof(gs_adcs_telem2_t, status) + offsetof(adcs_status_t, ukfStatus) )
#define ADCS_TELEM2_STA_IGRF	        (offsetof(gs_adcs_telem2_t, status) + offsetof(adcs_status_t, ephemStatus) + offsetof(gs_adcs_ephem_status_t, igrf))
#define ADCS_TELEM2_STA_SGP4	        (offsetof(gs_adcs_telem2_t, status) + offsetof(adcs_status_t, ephemStatus) + offsetof(gs_adcs_ephem_status_t, sgp4))
#define ADCS_TELEM2_STA_J2ELE	        (offsetof(gs_adcs_telem2_t, status) + offsetof(adcs_status_t, ephemStatus) + offsetof(gs_adcs_ephem_status_t, j2elem))
#define ADCS_TELEM2_STA_J2PRO	        (offsetof(gs_adcs_telem2_t, status) + offsetof(adcs_status_t, ephemStatus) + offsetof(gs_adcs_ephem_status_t, j2prop))
#define ADCS_TELEM2_STA_ETIME	        (offsetof(gs_adcs_telem2_t, status) + offsetof(adcs_status_t, ephemStatus) + offsetof(gs_adcs_ephem_status_t, time))
#define ADCS_TELEM2_STA_EPHEM	        (offsetof(gs_adcs_telem2_t, status) + offsetof(adcs_status_t, ephemStatus) + offsetof(gs_adcs_ephem_status_t, status))

#define ADCS_TELEM2_STA_RUN	            (offsetof(gs_adcs_telem2_t, status) + offsetof(adcs_status_t, runStatus))
#define ADCS_TELEM2_TLOOP	            (offsetof(gs_adcs_telem2_t, status) + offsetof(adcs_status_t, looptime))
#define ADCS_TELEM2_MAXTLOOP	        (offsetof(gs_adcs_telem2_t, status) + offsetof(adcs_status_t, maxlooptime))
#define ADCS_TELEM2_BDOT_RATE(i)		(offsetof(gs_adcs_telem2_t, bdot) + offsetof(gs_adcs_bdot_data_t, rate) + sizeof(float) * i)
#define ADCS_TELEM2_BDOT_DMAG(i)		(offsetof(gs_adcs_telem2_t, bdot) + offsetof(gs_adcs_bdot_data_t, diffmag) + sizeof(float) * i)
#define ADCS_TELEM2_BDOT_TORQUER(i)		(offsetof(gs_adcs_telem2_t, bdot) + offsetof(gs_adcs_bdot_data_t, torquerduty) + sizeof(float) * i)
#define ADCS_TELEM2_BDOT_DETUMBLED		(offsetof(gs_adcs_telem2_t, bdot) + offsetof(gs_adcs_bdot_data_t, detumbled))

#define ADCS_TELEM2_DEBUG_TASK			(offsetof(gs_adcs_telem2_t, debug) + offsetof(adcs_debug_t, task))
#define ADCS_TELEM2_DEBUG_MAIN			(offsetof(gs_adcs_telem2_t, debug) + offsetof(adcs_debug_t, main))



#else
#define ADCS_TELEM2_ACS_MODE			(0x0000)
#define ADCS_TELEM2_ACS_DMODE			(0x0001)
#define ADCS_TELEM2_ADS_MODE			(0x0002)
#define ADCS_TELEM2_ADS_DMODE			(0x0003)
#define ADCS_TELEM2_EPHEM_MODE			(0x0005)
#define ADCS_TELEM2_EPHEM_DMODE			(0x0004)
#define ADCS_TELEM2_YSPIN_MODE			(0x0006)
#define ADCS_TELEM2_STA_MAG				(0x0008)
#define ADCS_TELEM2_STA_EXTMAG			(0x0009)
#define ADCS_TELEM2_STA_CSS			    (0x000A)
#define ADCS_TELEM2_STA_FSS(i)			(0x000B + 1*i)
#define ADCS_TELEM2_STA_GYRO		    (0x0013)
#define ADCS_TELEM2_STA_GPS			    (0x0014)
#define ADCS_TELEM2_STA_HTPA(i)		    (0x0015 + 1*i)
#define ADCS_TELEM2_STA_BDOT		    (0x0017)
#define ADCS_TELEM2_STA_UKF		        (0x0018)
#define ADCS_TELEM2_STA_IGRF	        (0x0019)
#define ADCS_TELEM2_STA_SGP4	        (0x001A)
#define ADCS_TELEM2_STA_J2ELE	        (0x001B)
#define ADCS_TELEM2_STA_J2PRO	        (0x001C)
#define ADCS_TELEM2_STA_ETIME			(0x001D)
#define ADCS_TELEM2_STA_EPHEM			(0x001E)
#define ADCS_TELEM2_STA_RUN	            (0x001F)
#define ADCS_TELEM2_TLOOP	            (0x0020)
#define ADCS_TELEM2_MAXTLOOP	        (0x0022)
#define ADCS_TELEM2_BDOT_RATE(i)		(0x0028 + 4*i)
#define ADCS_TELEM2_BDOT_DMAG(i)		(0x0030 + 4*i)
#define ADCS_TELEM2_BDOT_TORQUER(i)		(0x003C + 4*i)
#define ADCS_TELEM2_BDOT_DETUMBLED		(0x0048)
#define ADCS_TELEM2_DEBUG_TASK			(0x004C)
#define ADCS_TELEM2_DEBUG_MAIN			(0x004D)

#endif


#ifndef  ADCS_FIXED_PARAM_ADD
#define ADCS_TELEM3_UKF_X(i)			(offsetof(gs_adcs_telem3_t, ukfdata) + offsetof(GS_ADCS_UKF_Data_t, EstimatedState) + sizeof(float) * i)
#define ADCS_TELEM3_UKF_Q(i)			(offsetof(gs_adcs_telem3_t, ukfdata) + offsetof(GS_ADCS_UKF_Data_t, q) + sizeof(float) * i)
#define ADCS_TELEM3_UKF_W(i)			(offsetof(gs_adcs_telem3_t, ukfdata) + offsetof(GS_ADCS_UKF_Data_t, w) + sizeof(float) * i)
#define ADCS_TELEM3_UKF_PDIAG(i)		(offsetof(gs_adcs_telem3_t, ukfdata) + offsetof(GS_ADCS_UKF_Data_t, Pdiag) + sizeof(float) * i)
#define ADCS_TELEM3_UKF_XPRED(i)		(offsetof(gs_adcs_telem3_t, ukfdata) + offsetof(GS_ADCS_UKF_Data_t, Xpred) + sizeof(float) * i)
#define ADCS_TELEM3_UKF_ZPRED(i)		(offsetof(gs_adcs_telem3_t, ukfdata) + offsetof(GS_ADCS_UKF_Data_t, Zpred) + sizeof(float) * i)
#define ADCS_TELEM3_UKF_SUNMAX(i)		(offsetof(gs_adcs_telem3_t, ukfdata) + offsetof(GS_ADCS_UKF_Data_t, sunsensorMax) + sizeof(float) * i)
#define ADCS_TELEM3_UKF_INECLIPSE		(offsetof(gs_adcs_telem3_t, ukfdata) + offsetof(GS_ADCS_UKF_Data_t, InEclipse))
#define ADCS_TELEM3_UKF_CHOICE			(offsetof(gs_adcs_telem3_t, ukfdata) + offsetof(GS_ADCS_UKF_Data_t, KFchoice))
#define ADCS_TELEM3_UKF_Z(i)			(offsetof(gs_adcs_telem3_t, ukfmeas) + offsetof(GS_ADCS_UKF_Measurement_t, MeasurementFilt) + sizeof(float) * i)
#define ADCS_TELEM3_UKF_MEAS(i)			(offsetof(gs_adcs_telem3_t, ukfmeas) + offsetof(GS_ADCS_UKF_Measurement_t, Measurement) + sizeof(float) * i)
#define ADCS_TELEM3_UKF_ENABLE(i)		(offsetof(gs_adcs_telem3_t, ukfmeas) + offsetof(GS_ADCS_UKF_Measurement_t, Enable) + sizeof(uint8_t) * i)
#define ADCS_TELEM3_UKF_CTRLT(i)		(offsetof(gs_adcs_telem3_t, ukfinput) + offsetof(GS_ADCS_UKF_Inputs_t, CtrlTorques) + sizeof(float) * i)
#define ADCS_TELEM3_UKF_CTRLM(i)		(offsetof(gs_adcs_telem3_t, ukfinput) + offsetof(GS_ADCS_UKF_Inputs_t, CtrlMomentum) + sizeof(float) * i)
#define ADCS_TELEM3_UKF_RATE(i)			(offsetof(gs_adcs_telem3_t, ukfinput) + offsetof(GS_ADCS_UKF_Inputs_t, Rate) + sizeof(float) * i)
#else
#define ADCS_TELEM3_UKF_X(i) 		(0x0000 + (sizeof(float) * i))
#define ADCS_TELEM3_UKF_Q(i) 		(0x0034 + (sizeof(float) * i))
#define ADCS_TELEM3_UKF_W(i) 		(0x0044 + (sizeof(float) * i))
#define ADCS_TELEM3_UKF_PDIAG(i) 	(0x0050 + (sizeof(float) * i))
#define ADCS_TELEM3_UKF_XPRED(i) 	(0x0080 + (sizeof(float) * i))
#define ADCS_TELEM3_UKF_ZPRED(i) 	(0x00B4 + (sizeof(float) * i))
#define ADCS_TELEM3_UKF_Z(i) 		(0x0190 + (sizeof(float) * i))
#define ADCS_TELEM3_UKF_ENABLE(i) 	(0x01C0 + (sizeof(uint8_t) * i))
#define ADCS_TELEM3_UKF_SUNMAX(i) 	(0x00E4 + (sizeof(float) * i))
#define ADCS_TELEM3_UKF_INECLIPSE 	(0x00FC)
#define ADCS_TELEM3_UKF_CHOICE	 	(0x00FD)
#define ADCS_TELEM3_UKF_CTRLT(i) 	(0x01CC + (sizeof(float) * i))
#define ADCS_TELEM3_UKF_CTRLM(i) 	(0x01D8 + (sizeof(float) * i))
#define ADCS_TELEM3_UKF_RATE(i) 	(0x01E4 + (sizeof(float) * i))
#endif





#define ADCS_TELEM4_EPHEM_JULIANDATE	(offsetof(gs_adcs_telem4_t, ephem) + offsetof(gs_adcs_ephem_t, julianDate))
#define ADCS_TELEM4_EPHEM_RECI(i)		(offsetof(gs_adcs_telem4_t, ephem) + offsetof(gs_adcs_ephem_t, scRECI) + sizeof(float) * i)
#define ADCS_TELEM4_EPHEM_VECI(i)		(offsetof(gs_adcs_telem4_t, ephem) + offsetof(gs_adcs_ephem_t, scVECI) + sizeof(float) * i)
#define ADCS_TELEM4_EPHEM_SUNECI(i)		(offsetof(gs_adcs_telem4_t, ephem) + offsetof(gs_adcs_ephem_t, sunECI) + sizeof(float) * i)
#define ADCS_TELEM4_EPHEM_MAGECI(i)		(offsetof(gs_adcs_telem4_t, ephem) + offsetof(gs_adcs_ephem_t, magECI) + sizeof(float) * i)
#define ADCS_TELEM4_EPHEM_DMAGECI(i)	(offsetof(gs_adcs_telem4_t, ephem) + offsetof(gs_adcs_ephem_t, dmagECI) + sizeof(float) * i)
#define ADCS_TELEM4_EPHEM_QUAT_IE(i)	(offsetof(gs_adcs_telem4_t, ephem) + offsetof(gs_adcs_ephem_t, rotationIE) + sizeof(float) * i)
#define ADCS_TELEM4_EPHEM_QUAT_IO(i)	(offsetof(gs_adcs_telem4_t, ephem) + offsetof(gs_adcs_ephem_t, rotationIO) + sizeof(float) * i)
#define ADCS_TELEM4_EPHEM_QUAT_IL(i)	(offsetof(gs_adcs_telem4_t, ephem) + offsetof(gs_adcs_ephem_t, rotationIL) + sizeof(float) * i)
#define ADCS_TELEM4_EPHEM_RATE_IO(i)	(offsetof(gs_adcs_telem4_t, ephem) + offsetof(gs_adcs_ephem_t, rateIO) + sizeof(float) * i)
#define ADCS_TELEM4_EPHEM_RATE_IL(i)	(offsetof(gs_adcs_telem4_t, ephem) + offsetof(gs_adcs_ephem_t, rateIL) + sizeof(float) * i)
#define ADCS_TELEM4_EPHEM_TNOECLIPSE	(offsetof(gs_adcs_telem4_t, ephem) + offsetof(gs_adcs_ephem_t, timeSinceEclipse))

#define ADCS_TELEM4_J2_EPOCH			(offsetof(gs_adcs_telem4_t, elem) + offsetof(gs_adcs_elem_t, epoch))
#define ADCS_TELEM4_J2_PECI(i)				(offsetof(gs_adcs_telem4_t, elem) + offsetof(gs_adcs_elem_t, pos_ECI) + sizeof(float) * i)
#define ADCS_TELEM4_J2_VECI(i)				(offsetof(gs_adcs_telem4_t, elem) + offsetof(gs_adcs_elem_t, vel_ECI) + sizeof(float) * i)
#define ADCS_TELEM4_J2_A				(offsetof(gs_adcs_telem4_t, elem) + offsetof(gs_adcs_elem_t, a))
#define ADCS_TELEM4_J2_N				(offsetof(gs_adcs_telem4_t, elem) + offsetof(gs_adcs_elem_t, n))
#define ADCS_TELEM4_J2_P				(offsetof(gs_adcs_telem4_t, elem) + offsetof(gs_adcs_elem_t, p))
#define ADCS_TELEM4_J2_INCL				(offsetof(gs_adcs_telem4_t, elem) + offsetof(gs_adcs_elem_t, incl))
#define ADCS_TELEM4_J2_RAAN				(offsetof(gs_adcs_telem4_t, elem) + offsetof(gs_adcs_elem_t, raan))
#define ADCS_TELEM4_J2_ARGPER				(offsetof(gs_adcs_telem4_t, elem) + offsetof(gs_adcs_elem_t, argPer))
#define ADCS_TELEM4_J2_TRUEANOM				(offsetof(gs_adcs_telem4_t, elem) + offsetof(gs_adcs_elem_t, trueAnom))
#define ADCS_TELEM4_J2_ECCANOM				(offsetof(gs_adcs_telem4_t, elem) + offsetof(gs_adcs_elem_t, eccAnom))
#define ADCS_TELEM4_J2_MEANANOM				(offsetof(gs_adcs_telem4_t, elem) + offsetof(gs_adcs_elem_t, meanAnom))

#define ADCS_TELEM5_CTRL_REFQ(i)			(offsetof(gs_adcs_telem5_t, ctrldata) + offsetof(gs_adcs_ctrl_t, ref_q) + sizeof(float) * i)
#define ADCS_TELEM5_CTRL_ERRQ(i)			(offsetof(gs_adcs_telem5_t, ctrldata) + offsetof(gs_adcs_ctrl_t, err_q) + sizeof(float) * i)
#define ADCS_TELEM5_CTRL_IERRQ(i)			(offsetof(gs_adcs_telem5_t, ctrldata) + offsetof(gs_adcs_ctrl_t, ierr_q) + sizeof(float) * i)
#define ADCS_TELEM5_CTRL_ERRRATE(i)			(offsetof(gs_adcs_telem5_t, ctrldata) + offsetof(gs_adcs_ctrl_t, err_rate) + sizeof(float) * i)
#define ADCS_TELEM5_CTRL_M(i)				(offsetof(gs_adcs_telem5_t, ctrldata) + offsetof(gs_adcs_ctrl_t, M) + sizeof(float) * i)
#define ADCS_TELEM5_CTRL_MWTORQUE(i)		(offsetof(gs_adcs_telem5_t, ctrldata) + offsetof(gs_adcs_ctrl_t, mw_torque) + sizeof(float) * i)
#define ADCS_TELEM5_CTRL_MWSPEED(i)			(offsetof(gs_adcs_telem5_t, ctrldata) + offsetof(gs_adcs_ctrl_t, mw_speed) + sizeof(float) * i)
#define ADCS_TELEM5_CTRL_MWMOMENT(i)		(offsetof(gs_adcs_telem5_t, ctrldata) + offsetof(gs_adcs_ctrl_t, mw_momentum) + sizeof(float) * i)
#define ADCS_TELEM5_CTRL_REFRATE(i)			(offsetof(gs_adcs_telem5_t, ctrldata) + offsetof(gs_adcs_ctrl_t, ref_rate) + sizeof(float) * i)
#define ADCS_TELEM5_CTRL_EULEROFF(i)		(offsetof(gs_adcs_telem5_t, ctrldata) + offsetof(gs_adcs_ctrl_t, euler_offset) + sizeof(float) * i)
#define ADCS_TELEM5_CTRL_BTORQUE(i)			(offsetof(gs_adcs_telem5_t, ctrldata) + offsetof(gs_adcs_ctrl_t, bodyTorque) + sizeof(float) * i)
#define ADCS_TELEM5_CTRL_BMOMENT(i)			(offsetof(gs_adcs_telem5_t, ctrldata) + offsetof(gs_adcs_ctrl_t, bodyMomentum) + sizeof(float) * i)

#ifndef  ADCS_FIXED_PARAM_ADD
#define ADCS_TELEM6_HTPA0_RAW0(i)				(offsetof(gs_adcs_telem6_t, htpa0) + offsetof(gs_adcs_htpa_t, raw0) + sizeof(int16_t) * i)
#define ADCS_TELEM6_HTPA0_RAW1(i)				(offsetof(gs_adcs_telem6_t, htpa0) + offsetof(gs_adcs_htpa_t, raw1) + sizeof(int16_t) * i)
#define ADCS_TELEM6_HTPA0_PTAT					(offsetof(gs_adcs_telem6_t, htpa0) + offsetof(gs_adcs_htpa_t, ptat))
#define ADCS_TELEM6_HTPA0_PTAT_COMP				(offsetof(gs_adcs_telem6_t, htpa0) + offsetof(gs_adcs_htpa_t, ptat_comp))
#define ADCS_TELEM6_HTPA1_RAW0(i)				(offsetof(gs_adcs_telem6_t, htpa1) + offsetof(gs_adcs_htpa_t, raw0) + sizeof(int16_t) * i)
#define ADCS_TELEM6_HTPA1_RAW1(i)				(offsetof(gs_adcs_telem6_t, htpa1) + offsetof(gs_adcs_htpa_t, raw1) + sizeof(int16_t) * i)
#define ADCS_TELEM6_HTPA1_PTAT 					(offsetof(gs_adcs_telem6_t, htpa1) + offsetof(gs_adcs_htpa_t, ptat))
#define ADCS_TELEM6_HTPA1_PTAT_COMP				(offsetof(gs_adcs_telem6_t, htpa1) + offsetof(gs_adcs_htpa_t, ptat_comp))
#else
#define ADCS_TELEM6_HTPA0_RAW0(i) 		(0x0000 + (sizeof(int16_t) * i))
#define ADCS_TELEM6_HTPA0_RAW1(i) 		(0x0040 + (sizeof(int16_t) * i))
#define ADCS_TELEM6_HTPA0_PTAT	 		(0x0080)
#define ADCS_TELEM6_HTPA0_PTAT_COMP	 	(0x0082)
#define ADCS_TELEM6_HTPA1_RAW0(i) 		(0x0084 + (sizeof(int16_t) * i))
#define ADCS_TELEM6_HTPA1_RAW1(i) 		(0x00C4 + (sizeof(int16_t) * i))
#define ADCS_TELEM6_HTPA1_PTAT	 		(0x0104)
#define ADCS_TELEM6_HTPA1_PTAT_COMP	 	(0x0106)
#endif

#define ADCS_TELEM7_DBG_EPHEM				(offsetof(gs_adcs_telem7_t, timing) + offsetof(adcs_timing_t, ephem_time))
#define ADCS_TELEM7_DBG_ADS					(offsetof(gs_adcs_telem7_t, timing) + offsetof(adcs_timing_t, ads_time))
#define ADCS_TELEM7_DBG_ACS					(offsetof(gs_adcs_telem7_t, timing) + offsetof(adcs_timing_t, acs_time))
#define ADCS_TELEM7_DBG_SENS				(offsetof(gs_adcs_telem7_t, timing) + offsetof(adcs_timing_t, sensor_time))

static const param_table_t adcs_param_telem1[] = {

		/* Magnetometer */
		{.name = "mag",				.addr = ADCS_TELEM1_MAG(0), 			.type = PARAM_FLOAT, 	.size = sizeof(float), 		.count = 3},
		{.name = "mag_valid",		.addr = ADCS_TELEM1_MAG_VALID,			.type = PARAM_UINT8,	.size = sizeof(uint8_t)},
		{.name = "extmag",			.addr = ADCS_TELEM1_EXTMAG(0),			.type = PARAM_FLOAT, 	.size = sizeof(float), 		.count = 3},
		{.name = "extmag_temp",		.addr = ADCS_TELEM1_EXTMAG_TEMP,			.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "extmag_valid",	.addr = ADCS_TELEM1_EXTMAG_VALID, 		.type = PARAM_UINT8,	.size = sizeof(uint8_t)},

		/* Sunsensor */
		{.name = "suns",			.addr = ADCS_TELEM1_SUNS(0),				.type = PARAM_FLOAT, 	.size = sizeof(float), 		.count = 6},
		{.name = "suns_raw",		.addr = ADCS_TELEM1_SUNS_RAW(0),			.type = PARAM_UINT16, 	.size = sizeof(uint16_t), 	.count = 6},
		{.name = "suns_valid",		.addr = ADCS_TELEM1_SUNS_VALID, 			.type = PARAM_UINT8,	.size = sizeof(uint8_t)},
		{.name = "suns_temp",		.addr = ADCS_TELEM1_SUNS_TEMP(0),		.type = PARAM_FLOAT, 	.size = sizeof(float), 		.count = 6},

		/* Gyro */
		{.name = "gyro",			.addr = ADCS_TELEM1_GYRO(0),			.type = PARAM_FLOAT, 	.size = sizeof(float), 		.count = 3},
		{.name = "gyro_norm",		.addr = ADCS_TELEM1_GYRO_NORM(0),		.type = PARAM_FLOAT, 	.size = sizeof(float), 		.count = 3},
		{.name = "gyro_trend",		.addr = ADCS_TELEM1_GYRO_TREND(0),		.type = PARAM_FLOAT, 	.size = sizeof(float), 		.count = 3},
		{.name = "gyro_temp",		.addr = ADCS_TELEM1_GYRO_TEMP,			.type = PARAM_FLOAT, 	.size = sizeof(float)},
		{.name = "gyro_valid",		.addr = ADCS_TELEM1_GYRO_VALID,		 	.type = PARAM_UINT8,	.size = sizeof(uint8_t)},

		/* Star Trackers */
		{.name = "st1",				.addr = ADCS_TELEM1_ST1(0),				.type = PARAM_FLOAT, 	.size = sizeof(float), 		.count = 4},
		{.name = "st1_valid",		.addr = ADCS_TELEM1_ST1_VALID,			.type = PARAM_INT8, 	.size = sizeof(int8_t)},
		{.name = "st2",				.addr = ADCS_TELEM1_ST2(0),				.type = PARAM_FLOAT, 	.size = sizeof(float), 		.count = 4},
		{.name = "st2_valid",		.addr = ADCS_TELEM1_ST2_VALID,			.type = PARAM_INT8, 	.size = sizeof(int8_t)},

		/* Fine-sunsensor */
		{.name = "fss",				.addr = ADCS_TELEM1_FSS(0, 0),			.type = PARAM_FLOAT, 	.size = sizeof(float), 		.count = 8 * 3},
		{.name = "fss_body",		.addr = ADCS_TELEM1_FSS_BODY(0, 0),	.type = PARAM_FLOAT, 	.size = sizeof(float), 		.count = 8 * 3},
		{.name = "fss_raw",			.addr = ADCS_TELEM1_FSS_RAW(0, 0),		.type = PARAM_UINT16, 	.size = sizeof(uint16_t), 	.count = 8 * 4},
		{.name = "fss_temp",		.addr = ADCS_TELEM1_FSS_TEMP(0),			.type = PARAM_FLOAT, 	.size = sizeof(float), 		.count = 8},
		{.name = "fss_valid",		.addr = ADCS_TELEM1_FSS_VALID(0),	 	.type = PARAM_UINT8,	.size = sizeof(uint8_t),	.count = 8},

		/* GPS */
		{.name = "gps_pos",			.addr = ADCS_TELEM1_GPS_POS(0), 			.type = PARAM_FLOAT, 	.size = sizeof(float), 		.count = 3},
		{.name = "gps_vel",			.addr = ADCS_TELEM1_GPS_VEL(0), 			.type = PARAM_FLOAT, 	.size = sizeof(float), 		.count = 3},
		{.name = "gps_pos_dev",		.addr = ADCS_TELEM1_GPS_POS_DEV(0), 		.type = PARAM_FLOAT, 	.size = sizeof(float), 		.count = 3},
		{.name = "gps_vel_dev",		.addr = ADCS_TELEM1_GPS_VEL_DEV(0), 		.type = PARAM_FLOAT, 	.size = sizeof(float), 		.count = 3},
		{.name = "gps_epoch",		.addr = ADCS_TELEM1_GPS_EPOCH,	 		.type = PARAM_UINT32, 	.size = sizeof(uint32_t)},
		{.name = "gps_julian",		.addr = ADCS_TELEM1_GPS_JULIAN,	 		.type = PARAM_DOUBLE, 	.size = sizeof(double)},
		{.name = "gps_valid",		.addr = ADCS_TELEM1_GPS_VALID,		 	.type = PARAM_UINT8,	.size = sizeof(uint8_t)},
		{.name = "gps_sat",			.addr = ADCS_TELEM1_GPS_SAT,		 	.type = PARAM_UINT8,	.size = sizeof(uint8_t)},
		{.name = "gps_satsol",		.addr = ADCS_TELEM1_GPS_SATSOL,		 	.type = PARAM_UINT8,	.size = sizeof(uint8_t)},
		{.name = "tdiff_jdate",		.addr = ADCS_TELEM1_GPS_TDIFF_JD,	 		.type = PARAM_DOUBLE, 	.size = sizeof(double)},
		{.name = "tdiff_unix",		.addr = ADCS_TELEM1_GPS_TDIFF_UT,	 		.type = PARAM_UINT32, 	.size = sizeof(uint32_t)},
		{.name = "pps_unix",		.addr = ADCS_TELEM1_GPS_PPS_UT,	 		.type = PARAM_UINT32, 	.size = sizeof(uint32_t)},
		{.name = "pps_jdate",		.addr = ADCS_TELEM1_GPS_PPS_JD,	 		.type = PARAM_DOUBLE, 	.size = sizeof(double)},

		/* Actuators */
		{.name = "torquer_duty",	.addr = ADCS_TELEM1_TORQUERDUTY(0), 	.type = PARAM_FLOAT, 	.size = sizeof(float), 		.count = 3},
		{.name = "torquer_calib",	.addr = ADCS_TELEM1_TORQUERCALIB(0), 	.type = PARAM_FLOAT, 	.size = sizeof(float), 		.count = 3},
		{.name = "wheel_torque",	.addr = ADCS_TELEM1_WHEELTORQUE(0),		.type = PARAM_FLOAT, 	.size = sizeof(float), 		.count = 4},
		{.name = "wheel_momen",		.addr = ADCS_TELEM1_WHEELMOMENTUM(0), 	.type = PARAM_FLOAT, 	.size = sizeof(float), 		.count = 4},
		{.name = "wheel_speed",		.addr = ADCS_TELEM1_WHEELSPEED(0), 		.type = PARAM_FLOAT, 	.size = sizeof(float), 		.count = 4},
		{.name = "wheel_enable",	.addr = ADCS_TELEM1_WHEELENABLE(0),		.type = PARAM_UINT8, 	.size = sizeof(uint8_t), 	.count = 4},

};



static const param_table_t adcs_param_telem2[] = {
		{.name = "acs_mode",	.addr = ADCS_TELEM2_ACS_MODE, 		.type = PARAM_INT8,	.size = sizeof(int8_t)},
		{.name = "acs_dmode",	.addr = ADCS_TELEM2_ACS_DMODE, 		.type = PARAM_INT8,	.size = sizeof(int8_t)},
		{.name = "ads_mode",	.addr = ADCS_TELEM2_ADS_MODE, 		.type = PARAM_INT8,	.size = sizeof(int8_t)},
		{.name = "ads_dmode",	.addr = ADCS_TELEM2_ADS_DMODE, 		.type = PARAM_INT8,	.size = sizeof(int8_t)},
		{.name = "ephem_mode",	.addr = ADCS_TELEM2_EPHEM_MODE, 		.type = PARAM_INT8,	.size = sizeof(int8_t)},
		{.name = "ephem_dmode",	.addr = ADCS_TELEM2_EPHEM_DMODE, 		.type = PARAM_INT8,	.size = sizeof(int8_t)},
		{.name = "yspin_mode",	.addr = ADCS_TELEM2_YSPIN_MODE, 		.type = PARAM_INT8,	.size = sizeof(int8_t)},
		{.name = "status_mag",	.addr = ADCS_TELEM2_STA_MAG, 		.type = PARAM_INT8,	.size = sizeof(int8_t)},
		{.name = "status_exmag",.addr = ADCS_TELEM2_STA_EXTMAG, 	.type = PARAM_INT8,	.size = sizeof(int8_t)},
		{.name = "status_css",	.addr = ADCS_TELEM2_STA_CSS, 		.type = PARAM_INT8,	.size = sizeof(int8_t)},
		{.name = "status_fss",	.addr = ADCS_TELEM2_STA_FSS(0), 	.type = PARAM_INT8,	.size = sizeof(int8_t), .count = 8},
		{.name = "status_gyro",	.addr = ADCS_TELEM2_STA_GYRO, 		.type = PARAM_INT8,	.size = sizeof(int8_t)},
		{.name = "status_gps",	.addr = ADCS_TELEM2_STA_GPS, 		.type = PARAM_INT8,	.size = sizeof(int8_t)},
		{.name = "status_htpa",	.addr = ADCS_TELEM2_STA_HTPA(0), 	.type = PARAM_INT8,	.size = sizeof(int8_t), .count = 2},
		{.name = "status_bdot",	.addr = ADCS_TELEM2_STA_BDOT, 		.type = PARAM_INT8,	.size = sizeof(int8_t)},
		{.name = "status_ukf",	.addr = ADCS_TELEM2_STA_UKF, 		.type = PARAM_INT8,	.size = sizeof(int8_t)},
		{.name = "status_igrf",	.addr = ADCS_TELEM2_STA_IGRF, 		.type = PARAM_INT8,	.size = sizeof(int8_t)},
		{.name = "status_sgp4",	.addr = ADCS_TELEM2_STA_SGP4, 		.type = PARAM_INT8,	.size = sizeof(int8_t)},
		{.name = "status_j2ele",.addr = ADCS_TELEM2_STA_J2ELE, 		.type = PARAM_INT8,	.size = sizeof(int8_t)},
		{.name = "status_j2pro",.addr = ADCS_TELEM2_STA_J2PRO, 		.type = PARAM_INT8,	.size = sizeof(int8_t)},
		{.name = "status_etime",.addr = ADCS_TELEM2_STA_ETIME, 		.type = PARAM_INT8,	.size = sizeof(int8_t)},
		{.name = "status_ephem",.addr = ADCS_TELEM2_STA_EPHEM, 		.type = PARAM_INT8,	.size = sizeof(int8_t)},
		{.name = "status_run",	.addr = ADCS_TELEM2_STA_RUN, 		.type = PARAM_INT8,	.size = sizeof(int8_t)},
		{.name = "looptime",	.addr = ADCS_TELEM2_TLOOP, 			.type = PARAM_UINT16,	.size = sizeof(uint16_t)},
		{.name = "maxlooptime",	.addr = ADCS_TELEM2_MAXTLOOP, 		.type = PARAM_UINT16,	.size = sizeof(uint16_t)},

		{.name = "bdot_rate",	.addr = ADCS_TELEM2_BDOT_RATE(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 2},
		{.name = "bdot_dmag",	.addr = ADCS_TELEM2_BDOT_DMAG(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 3},
		{.name = "bdot_torquer",.addr = ADCS_TELEM2_BDOT_TORQUER(0),.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 3},
		{.name = "bdot_detumb", .addr = ADCS_TELEM2_BDOT_DETUMBLED, .type = PARAM_UINT8, .size = sizeof(uint8_t)},

		{.name = "dbtask",		.addr = ADCS_TELEM2_DEBUG_TASK, 	.type = PARAM_UINT8,	.size = sizeof(uint8_t)},
		{.name = "dbmain",		.addr = ADCS_TELEM2_DEBUG_MAIN, 	.type = PARAM_UINT8,	.size = sizeof(uint8_t)},
};


static const param_table_t adcs_param_telem3[] = {
		{.name = "ukf_X",			.addr = ADCS_TELEM3_UKF_X(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = UKF_MAX_NUM_X},
		{.name = "ukf_q",			.addr = ADCS_TELEM3_UKF_Q(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 4},
		{.name = "ukf_w",			.addr = ADCS_TELEM3_UKF_W(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 3},
		{.name = "ukf_Pdiag",		.addr = ADCS_TELEM3_UKF_PDIAG(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = UKF_MAX_NUM_ERR},
		{.name = "ukf_Xpred",		.addr = ADCS_TELEM3_UKF_XPRED(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = UKF_MAX_NUM_X},
		{.name = "ukf_Zpred",		.addr = ADCS_TELEM3_UKF_ZPRED(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = UKF_MAX_NUM_Z},
		{.name = "ukf_Z",			.addr = ADCS_TELEM3_UKF_Z(0), 	    .type = PARAM_FLOAT, .size = sizeof(float), 	.count = UKF_MAX_NUM_Z},
		{.name = "ukf_enable",		.addr = ADCS_TELEM3_UKF_ENABLE(0), 	.type = PARAM_UINT8, .size = sizeof(uint8_t), 	.count = UKF_MAX_NUM_Z},
		{.name = "ukf_sunmax",		.addr = ADCS_TELEM3_UKF_SUNMAX(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 6},
		{.name = "ukf_ineclipse",	.addr = ADCS_TELEM3_UKF_INECLIPSE, 	.type = PARAM_UINT8, .size = sizeof(uint8_t)},
		{.name = "ukf_choice",		.addr = ADCS_TELEM3_UKF_CHOICE, 	.type = PARAM_UINT8, .size = sizeof(uint8_t)},
		{.name = "ukf_ctrlT",		.addr = ADCS_TELEM3_UKF_CTRLT(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 3},
		{.name = "ukf_ctrlM",		.addr = ADCS_TELEM3_UKF_CTRLM(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 3},
		{.name = "ukf_rate",		.addr = ADCS_TELEM3_UKF_RATE(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 3},
};

static const param_table_t adcs_param_telem4[] = {
		{.name = "ephem_jdate",		.addr = ADCS_TELEM4_EPHEM_JULIANDATE,	.type = PARAM_DOUBLE, .size = sizeof(double)},
		{.name = "ephem_reci",		.addr = ADCS_TELEM4_EPHEM_RECI(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 3},
		{.name = "ephem_veci",		.addr = ADCS_TELEM4_EPHEM_VECI(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 3},
		{.name = "ephem_suneci",	.addr = ADCS_TELEM4_EPHEM_SUNECI(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 3},
		{.name = "ephem_mageci",	.addr = ADCS_TELEM4_EPHEM_MAGECI(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 3},
		{.name = "ephem_dmageci",	.addr = ADCS_TELEM4_EPHEM_DMAGECI(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 3},
		{.name = "ephem_quat_ie",		.addr = ADCS_TELEM4_EPHEM_QUAT_IE(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 4},
		{.name = "ephem_quat_io",		.addr = ADCS_TELEM4_EPHEM_QUAT_IE(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 4},
		{.name = "ephem_quat_il",		.addr = ADCS_TELEM4_EPHEM_QUAT_IE(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 4},
		{.name = "ephem_rate_io",		.addr = ADCS_TELEM4_EPHEM_RATE_IO(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 4},
		{.name = "ephem_rate_il",		.addr = ADCS_TELEM4_EPHEM_RATE_IL(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 4},
		{.name = "ephem_t_eclip",		.addr = ADCS_TELEM4_EPHEM_TNOECLIPSE, 	.type = PARAM_INT32, .size = sizeof(int)},
		{.name = "j2_epoch",		.addr = ADCS_TELEM4_J2_EPOCH, 	.type = PARAM_DOUBLE, .size = sizeof(double)},
		{.name = "j2_peci",			.addr = ADCS_TELEM4_J2_PECI(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 3},
		{.name = "j2_veci",			.addr = ADCS_TELEM4_J2_VECI(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 3},
		{.name = "j2_a",			.addr = ADCS_TELEM4_J2_A, 			.type = PARAM_FLOAT, .size = sizeof(float)},
		{.name = "j2_n",			.addr = ADCS_TELEM4_J2_N, 			.type = PARAM_FLOAT, .size = sizeof(float)},
		{.name = "j2_p",			.addr = ADCS_TELEM4_J2_P, 			.type = PARAM_FLOAT, .size = sizeof(float)},
		{.name = "j2_incl",			.addr = ADCS_TELEM4_J2_INCL, 			.type = PARAM_FLOAT, .size = sizeof(float)},
		{.name = "j2_raan",			.addr = ADCS_TELEM4_J2_RAAN, 			.type = PARAM_FLOAT, .size = sizeof(float)},
		{.name = "j2_argper",			.addr = ADCS_TELEM4_J2_ARGPER, 			.type = PARAM_FLOAT, .size = sizeof(float)},
		{.name = "j2_trueanom",			.addr = ADCS_TELEM4_J2_TRUEANOM, 			.type = PARAM_FLOAT, .size = sizeof(float)},
		{.name = "j2_eccanom",			.addr = ADCS_TELEM4_J2_ECCANOM, 			.type = PARAM_FLOAT, .size = sizeof(float)},
		{.name = "j2_meananom",			.addr = ADCS_TELEM4_J2_MEANANOM, 			.type = PARAM_FLOAT, .size = sizeof(float)},
};

static const param_table_t adcs_param_telem5[] = {
		{.name = "ctrl_refq",		.addr = ADCS_TELEM5_CTRL_REFQ(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 4},
		{.name = "ctrl_errq",		.addr = ADCS_TELEM5_CTRL_ERRQ(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 4},
		{.name = "ctrl_ierrq",		.addr = ADCS_TELEM5_CTRL_IERRQ(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 4},
		{.name = "ctrl_errrate",	.addr = ADCS_TELEM5_CTRL_ERRRATE(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 3},
		{.name = "ctrl_M",			.addr = ADCS_TELEM5_CTRL_M(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 3},
		{.name = "ctrl_mwtorque",	.addr = ADCS_TELEM5_CTRL_MWTORQUE(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 4},
		{.name = "ctrl_mwspeed",	.addr = ADCS_TELEM5_CTRL_MWSPEED(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 4},
		{.name = "ctrl_mwmoment",	.addr = ADCS_TELEM5_CTRL_MWMOMENT(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 4},
		{.name = "ctrl_refrate",	.addr = ADCS_TELEM5_CTRL_REFRATE(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 3},
		{.name = "ctrl_euleroff",	.addr = ADCS_TELEM5_CTRL_EULEROFF(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 3},
		{.name = "ctrl_btorque",	.addr = ADCS_TELEM5_CTRL_BTORQUE(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 3},
		{.name = "ctrl_bmoment",	.addr = ADCS_TELEM5_CTRL_BMOMENT(0), 	.type = PARAM_FLOAT, .size = sizeof(float), 	.count = 3},
};

static const param_table_t adcs_param_telem6[] = {
		{.name = "htpa0_raw0",		.addr = ADCS_TELEM6_HTPA0_RAW0(0), 	.type = PARAM_INT16, .size = sizeof(int16_t), 	.count = 16*2},
		{.name = "htpa0_raw1",		.addr = ADCS_TELEM6_HTPA0_RAW1(0), 	.type = PARAM_INT16, .size = sizeof(int16_t), 	.count = 16*2},
		{.name = "htpa0_ptat",		.addr = ADCS_TELEM6_HTPA0_PTAT, 	.type = PARAM_INT16, .size = sizeof(int16_t)},
		{.name = "htpa0_ptat_c",	.addr = ADCS_TELEM6_HTPA0_PTAT_COMP, 	.type = PARAM_INT16, .size = sizeof(int16_t)},
		{.name = "htpa1_raw0",		.addr = ADCS_TELEM6_HTPA1_RAW0(0), 	.type = PARAM_INT16, .size = sizeof(int16_t), 	.count = 16*2},
		{.name = "htpa1_raw1",		.addr = ADCS_TELEM6_HTPA1_RAW1(0), 	.type = PARAM_INT16, .size = sizeof(int16_t), 	.count = 16*2},
		{.name = "htpa1_ptat",		.addr = ADCS_TELEM6_HTPA1_PTAT, 	.type = PARAM_INT16, .size = sizeof(int16_t)},
		{.name = "htpa1_ptat_c",	.addr = ADCS_TELEM6_HTPA1_PTAT_COMP, 	.type = PARAM_INT16, .size = sizeof(int16_t)},
};

static const param_table_t adcs_param_telem7[] = {
		{.name = "ephem_time",		.addr = ADCS_TELEM7_DBG_EPHEM,			.type = PARAM_UINT32, 	.size = sizeof(uint32_t)},
		{.name = "ads_time",		.addr = ADCS_TELEM7_DBG_ADS,			.type = PARAM_UINT32, 	.size = sizeof(uint32_t)},
		{.name = "acs_time",		.addr = ADCS_TELEM7_DBG_ACS,			.type = PARAM_UINT32, 	.size = sizeof(uint32_t)},
		{.name = "sens_time",		.addr = ADCS_TELEM7_DBG_SENS,			.type = PARAM_UINT32, 	.size = sizeof(uint32_t)},
};


#define ADCS_PARAM_TELEM1_COUNT (sizeof(adcs_param_telem1) / sizeof(adcs_param_telem1[0]))
#define ADCS_PARAM_TELEM2_COUNT (sizeof(adcs_param_telem2) / sizeof(adcs_param_telem2[0]))
#define ADCS_PARAM_TELEM3_COUNT (sizeof(adcs_param_telem3) / sizeof(adcs_param_telem3[0]))
#define ADCS_PARAM_TELEM4_COUNT (sizeof(adcs_param_telem4) / sizeof(adcs_param_telem4[0]))
#define ADCS_PARAM_TELEM5_COUNT (sizeof(adcs_param_telem5) / sizeof(adcs_param_telem5[0]))
#define ADCS_PARAM_TELEM6_COUNT (sizeof(adcs_param_telem6) / sizeof(adcs_param_telem6[0]))
#define ADCS_PARAM_TELEM7_COUNT (sizeof(adcs_param_telem7) / sizeof(adcs_param_telem7[0]))

#define ADCS_PARAM_TELEM1_MEMSIZE			sizeof(gs_adcs_telem1_t)
#define ADCS_PARAM_TELEM2_MEMSIZE			sizeof(gs_adcs_telem2_t)
#define ADCS_PARAM_TELEM3_MEMSIZE			sizeof(gs_adcs_telem3_t)
#define ADCS_PARAM_TELEM4_MEMSIZE			sizeof(gs_adcs_telem4_t)
#define ADCS_PARAM_TELEM5_MEMSIZE			sizeof(gs_adcs_telem5_t)
#define ADCS_PARAM_TELEM6_MEMSIZE			sizeof(gs_adcs_telem6_t)
#define ADCS_PARAM_TELEM7_MEMSIZE			sizeof(gs_adcs_telem7_t)

#endif /* ADCS_TELEM_H_ */
