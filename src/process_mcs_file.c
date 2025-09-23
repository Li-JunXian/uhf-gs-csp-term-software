
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>
#include <time.h>
#include <fcntl.h>

#include <util/log.h>

int decode_parameters(unsigned char * hex_input, char * pri, char * src_out, char * dst_out, char * dst_p_out, char * src_p_out, char * hmac_out, char * xtea_out, char * rdp_out, char * crc_out, char * data_length_out);

int data_size;

int process_mcs_file(char * mcs_filename, char* pri, char * src, char * dst, char * dst_port, char * src_port, char * hmac, char * xtea, char * rdp, char * crc, char * data_length, char * csp_packet)
{	
	FILE *fs1 = fopen(mcs_filename, "rb");
	
    	if(fs1 == NULL)
    	{
        	perror("File not found on server!");
		exit(EXIT_FAILURE);
		return 0;
    	}
	/* Check file size */
	int size = 0;		// Set the data length to 0
	fseek(fs1, 0, SEEK_END);
	size = (int)(ftell(fs1)); // get current file pointer as data size
	fseek(fs1, 0, SEEK_SET);
	if (size <= 6) {
		log_error("MCS packet error: packet size <= 6 bytes");
		fclose(fs1);
		return 0;
	}

	/* Csp header field*/
	unsigned char header_buffer[6];	// Csp-packet header is 6 bytes
	int rcv_bytes = fread(header_buffer, sizeof(char), 6, fs1);
	if(rcv_bytes < 1){
		log_error("Csp header reading error");
		fclose(fs1);
		return 0;
	}
	
	/* Csp packet field*/
	data_size = size-6;	
	int rcv_bytes_ = fread(csp_packet, sizeof(char), data_size, fs1);
 	if(rcv_bytes_ < 1){
		log_error("Csp packet reading error");
		fclose(fs1);
		return 0;
	}
	decode_parameters(header_buffer, pri, src, dst, dst_port, src_port, hmac, xtea, rdp, crc, data_length);

	fclose(fs1);
	return 1;
}

int decode_parameters(unsigned char * hex_input, char * pri, char * src_out, char * dst_out, char * dst_p_out, char * src_p_out, char * hmac_out, char * xtea_out, char * rdp_out, char * crc_out, char * data_length_out)
{
	sprintf(pri,"%d",((hex_input[0])>>6) & 0x03);
	sprintf(src_out,"%d",((hex_input[0])>>1) & 0x1F);
	sprintf(dst_out,"%d",(((hex_input[0])<<8 | hex_input[1]) >> 4)& 0x1F);
	sprintf(dst_p_out,"%d",(((hex_input[1])<<8 | hex_input[2]) >> 6) & 0x3F);
	sprintf(src_p_out,"%d",((hex_input[1])<<8 | hex_input[2]) & 0x3F);
	sprintf(hmac_out,"%d",((hex_input[3]>>3) & 0x01));
	sprintf(xtea_out,"%d",((hex_input[3]>>2) & 0x01)); 
	sprintf(rdp_out,"%d",((hex_input[3]>>1) & 0x01)); 
	sprintf(crc_out,"%d",(hex_input[3] & 0x01));
	sprintf(data_length_out,"%d",(hex_input[5] & 0xFF));
	return 0;	
}

