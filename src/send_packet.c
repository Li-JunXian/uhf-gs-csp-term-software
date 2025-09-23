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

#include "doppler_freq_correction.h"
#include "receive_packet.h"
//#include "process_mcs_file.h"

/* Option to perform automatic LNA feature*/
#define AUTO_LNA	0	/* change to 1 to enable LNA feature*/

char * get_time(char * my_str);

int send_packet(char * mcs_uplink_filename){
	
	char time_string_sent[100];
	char time_file_sent[200];	

	/* setup the packet structure for request */
	csp_packet_t *packet = csp_buffer_get(SIZE_MAX);
    	if (packet == NULL) 
	{
        	printf("Failed to get buffer element\\n");
        	return -1;
    	}

	char pri[3], src_addr[6], dst_addr[6], dst_port[6], src_port[6], hmac[6], xtea[6], rdp[6], crc[6], mcs_data_length[6];

	int pri_int, src_addr_int, dst_addr_int, dst_port_int, src_port_int, hmac_int, xtea_int, rdp_int, crc_int, mcs_data_length_int;	
	

	int process = process_mcs_file(mcs_uplink_filename, pri, src_addr, dst_addr, dst_port, src_port, hmac, xtea, rdp, crc, mcs_data_length, packet->data);
	if (process == 1) {
		printf("Processing MCS Uplink Packet.......\r\n");
	} else {
		printf("Processing MCS Packet Error.......\r\n");
		return -1;
	}
	pri_int = atoi(pri);
	src_addr_int = atoi(src_addr);
	dst_addr_int = atoi(dst_addr);
	dst_port_int = atoi(dst_port);
	src_port_int = atoi(src_port);
	hmac_int = atoi(hmac);
	xtea_int = atoi(xtea);
	rdp_int = atoi(rdp);
	crc_int = atoi(crc);
	mcs_data_length_int = atoi(mcs_data_length);

	printf("[prio: %d], [src addr: %d], [dest addr: %d], [dest port: %d], [src port: %d]\r\n",pri_int, src_addr_int, dst_addr_int, dst_port_int, src_port_int);
	printf("[hmac: %d], [xtea: %d], [rdp: %d], [crc: %d], [data length: %d bytes]\r\n",hmac_int, xtea_int, rdp_int, crc_int, mcs_data_length_int);

	packet->length = mcs_data_length_int;

	/* TC uplink packet if destination addr <= 15 */
	if(dst_addr_int <= 15) {
		if(AUTO_LNA == 1){
			int st_off = lna_conf(2);
			if(st_off == 1)
				log_debug("Unable to config LNA usbrelay, please check");
			sleep(0.5); // delay 0.5 seconds waiting for LNA to turn off
		}

		if(csp_sendto(pri_int, dst_addr_int, dst_port_int, src_port_int, 0, packet, 1000) == -1) {
			printf("Failed to send CSP_Packet\r\n");
		} else {
			printf("...CSP_Packet Sent out from GS100 at %s\r\n",get_time(time_string_sent));
			if(AUTO_LNA == 1){
				//sleep(0.5); // delay 0.5 seconds waiting for LNA to turn on
				int st_on = lna_conf(1);
				if(st_on == 1)
					log_debug("Unable to config LNA usbrelay, please check");
				sleep(0.5); // delay 0.5 seconds waiting for LNA to turn on
			}
		}
	} else {
		if (dst_addr_int > 15) {	
			printf("Receiving CMD packet from MCS Client......\n");
			
			if (packet->data[0] == 0x01) {
				log_info("Receiving MCS Sat Selection Command from MCS Client......");
				uint32_t no_ = packet->data[1];
				if( mcs_sat_sel(no_) != 1) {
					log_error("[TCP Server] Failed to select Satellite No.");
				}
			} 
			else if (packet->data[0] == 0x00) {
				log_info("Receiving MCS Sat Read Command from MCS Client......");
				uint32_t no_read = mcs_sat_read();
				log_info("MCS_SAT_READ: Tracking Satellite %d\n", no_read);
				/* To check feedback with what packet format? Is MCS maintaining the connection? */
			}
			else if (packet->data[0] == 0x06) {
				log_info("Receiving MCS TLE Update Command from MCS Client......");
				updatetle();
			}
			/* ONLY for fixed frequency setting, Doppler shift freq compensation code will overwrite this */
			else if (packet->data[0] == 0x02) {
				log_info("Receiving MCS Set RF Freq Command from MCS Client......");
				uint32_t rx_freq_mcs = (packet->data[1] << 24) + (packet->data[2] << 16) + (packet->data[3] << 8) + packet->data[4];
				if (!ax100_set_rx_freq(29, 1000, rx_freq_mcs))
					log_error("Fail to set MCS RX Freq......");
			}
			else if (packet->data[0] == 0x03) {
				log_info("Receiving MCS Set TX Freq Command from MCS Client......");
				uint32_t tx_freq_mcs = (packet->data[1] << 24) + (packet->data[2] << 16) + (packet->data[3] << 8) + packet->data[4];
				if (!ax100_set_tx_freq(29, 1000, tx_freq_mcs))
					log_error("Fail to set MCS TX Freq......");
			}

		} else {	
			/* Test trigger packet downlink*/
			receive_packet(packet);
		}
	}
	
	/* Backup the packet in directory with timestamp */
	strcpy(time_file_sent,"/home/rai/Desktop/GS_Server_Folder/Sent_To_MCS/");	
	strcat(time_file_sent,time_string_sent);
	strcat(time_file_sent,".bin");
	printf("Saving data into file...");

	FILE *file_sent_pointer = fopen(time_file_sent,"ab");

	if(file_sent_pointer == NULL)
	{
		printf("target file cannot be opened.\r\n");
	}
	if(fwrite(packet->data, 1, mcs_data_length_int, file_sent_pointer) == 0)
	{
		printf("Saving unsucessful.\r\n");
	} else {
		printf("Saving complete.\r\n\n");
	}
	fclose(file_sent_pointer);
	//csp_buffer_free(packet);
	return 0;
}
