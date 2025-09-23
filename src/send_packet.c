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
#include "gui_backend.h"
 #include "receive_packet.h"

#include "process_mcs_file.h"
 
 /* Option to perform automatic LNA feature*/
 #define AUTO_LNA	0	/* change to 1 to enable LNA feature*/

typedef struct {
	uint8_t priority;
	uint8_t src;
	uint8_t dst;
	uint8_t dst_port;
	uint8_t src_port;
	uint8_t hmac;
	uint8_t xtea;
	uint8_t rdp;
	uint8_t crc;
} mcs_packet_header_t;

static int send_packet_execute(const char *origin, const mcs_packet_header_t *header,
				   csp_packet_t *packet, const uint8_t *payload, size_t payload_len,
				   int *packet_consumed)
{
	int success = 1;
	int consumed = 0;
	char time_string_sent[100] = "";
	char time_file_sent[200] = "";
 

	if ((header == NULL) || (packet == NULL) || (payload == NULL))
		return -1;
 

	packet->length = payload_len;

        if (header->dst <= 15) {
                if (AUTO_LNA == 1) {
                        int st_off = lna_conf(2);
                        if (st_off == 1)
                                log_debug("Unable to config LNA usbrelay, please check");
                        usleep(500000);
                }
 

		if (csp_sendto(header->priority, header->dst, header->dst_port, header->src_port, 0,
			       packet, 1000) == -1) {
 			printf("Failed to send CSP_Packet\r\n");
			success = 0;
 		} else {

			printf("...CSP_Packet Sent out from GS100 at %s\r\n", get_time(time_string_sent));
                        consumed = 1;
                        if (AUTO_LNA == 1) {
                                int st_on = lna_conf(1);
                                if (st_on == 1)
                                        log_debug("Unable to config LNA usbrelay, please check");
                                usleep(500000);
                        }
 		}
 	} else {
	
		if (header->dst > 15) {
 			printf("Receiving CMD packet from MCS Client......\n");

			if (payload_len > 0) {
				switch (payload[0]) {
				case 0x01: {
					log_info("Receiving MCS Sat Selection Command from MCS Client......");
					if (payload_len > 1) {
						uint32_t no_ = payload[1];
						if (mcs_sat_sel(no_) != 1) {
							log_error("[TCP Server] Failed to select Satellite No.");
							success = 0;
						}
					}
					break;
				}
				case 0x00: {
					log_info("Receiving MCS Sat Read Command from MCS Client......");
					uint32_t no_read = mcs_sat_read();
					log_info("MCS_SAT_READ: Tracking Satellite %d\n", no_read);
					break;
				}
				case 0x06:
					log_info("Receiving MCS TLE Update Command from MCS Client......");
					updatetle();
					break;
				case 0x02:
					if (payload_len >= 5) {
						log_info("Receiving MCS Set RF Freq Command from MCS Client......");
						uint32_t rx_freq_mcs = (payload[1] << 24) + (payload[2] << 16) + (payload[3] << 8) + payload[4];
						if (!ax100_set_rx_freq(29, 1000, rx_freq_mcs))
							log_error("Fail to set MCS RX Freq......");
					} else {
						success = 0;
					}
					break;
				case 0x03:
					if (payload_len >= 5) {
						log_info("Receiving MCS Set TX Freq Command from MCS Client......");
						uint32_t tx_freq_mcs = (payload[1] << 24) + (payload[2] << 16) + (payload[3] << 8) + payload[4];
						if (!ax100_set_tx_freq(29, 1000, tx_freq_mcs))
							log_error("Fail to set MCS TX Freq......");
					} else {
						success = 0;
					}
					break;
				default:
					break;
 				}

 			}

		} else {
 			/* Test trigger packet downlink*/
 			receive_packet(packet);
 		}
 	}

 

	if (time_string_sent[0] == '\0')
		get_time(time_string_sent);

	snprintf(time_file_sent, sizeof(time_file_sent),
		"/home/rai/Desktop/GS_Server_Folder/Sent_To_MCS/%s.bin", time_string_sent);
	printf("Saving data into file...");
 

	FILE *file_sent_pointer = fopen(time_file_sent, "ab");
	if (file_sent_pointer == NULL) {
		printf("target file cannot be opened.\r\n");

 	} else {

		if (fwrite(payload, 1, payload_len, file_sent_pointer) == 0) {
			printf("Saving unsucessful.\r\n");
		} else {
			printf("Saving complete.\r\n\n");
		}
		fclose(file_sent_pointer);
 	}


	gui_backend_notify_uplink(origin != NULL ? origin : "UNKNOWN", time_file_sent,
			      payload_len, success);

	if (packet_consumed != NULL)
		*packet_consumed = consumed;

	return success ? 0 : -1;
}


