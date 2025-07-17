/*
 * client_adcs_gps.c
 *
 *  Created on: Aug 13, 2015
 *      Author: holst
 */

#include <stdio.h>
#include <csp/csp.h>
#include <csp/csp_endian.h>

#include <util/log.h>
#include <util/hexdump.h>
#include <util/byteorder.h>

#include <adcs/adcs_types.h>
#include <adcs/adcs_param.h>
#include <adcs/adcs_client_gps.h>

#define ADCS_CSP_TIMEOUT 2000
#define ADCS_GPS_CSP_TIMEOUT 10000

static int node_gps = 4;
static int port_gps = 20;

void adcs_gps_node_set(int node, int port) {
	node_gps = node;
	port_gps = port;
}

/* GPS commands */
int adcs_gps_get_sample(adcs_gps_sample_t *sample) {
    int retval;
    adcs_server_packet_t p;
    p.cmd = ADCS_CMD_GPS_SAMPLE;
    retval = csp_transaction(CSP_PRIO_NORM, node_gps, port_gps, ADCS_GPS_CSP_TIMEOUT, &p, ADCS_SERVER_PACKET_SIZE(0), &p, ADCS_SERVER_PACKET_SIZE(adcs_gps_sample_t));
    if (retval != ADCS_SERVER_PACKET_SIZE(adcs_gps_sample_t)) {
        retval = 0;
    }
    sample->jdate = util_ntohdbl(p.gps.sample.jdate);
    for (int i = 0; i < 3; i++) {
        sample->posxyz[i] = util_ntohflt(p.gps.sample.posxyz[i]);
        sample->velxyz[i] = util_ntohflt(p.gps.sample.velxyz[i]);
    }
    sample->satellitesTrack = p.gps.sample.satellitesTrack;
    sample->satellitesSol = p.gps.sample.satellitesSol;
    return retval;
}

int adcs_gps_reset(void){
	int retval;
	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_GPS_RESET;
	retval = csp_transaction(CSP_PRIO_NORM, node_gps, port_gps, ADCS_GPS_CSP_TIMEOUT, &p, ADCS_SERVER_PACKET_SIZE(0), &p, ADCS_SERVER_PACKET_SIZE(0));
	if (retval != ADCS_SERVER_PACKET_SIZE(0)) {
		retval = 0;
	}
	return retval;
}

int adcs_gps_on() {
    int retval;
    adcs_server_packet_t p;
    p.cmd = ADCS_CMD_GPS_ON;
    retval = csp_transaction(CSP_PRIO_NORM, node_gps, port_gps, ADCS_CSP_TIMEOUT, &p, ADCS_SERVER_PACKET_SIZE(0), &p, ADCS_SERVER_PACKET_SIZE(0));
    if (retval != ADCS_SERVER_PACKET_SIZE(0)) {
        retval = 0;
    }
    return retval;
}

int adcs_gps_off(){
	int retval;
	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_GPS_OFF;
	retval = csp_transaction(CSP_PRIO_NORM, node_gps, port_gps, ADCS_CSP_TIMEOUT, &p, ADCS_SERVER_PACKET_SIZE(0), &p, ADCS_SERVER_PACKET_SIZE(0));
	if (retval != ADCS_SERVER_PACKET_SIZE(0)) {
		retval = 0;
	}
	return retval;
}

int adcs_gps_rxstat(adcs_gps_rxstatus_t *rxstatus){
    int retval;
    adcs_server_packet_t p;
    p.cmd = ADCS_CMD_GPS_RXSTAT;
    retval = csp_transaction(CSP_PRIO_NORM, node_gps, port_gps, ADCS_CSP_TIMEOUT, &p, ADCS_SERVER_PACKET_SIZE(NULL), &p, ADCS_SERVER_PACKET_SIZE(adcs_gps_rxstatus_t));
    if (retval != ADCS_SERVER_PACKET_SIZE(adcs_gps_rxstatus_t)) {
        retval = 0;
    }
    rxstatus->error = util_ntoh32(p.gps.rxstat.error);
    rxstatus->nr_stats = util_ntoh32(p.gps.rxstat.nr_stats);
    rxstatus->rxstat = util_ntoh32(p.gps.rxstat.rxstat);
    rxstatus->aux1stat = util_ntoh32(p.gps.rxstat.aux1stat);
    rxstatus->aux2stat = util_ntoh32(p.gps.rxstat.aux2stat);
    rxstatus->aux3stat = util_ntoh32(p.gps.rxstat.aux3stat);
    return retval;
}

