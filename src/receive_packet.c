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
#include "receive_packet.h"
#include "itoa.h"

/*For creation of new file directory*/
#include <sys/types.h>
#include <sys/stat.h>

#define DEBUG 0

char * get_time(char * my_str);	//function declaration from get_timestamp.c
int * downlink_client(char * fs_name, int file_type);
unsigned int count = 0;
char suffix[] = {'_', '0', '0', 0};

void receive_packet(csp_packet_t *packet)
{
	char time_string_recv[100];
	char time_file_recv[200];
	uint8_t header_values[10];

	printf("Receiving Packet Data from Satellite at %s\r\n",get_time(time_string_recv));
	
	/*
	if(csp_crc32_verify(packet, 1)==-1) {
		printf("crc failed to verify\r\n");
	} else {
		printf("crc verified successfully\r\n");
	}
	*/
	
	/* get the csp packet headers, require the AX100 using csp_connect() function */
	csp_conn_get_header_values(header_values);

	uint8_t prio_value = 0;	// Priority field not used
	uint8_t reserve = 0;	// Reserve field 
	uint8_t flags = 0;	// flags field not used

	printf("Re-generating and saving CSP packet header.....\r\n");
	printf("[prio: %u], [src addr: %u], [dest addr: %u], [dest port: %u], [src port: %u]\r\n",prio_value, header_values[1], header_values[0], header_values[3], header_values[2]);
	printf("[hmac: 0], [xtea: 0], [rdp: 0], [crc: 0], [data length: %d bytes]\r\n", packet->length);	
	
	/* Formating csp header for MCS */
	uint8_t csp_header[6];
	csp_header[0] = ((prio_value << 6) & 0xC0) | ((header_values[1] << 1) & 0x3E) | ((header_values[0] >> 4) & 0x01);
	csp_header[1] = ((header_values[0] << 4) & 0xF0) | ((header_values[3] >> 2) & 0x0F);
	csp_header[2] = ((header_values[3] << 6) & 0xC0) | (header_values[2] & 0x3F);
	csp_header[3] = ((reserve << 4) & 0xF0) | (flags & 0x0F);
	csp_header[4] = (packet->length >> 8);
	csp_header[5] = (packet->length);


	/* Debug printing data field */
	if (DEBUG) {
		puts("Received packet:");
		for(unsigned int i = 1; i < packet->length; i++) {
			printf("%02x", packet->data[i - 1]);
			if (i % 2 == 0) {
				printf(" ");
			}
			if (i % 8 == 0) {
				printf("\r\n");
			}
		}
		printf("\r\n");
	}	
	
	/* Backup downlink csp packet in local directory*/
	strcpy(time_file_recv,"/home/rai/Desktop/GS_Server_Folder/Sent_To_MCS/");	
	strcat(time_file_recv,time_string_recv);
	sprintf(suffix + 1, "%02u", count++ % 100);
	// suffix[1] = '0' + ();
	strcat(time_file_recv, suffix);
	strcat(time_file_recv,".bin");
	printf("Saving data into file...\r\n");

	FILE *file_recv_pointer = fopen(time_file_recv, "wb");	
	if(file_recv_pointer == NULL) {
		printf("target file cannot be opened.\r\n");
		return;
	}
	if(fwrite(csp_header, 1 ,6,file_recv_pointer) == 0) {
		printf("Saving csp packet header unsucessful\r\n");
		return;
	}

	if(fwrite(packet->data, 1 ,packet->length,file_recv_pointer) == 0) {
		printf("Saving csp packet unsucessful\r\n");
	} else {
		printf("Saving csp packet complete\r\n\n");
	}
	fclose(file_recv_pointer);


	/* Differentiate packet type using destination node address */
	if( header_values[0] < 20) {
		printf("Beacon data received! Processing now.....\r\n");
		downlink_client(time_file_recv,1); //send the beacon data to MCS server
	} else {
		printf("Downlink data received! Sending to MCS server now .....\r\n");
		downlink_client(time_file_recv,0); //send the downlink data to MCS server
	}
	return;	
}
