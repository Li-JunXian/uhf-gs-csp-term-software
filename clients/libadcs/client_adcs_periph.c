/*
 * client_adcs_periph.c
 *
 *  Created on: Jan 23, 2017
 *      Author: holst
 */

#include <stdio.h>
#include <csp/csp.h>
#include <csp/csp_endian.h>

#include <util/log.h>
#include <util/hexdump.h>
#include <util/byteorder.h>

#include "adcs/adcs_types.h"
#include "adcs/adcs_param.h"

#define ADCS_CSP_TIMEOUT 2000
#define ADCS_GPS_CSP_TIMEOUT 10000

/* Node / Server declaration */
static int node_adcs = 4;
static int port_adcs = 20;
static int timeout_adcs = ADCS_CSP_TIMEOUT;

void adcs_periph_node_set(int node, int port) {
	node_adcs = node;
	port_adcs = port;
}

void adcs_periph_timeout_set(int timeout) {
	timeout_adcs = timeout;
}

/* Sensor commands */
int adcs_sensor_init(void){
	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_SENSOR_INIT;
	return csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs, &p, ADCS_SERVER_PACKET_SIZE(0), NULL, 0);
}

/* Act commands */
int adcs_act_mw_get_status(adcs_act_mw_get_status_t *value){
	int retval;
	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_ACT_MW_GET_STAT;
	retval = csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs,
			&p, ADCS_SERVER_PACKET_SIZE(NULL),
			&p,	ADCS_SERVER_PACKET_SIZE(adcs_act_mw_get_status_t));
	if (retval == ADCS_SERVER_PACKET_SIZE(adcs_act_mw_get_status_t)) {
		for (int i=0; i<4; i++){
			value->enable[i] = p.mw.mw_status.enable[i];
			value->speed[i] = util_ntohflt(p.mw.mw_status.speed[i]);
			value->torque[i] = util_ntohflt(p.mw.mw_status.torque[i]);
		}
	}
	else
		retval = 0;
	return retval;
}

int adcs_act_mw_set(adcs_act_mw_set_t value){
	int retval;
	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_ACT_MW_SET;
	p.mw.mw_set.mw = value.mw;
	p.mw.mw_set.torque = util_htonflt(value.torque);
	p.mw.mw_set.Ts = util_hton32(value.Ts);

	retval = csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs,
			&p, ADCS_SERVER_PACKET_SIZE(adcs_act_mw_set_t),
			&p,	ADCS_SERVER_PACKET_SIZE(adcs_act_mw_set_t));
	if (retval == ADCS_SERVER_PACKET_SIZE(adcs_act_mw_set_t)) {
	}
	else {
		retval = 0;
	}
	return retval;
}

int adcs_act_mw_enable(adcs_act_mw_enable_t value){
	int retval;
	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_ACT_MW_EN;
	p.mw.mw_enable.mw = value.mw;
	p.mw.mw_enable.enable = value.enable;
	retval = csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs,
			&p, ADCS_SERVER_PACKET_SIZE(adcs_act_mw_enable_t),
			&p,	ADCS_SERVER_PACKET_SIZE(adcs_act_mw_enable_t));
	if (retval == ADCS_SERVER_PACKET_SIZE(adcs_act_mw_enable_t)) {
	}
	else {
		retval = 0;
	}
	return retval;
}

int adcs_act_mw_on(){
	int retval;
	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_ACT_MW_ON;
	retval = csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs, &p, ADCS_SERVER_PACKET_SIZE(0), &p, ADCS_SERVER_PACKET_SIZE(0));
	if (retval == ADCS_SERVER_PACKET_SIZE(0)) {
	}
	else {
		retval = 0;
	}
	return retval;
}

int adcs_act_mw_off(){
	int retval;
	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_ACT_MW_OFF;
	retval = csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs, &p, ADCS_SERVER_PACKET_SIZE(0), &p, ADCS_SERVER_PACKET_SIZE(0));
	if (retval == ADCS_SERVER_PACKET_SIZE(0)) {
	}
	else {
		retval = 0;
	}
	return retval;
}

int adcs_act_mw_diag(adcs_act_mw_diag_t value){
	int retval;
	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_ACT_MW_DIAG;
	p.mw.mw_diag.mw = value.mw;

	retval = csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, 10*timeout_adcs,
			&p, ADCS_SERVER_PACKET_SIZE(adcs_act_mw_diag_t),
			&p,	ADCS_SERVER_PACKET_SIZE(adcs_act_mw_diag_t));
	if (retval == ADCS_SERVER_PACKET_SIZE(adcs_act_mw_diag_t)) {
		printf("Current consumption: %i\r\n",util_ntoh16(p.mw.mw_diag.current));
		printf("Time 0-100 rad/s: %i ms (Expected 6000 ms)\r\n",util_ntoh16(p.mw.mw_diag.time));
	}
	else
		retval = 0;
	return retval;
}

int adcs_act_get_torquers(adcs_act_torquers_t *value)
{
	int retval;
	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_TRQ_GET;
	retval = csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs, &p, ADCS_SERVER_PACKET_SIZE(0), &p, ADCS_SERVER_PACKET_SIZE(adcs_act_torquers_t));
	if (retval == ADCS_SERVER_PACKET_SIZE(adcs_act_torquers_t)) {
		value->pwm[0] = util_ntohflt(p.trq.trq.pwm[0]);
		value->pwm[1] = util_ntohflt(p.trq.trq.pwm[1]);
		value->pwm[2] = util_ntohflt(p.trq.trq.pwm[2]);
	}
	return retval;
}

int adcs_act_set_torquers(adcs_act_torquers_t *value)
{
	int retval;
	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_TRQ_SET;
	p.trq.trq.pwm[0] = util_htonflt(value->pwm[0]);
	p.trq.trq.pwm[1] = util_htonflt(value->pwm[1]);
	p.trq.trq.pwm[2] = util_htonflt(value->pwm[2]);
	retval = csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs, &p, ADCS_SERVER_PACKET_SIZE(adcs_act_torquers_t), &p, ADCS_SERVER_PACKET_SIZE(0));
	return retval;
}