int adcs_gps_temp(adcs_float_t *temp){
    int retval;
    adcs_server_packet_t p;
    p.cmd = ADCS_CMD_GPS_TEMP;
    retval = csp_transaction(CSP_PRIO_NORM, node_gps, port_gps, ADCS_CSP_TIMEOUT, &p, ADCS_SERVER_PACKET_SIZE(NULL), &p, ADCS_SERVER_PACKET_SIZE(adcs_gps_temp_t));
    if (retval != ADCS_SERVER_PACKET_SIZE(adcs_gps_temp_t)) {
        retval = 0;
        *temp = 0;
    }
    else {
        *temp = util_ntohflt(p.gps.temperature);
    }
    return retval;
}

int adcs_gps_ecutoff(float value){
    int retval;
    adcs_server_packet_t p;
    p.cmd = ADCS_CMD_GPS_ECUTOFF;
    p.gps.cutoff_angle =  util_htonflt(value);
    retval = csp_transaction(CSP_PRIO_NORM, node_gps, port_gps, ADCS_CSP_TIMEOUT, &p, ADCS_SERVER_PACKET_SIZE(adcs_gps_temp_t), &p, ADCS_SERVER_PACKET_SIZE(0));
    if (retval != ADCS_SERVER_PACKET_SIZE(0)) {
        retval = 0;
    }
    return retval;
}

int adcs_gps_last_sample(adcs_gps_last_sample_t *data)
{
    int retval;
    adcs_server_packet_t p;
    p.cmd = ADCS_CMD_GPS_GETPOS;
    retval = csp_transaction(CSP_PRIO_NORM, node_gps, port_gps, ADCS_CSP_TIMEOUT, &p, ADCS_SERVER_PACKET_SIZE(0), &p, ADCS_SERVER_PACKET_SIZE(adcs_gps_last_sample_t));
    if (retval == ADCS_SERVER_PACKET_SIZE(adcs_gps_last_sample_t)) {
        for (int i=0; i<3; i++) {
            data->posxyz[i] = util_ntoh32(p.gps.lastsample.posxyz[i]);
            data->velxyz[i] = util_ntoh32(p.gps.lastsample.velxyz[i]);
        }
        data->valid = p.gps.lastsample.valid;
        data->jdate = util_ntoh64(p.gps.lastsample.jdate);
    }
    else {
        memset(data,0,sizeof(data));
    }
    return retval;
}

int adcs_gps_last_pps(adcs_gps_last_pps_t *time)
{
    int retval;
    adcs_server_packet_t p;
    p.cmd = ADCS_CMD_GPS_GETTIME;
    retval = csp_transaction(CSP_PRIO_NORM, node_gps, port_gps, ADCS_CSP_TIMEOUT, &p, ADCS_SERVER_PACKET_SIZE(0), &p, ADCS_SERVER_PACKET_SIZE(adcs_gps_last_pps_t));
    if (retval == ADCS_SERVER_PACKET_SIZE(adcs_gps_last_pps_t)) {
        time->valid = p.gps.lastpps.valid;
        time->jdate = util_ntoh64(p.gps.lastpps.jdate);
        time->unixtime = util_ntoh32(p.gps.lastpps.unixtime);
    }
    else {
        memset(time,0,sizeof(time));
    }
    return retval;
}

