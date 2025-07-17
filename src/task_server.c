/**
 * Mission Control Server
 *
 * @author Johan De Claville Christiansen
 * Copyright 2012 GomSpace ApS. All rights reserved.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>
#include <time.h>
#include <fcntl.h> 

/* Drivers / Util */
#include <util/console.h>
#include <util/vmem.h>
#include <util/clock.h>
#include <util/timestamp.h>
#include <util/strtime.h>

#include <param/param.h>
#include <gscript/gscript.h>
#include <ftp/ftp_server.h>
#include <csp/csp.h>
#include <util/log.h>
#include <csp-term.h>

/*For creation of new file directory*/
#include <sys/types.h>
#include <sys/stat.h>

#include "receive_packet.h"

#define MY_PORT 15	//PORT added to listen for test traffic
#define RE_PORT 16	//PORT added to send data when triggered
#define PING_RX_PORT 17	//PORT added to do LEOP ping response function
#define RE_DEST_PORT 15	//PORT where reply data is send to
#define DEST_ADDR 23	//ADDR where reply data is send to

char * get_time(char * my_str); 	//function declaration from get_timestamp.c
int * start_client();
void reply_data(csp_conn_t *conn, csp_packet_t *packet);
void Receive_data(csp_conn_t *conn, csp_packet_t *packet);

//--------------------------------------------------------------------------------------------------------
void * task_server(void * parameters) {
	
	/* Create socket */
	csp_socket_t * sock = csp_socket(0);

	/* Bind every port to socket*/
	csp_bind(sock, CSP_ANY);

	/* Create 10 connections backlog queue */
	csp_listen(sock, 10);

	/* Pointer to current connection and packet */
	csp_conn_t * conn;
	csp_packet_t * packet;

	/* Setup FTP server RAM */
	ftp_register_backend(BACKEND_RAM, &backend_ram);
	ftp_register_backend(BACKEND_FILE, &backend_file);

	/* Process incoming connections */
	while (1) {

		/* Wait for connection, 1000 ms timeout */
		if ((conn = csp_accept(sock, 1000)) == NULL)
		{
			//printf("No connection from satellite...\r\n");
			continue;
		}

		/* Print the csp header paramters in the csp packet send to GS100 */
		//printf("Case 1 : csp_conn_dport(conn) is %d\r\n", csp_conn_dport(conn));

		/* Spawn new task for FTP */
		if (csp_conn_dport(conn) == CSPTERM_PORT_FTP) {
			pthread_t handle_ftp;
			pthread_attr_t attr;

			/* Initialise attr */
			pthread_attr_init(&attr);
			/* Create thread as detached to prevent memory leak */ 
			pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
			extern void * task_ftp(void * conn_param);
			pthread_create(&handle_ftp, &attr, task_ftp, conn);
			continue;
		}
		/* Read packets. Timout is 1000 ms */
		while ((packet = csp_read(conn, 1000)) != NULL) {

			/* Print the csp header paramters in the csp packet send to GS100 */
			//printf("Case 2 : csp_conn_dport(conn) is %d\r\n", csp_conn_dport(conn));

			switch(csp_conn_dport(conn)) {

				case MY_PORT:	//PORT 15
					//printf("Downlink packet from Space\r\n");
					receive_packet(packet);	// TM port
					csp_close(conn);
				break;

				case RE_PORT:	//PORT 16
					reply_data(conn, packet); // Reply port
					csp_close(conn);
				break;

				case PING_RX_PORT:	//PORT 17
					Receive_data(conn, packet); // Ping receive port
					csp_close(conn);
				break;

				case CSPTERM_PORT_RPARAM:
					rparam_service_handler(conn, packet);
					csp_close(conn);
				break;

				case CSPTERM_PORT_GSCRIPT:
					gscript_service_handler(conn, packet);
					csp_close(conn);
				break;

				default: {
					csp_service_handler(conn, packet);
					csp_close(conn);
					break;
				}

			}

		}

		/* Close current connection, and handle next */
		csp_close(conn);

	}
	return NULL;

}
void Receive_data(csp_conn_t *conn, csp_packet_t *packet)
{
	printf("Ping 25 to satellite: Ping received!\n");
}
//-----------------------------------------------------------------------------------------------------------------------------------
void reply_data(csp_conn_t *conn, csp_packet_t *packet)
{
	
	conn = csp_connect(CSP_PRIO_NORM, DEST_ADDR, RE_DEST_PORT, 1000, CSP_O_NONE);	//allow user to set dest port else will reply to the same port
	if (conn == NULL) {
		/* Connect failed */
		printf("Connection setup failed\n");
		/* Remember to free packet buffer */
		csp_buffer_free(packet);
		return;
	}else{
	printf("connection setup is sucessful\r\n");
	}
	
	/* Defining the data values */
	packet->data[0] = 0xaa;
	packet->data[1] = 0xbb;
	packet->data[2] = 0xcc;

	/* Define the data lenngth */
	packet->length = 3;

	for(unsigned int i = 0; i < packet->length; i++)
	{
		printf("reply packet data %u is %x.\r\n", i, packet->data[i]);
	}
	
	/* Sending data to satellite */
	if(!csp_send(conn, packet, 1000))
	{
		/* Send failed */
		printf("Send failed\n");
		csp_buffer_free(packet);
	}
	printf("Reply sent.\r\n");

}
