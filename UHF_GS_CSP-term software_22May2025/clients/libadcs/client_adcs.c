/*
 * client_adcs.c
 *
 *  Created on: Mar 24, 2011
 *      Author: jal
 */

#include <stdio.h>
#include <csp/csp.h>
#include <csp/csp_endian.h>

#include <util/log.h>
#include <util/hexdump.h>
#include <util/byteorder.h>

#include "adcs/adcs_types.h"
#include "adcs/adcs_param.h"
#include "adcs/adcs_client_periph.h"

#define ADCS_CSP_TIMEOUT 2000
#define ADCS_GPS_CSP_TIMEOUT 10000

static int node_adcs = 4;
static int port_adcs = 20;
static int timeout_adcs = ADCS_CSP_TIMEOUT;

void adcs_node_set(int node, int port)
{
	node_adcs = node;
	port_adcs = port;
	adcs_periph_node_set(node, port);
	adcs_gps_node_set(node, port);
}

void adcs_timeout_set(int timeout)
{
	timeout_adcs = timeout;
}

int adcs_start(void)
{
	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_STARTSTOP;
	p.state.startstop.mode = 0;
	return csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs, &p, ADCS_SERVER_PACKET_SIZE(adcs_startstop_t), NULL, 0);
}

int adcs_stop(void)
{
	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_STARTSTOP;
	p.state.startstop.mode = 1;
	return csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs, &p, ADCS_SERVER_PACKET_SIZE(adcs_startstop_t), NULL, 0);
}

int adcs_fullstop(void)
{
	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_STARTSTOP;
	p.state.startstop.mode = 4;
	return csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs, &p, ADCS_SERVER_PACKET_SIZE(adcs_startstop_t), NULL, 0);
}

int adcs_restart(void)
{
	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_STARTSTOP;
	p.state.startstop.mode = 2;
	return csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs, &p, ADCS_SERVER_PACKET_SIZE(adcs_startstop_t), NULL, 0);
}


int adcs_pause(void)
{
	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_STARTSTOP;
	p.state.startstop.mode = 3;
	return csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs, &p, ADCS_SERVER_PACKET_SIZE(adcs_startstop_t), NULL, 0);
}

int adcs_get_adcs_state(adcs_mode_t *mode)
{
	int retval;
	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_GET_ADCS_STATE;
	retval = csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs, &p, ADCS_SERVER_PACKET_SIZE(0), &p, ADCS_SERVER_PACKET_SIZE(adcs_mode_t));
	if (retval == ADCS_SERVER_PACKET_SIZE(adcs_mode_t)) {
		*mode = p.state.get_adcs_modes;
	}
	else {
		retval = 0;
	}
	return retval;
}

int adcs_set_ads_state(ads_mode_t state)
{
	int retval;
	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_SET_ADS_STATE;
	p.state.set_ads.state = state;
	retval = csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs, &p, ADCS_SERVER_PACKET_SIZE(adcs_state_t), &p, ADCS_SERVER_PACKET_SIZE(adcs_state_t));
	if (retval == ADCS_SERVER_PACKET_SIZE(adcs_state_t)) {
		log_info("Desired ADS state set!");
	}
	else {
		retval = 0;
	}
	return retval;
}

int adcs_set_ephem_state(ads_mode_t state)
{
	int retval;
	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_SET_EPHEM_STATE;
	p.state.set_ephem.state = state;
	retval = csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs, &p, ADCS_SERVER_PACKET_SIZE(adcs_state_t), &p, ADCS_SERVER_PACKET_SIZE(adcs_state_t));
	if (retval == ADCS_SERVER_PACKET_SIZE(adcs_state_t)) {
		log_info("Desired ephem state set!");
	}
	else {
		retval = 0;
	}
	return retval;
}


int adcs_set_acs_state(acs_mode_t mode)
{
	int retval;
	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_SET_ACS_STATE;
	p.state.set_acs.state = mode;
	retval = csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs, &p, ADCS_SERVER_PACKET_SIZE(adcs_state_t), &p, ADCS_SERVER_PACKET_SIZE(adcs_state_t));
	if (retval == ADCS_SERVER_PACKET_SIZE(adcs_state_t)) {
		log_info("Desired ACS state set!");
	}
	else {
		retval = 0;
	}
	return retval;
}


int adcs_ephem_new_tle()
{
	int retval;
	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_EPHEM_NEW_TLE;
	retval = csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs, &p, ADCS_SERVER_PACKET_SIZE(0), &p, ADCS_SERVER_PACKET_SIZE(0));
	return retval;
}


int adcs_hk_basic(adcs_hk_basic_t *hkbasic)
{
	int retval;
	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_HK_BASIC;
	retval = csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs, &p, ADCS_SERVER_PACKET_SIZE(0), &p, ADCS_SERVER_PACKET_SIZE(adcs_hk_basic_t));
	if (retval == ADCS_SERVER_PACKET_SIZE(adcs_hk_basic_t)) {
		hkbasic->jdate = csp_ntohdbl(p.hk.basic.jdate);
		hkbasic->q[0] = csp_ntohflt(p.hk.basic.q[0]);
		hkbasic->q[1] = csp_ntohflt(p.hk.basic.q[1]);
		hkbasic->q[2] = csp_ntohflt(p.hk.basic.q[2]);
		hkbasic->q[3] = csp_ntohflt(p.hk.basic.q[3]);
		hkbasic->w[0] = csp_ntohflt(p.hk.basic.w[0]);
		hkbasic->w[1] = csp_ntohflt(p.hk.basic.w[1]);
		hkbasic->w[2] = csp_ntohflt(p.hk.basic.w[2]);
		hkbasic->pos[0] = csp_ntohflt(p.hk.basic.pos[0]);
		hkbasic->pos[1] = csp_ntohflt(p.hk.basic.pos[1]);
		hkbasic->pos[2] = csp_ntohflt(p.hk.basic.pos[2]);
		hkbasic->eclipse = p.hk.basic.eclipse;
	}
	else
		retval = 0;
	return retval;
}

