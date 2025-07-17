/*
 * adcs_client_gps.h
 *
 *  Created on: Aug 13, 2015
 *      Author: holst
 */

#ifndef CLIENT_GPS_H_
#define CLIENT_GPS_H_

#include <adcs/adcs_types.h>


/* GPS functions */
int adcs_gps_get_sample(adcs_gps_sample_t *sample);
int adcs_gps_reset(void);
int adcs_gps_on(void);
int adcs_gps_off(void);
int adcs_gps_ascii(gps_ascii_cmd_t *cmd);
int adcs_gps_trackstat(void);
int adcs_gps_rxstat(adcs_gps_rxstatus_t *rxstatus);
int adcs_gps_temp(adcs_float_t *temp);
int adcs_gps_ecutoff(float value);
int adcs_gps_last_sample(adcs_gps_last_sample_t *data);
int adcs_gps_last_pps(adcs_gps_last_pps_t *time);
void adcs_gps_node_set(int node, int port);


#endif /* LIB_LIBADCS_CLIENT_ADCS_ADCS_CLIENT_GPS_H_ */