char * get_time(char * my_str);

int send_packet(char * mcs_uplink_filename)
{
        csp_packet_t *packet = csp_buffer_get(SIZE_MAX);
        if (packet == NULL) {
                printf("Failed to get buffer element\\n");
                return -1;
        }

        char pri[3], src_addr[6], dst_addr[6], dst_port[6], src_port[6], hmac[6], xtea[6], rdp[6], crc[6], mcs_data_length[6];

        int process = process_mcs_file(mcs_uplink_filename, pri, src_addr, dst_addr, dst_port, src_port,
                                       hmac, xtea, rdp, crc, mcs_data_length, packet->data);
        if (process != 1) {
                printf("Processing MCS Packet Error.......\r\n");
                csp_buffer_free(packet);
                return -1;
        }

        int pri_int = atoi(pri);
        int src_addr_int = atoi(src_addr);
        int dst_addr_int = atoi(dst_addr);        int dst_port_int = atoi(dst_port);
        int src_port_int = atoi(src_port);
        int hmac_int = atoi(hmac);
        int xtea_int = atoi(xtea);
        int rdp_int = atoi(rdp);
        int crc_int = atoi(crc);
        int mcs_data_length_int = atoi(mcs_data_length);

        printf("Processing MCS Uplink Packet.......\r\n");
        printf("[prio: %d], [src addr: %d], [dest addr: %d], [dest port: %d], [src port: %d]\r\n",
               pri_int, src_addr_int, dst_addr_int, dst_port_int, src_port_int);
        printf("[hmac: %d], [xtea: %d], [rdp: %d], [crc: %d], [data length: %d bytes]\r\n",
               hmac_int, xtea_int, rdp_int, crc_int, mcs_data_length_int);

        packet->length = mcs_data_length_int;

        mcs_packet_header_t header = {
                .priority = (uint8_t) pri_int,
                .src = (uint8_t) src_addr_int,
                .dst = (uint8_t) dst_addr_int,
                .dst_port = (uint8_t) dst_port_int,
                .src_port = (uint8_t) src_port_int,
                .hmac = (uint8_t) hmac_int,
                .xtea = (uint8_t) xtea_int,
                .rdp = (uint8_t) rdp_int,
                .crc = (uint8_t) crc_int,
        };

        int consumed = 0;
        int status = send_packet_execute("MCS", &header, packet, packet->data,
                                         packet->length, &consumed);
        if (!consumed)
                csp_buffer_free(packet);

        return status;
}

int send_packet_struct(const char *origin, const mcs_packet_header_t *header,
                       const uint8_t *payload, size_t payload_len)
{
        if ((header == NULL) || (payload == NULL))
                return -1;

        csp_packet_t *packet = csp_buffer_get(SIZE_MAX);
        if (packet == NULL) {
                printf("Failed to get buffer element\n");
                return -1;
        }

        if (payload_len > sizeof(packet->data)) {
                csp_buffer_free(packet);
                printf("Payload too large for CSP buffer\n");
                return -1;
        }

        memcpy(packet->data, payload, payload_len);
        packet->length = payload_len;

        int consumed = 0;
        int status = send_packet_execute(origin, header, packet, payload, payload_len, &consumed);
        if (!consumed)
                csp_buffer_free(packet);

        return status;
}