/* BDOT commands */
int adcs_bdot_force_detumbled(void)
{
	int retval;
	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_BDOT_FORCE_DETUMBLED;
	retval = csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs, &p, ADCS_SERVER_PACKET_SIZE(0), &p, ADCS_SERVER_PACKET_SIZE(0));
	return retval;
}

/* Pointing commands */
int adcs_set_reset(void)
{
	int retval = 0;
	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_SET_RESET;
	retval = csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs, &p, ADCS_SERVER_PACKET_SIZE(0), &p, ADCS_SERVER_PACKET_SIZE(0));
	return retval;
}

int adcs_set_nadir(void)
{
	int retval = 0;
	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_SET_NADIR;
	retval = csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs, &p, ADCS_SERVER_PACKET_SIZE(0), &p, ADCS_SERVER_PACKET_SIZE(0));
	return retval;
}

int adcs_set_ram(void)
{
	int retval = 0;
	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_SET_RAM;
	retval = csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs, &p, ADCS_SERVER_PACKET_SIZE(0), &p, ADCS_SERVER_PACKET_SIZE(0));
	return retval;
}

int adcs_set_mindrag(void)
{
	int retval = 0;
	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_SET_MINDRAG;
	retval = csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs, &p, ADCS_SERVER_PACKET_SIZE(0), &p, ADCS_SERVER_PACKET_SIZE(0));
	return retval;
}

int adcs_set_maxdrag(void)
{
	int retval = 0;
	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_SET_MAXDRAG;
	retval = csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs, &p, ADCS_SERVER_PACKET_SIZE(0), &p, ADCS_SERVER_PACKET_SIZE(0));
	return retval;
}

int adcs_set_custom0(void)
{
	int retval = 0;
	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_SET_CUSTOM0;
	retval = csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs, &p, ADCS_SERVER_PACKET_SIZE(0), &p, ADCS_SERVER_PACKET_SIZE(0));
	return retval;
}

int adcs_set_custom1(void)
{
	int retval = 0;
	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_SET_CUSTOM1;
	retval = csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs, &p, ADCS_SERVER_PACKET_SIZE(0), &p, ADCS_SERVER_PACKET_SIZE(0));
	return retval;
}

int adcs_set_custom2(void)
{
	int retval = 0;
	adcs_server_packet_t p;
	p.cmd = ADCS_CMD_SET_CUSTOM2;
	retval = csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs, &p, ADCS_SERVER_PACKET_SIZE(0), &p, ADCS_SERVER_PACKET_SIZE(0));
	return retval;
}

/* Tracking commands */
int adcs_track_reset(void)
{
    int retval = 0;
    adcs_server_packet_t p;
    p.cmd = ADCS_CMD_TRACK_RESET;
    retval = csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs, &p, ADCS_SERVER_PACKET_SIZE(0), &p, ADCS_SERVER_PACKET_SIZE(0));
    return retval;
}

int adcs_track_sun(void)
{
    int retval = 0;
    adcs_server_packet_t p;
    p.cmd = ADCS_CMD_TRACK_SUN;
    retval = csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs, &p, ADCS_SERVER_PACKET_SIZE(0), &p, ADCS_SERVER_PACKET_SIZE(0));
    return retval;
}

int adcs_track_moon(void)
{
    int retval = 0;
    adcs_server_packet_t p;
    p.cmd = ADCS_CMD_TRACK_MOON;
    retval = csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs, &p, ADCS_SERVER_PACKET_SIZE(0), &p, ADCS_SERVER_PACKET_SIZE(0));
    return retval;
}

int adcs_track_target0(void)
{
    int retval = 0;
    adcs_server_packet_t p;
    p.cmd = ADCS_CMD_TRACK_TARGET0;
    retval = csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs, &p, ADCS_SERVER_PACKET_SIZE(0), &p, ADCS_SERVER_PACKET_SIZE(0));
    return retval;
}

int adcs_track_target1(void)
{
    int retval = 0;
    adcs_server_packet_t p;
    p.cmd = ADCS_CMD_TRACK_TARGET1;
    retval = csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs, &p, ADCS_SERVER_PACKET_SIZE(0), &p, ADCS_SERVER_PACKET_SIZE(0));
    return retval;
}

int adcs_track_target2(void)
{
    int retval = 0;
    adcs_server_packet_t p;
    p.cmd = ADCS_CMD_TRACK_TARGET2;
    retval = csp_transaction(CSP_PRIO_NORM, node_adcs, port_adcs, timeout_adcs, &p, ADCS_SERVER_PACKET_SIZE(0), &p, ADCS_SERVER_PACKET_SIZE(0));
    return retval;
}