int adcs_gps_ascii(gps_ascii_cmd_t *cmd){

	/* Host to network */
	/* Connect */
	csp_conn_t * conn = csp_connect(CSP_PRIO_NORM, node_gps, port_gps, ADCS_CSP_TIMEOUT, 0);
	csp_packet_t * packet = csp_buffer_get(ADCS_SERVER_PACKET_SIZE(gps_ascii_cmd_t));

	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_GPS_ASCII;
	p.gps.cmd.length = csp_hton16(cmd->length);
	p.gps.cmd.timeout = csp_hton16(cmd->timeout);
	memset(p.gps.cmd.data, 0, sizeof(p.gps.cmd.data)); // Clear string
	memcpy(p.gps.cmd.data, cmd->data, cmd->length);

	/* Packet */
	packet->length = ADCS_SERVER_PACKET_SIZE(gps_ascii_cmd_t);
	memcpy(packet->data, &p, sizeof(p.gps.cmd));
	/* Send packet */
	if (!csp_send(conn, packet, 0)) {
		csp_buffer_free(packet);
		csp_close(conn);
		return -1;
	}

	gps_csp_res_t res;
	printf("Response: \r\n");
	/* Wait for replies */
	while ((packet = csp_read(conn, ADCS_GPS_CSP_TIMEOUT)) != NULL) {
		memcpy(&res, packet->data, packet->length);
		csp_buffer_free(packet);
		printf("%.*s", res.len, res.data);
		if (res.len < 180)
			break;
	}
	printf("\r\n");
	csp_close(conn);
	return 0;
}

int adcs_gps_trackstat( void ){
	/* Host to network */
	/* Connect */
	csp_conn_t * conn = csp_connect(CSP_PRIO_NORM, node_gps, port_gps, ADCS_CSP_TIMEOUT, 0);
	csp_packet_t * packet = csp_buffer_get(ADCS_SERVER_PACKET_SIZE(176));

	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_GPS_TRACKSTAT;

	/* Packet */
	packet->length = ADCS_SERVER_PACKET_SIZE(176);
	memcpy(packet->data, &p, sizeof(p));
	/* Send packet */
	if (!csp_send(conn, packet, 0)) {
		csp_buffer_free(packet);
		csp_close(conn);
		return -1;
	}

	adcs_gps_trackstat_t * res;

	printf("Response: \r\n");
	/* Wait for replies */
	uint8_t packets_recieved = 0;
	while ((packet = csp_read(conn, ADCS_GPS_CSP_TIMEOUT)) != NULL) {
		res = (adcs_gps_trackstat_t *) packet->data;

		/* Invalid packet headers are stuffed as 0xfff... */
		if (util_ntoh32(res->sol_status) == 0xffffffff){
			printf("No reply\r\n");
			csp_buffer_free(packet);
			csp_close(conn);
			break;
		}

		/* HEADER */
		if (packets_recieved == 0){ // First header only
			printf("Solution status: %"PRIu32"", util_ntoh32(res->sol_status));

			if (!(util_ntoh32(res->sol_status)))
				printf("\E[0;32m Valid \E[0m \r\n");
			else
				printf("\E[1;31m Not valid \E[0m \r\n");

			printf("Cutoff angle: %f\r\n", util_ntohflt(res->cutoff));
			printf("Position type: %"PRIu32"\r\n", util_ntoh32(res->pos_type));
			printf("Num of chan: %"PRIu32"\r\n", util_ntoh32(res->num_of_chans));
		}

		/* SATELLITES*/
		for (int i=0; i<4; i++){ // 4 satellites per packet
			if (util_ntoh16(res->sat[i].prn) != 0){

				if (util_ntoh32(res->sat[i].reject) == 0) // Valid
					printf("\E[0;32m");
				else if (util_ntoh32(res->sat[i].reject) == 99) // No ephem
					printf("\E[0;31m");
				else
					printf("\E[0;33m");
				printf("PRN: %i ", util_ntoh16(res->sat[i].prn));
				printf("Dplr: %f ", util_ntohflt(res->sat[i].doppler));
				printf("CNR: %f ", util_ntohflt(res->sat[i].c_no));
				printf("Lck: %f[s] ", util_ntohflt(res->sat[i].locktime));
				printf("Rjct: %"PRIu32" ", util_ntoh32(res->sat[i].reject));
				if (util_ntoh32(res->sat[i].reject) == 0)
					printf("\E[0;35m \t PSRr: %f PSRd: %f PSRw %f",util_ntohdbl(res->sat[i].psr), util_ntohflt(res->sat[i].psr_res), util_ntohflt(res->sat[i].psr_weigth));
				printf("\E[0m \r\n");
			}
		}

		csp_buffer_free(packet);
		packets_recieved++;
		if (packets_recieved > 4)
			break;
	}
	printf("\r\n");
	csp_close(conn);
	return 0;
}
