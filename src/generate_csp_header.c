#include <stdio.h>
#include <signal.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/wait.h>
#include <ctype.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <fcntl.h>

#include <stdint.h>
#include <inttypes.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>

#include <conf_cspterm.h>
#include <csp-term.h>

/* Parameter system */
#include <param/param.h>
#include <param/param_example_table.h>

/* Log system */
#include <log/log.h>

/* CSP */
#include <csp/csp.h>
#include <csp/interfaces/csp_if_kiss.h>
#include <csp/interfaces/csp_if_can.h>
#include <csp/interfaces/csp_if_zmqhub.h>
#include <csp/drivers/usart.h>

/* Drivers / Util */
#include <util/console.h>
#include <util/log.h>
#include <util/vmem.h>
#include <util/clock.h>
#include <util/timestamp.h>
#include <util/strtime.h>

#include "itoa.h"

/*Create own command */
#include <command/command.h>

char * generate_csp_header(char * dest_addr, char * src_addr, char * src_port, char * dest_port, char * data_length, char * csp_header_output) {

	int dest_addr_len, src_addr_len, src_port_len, dest_port_len, data_length_len;
	int addr_original_bits = 5;
	int port_original_bits = 6;
	int length_original_bits = 8;
	int count;

	char prio_buf[20], dest_addr_buf[20], src_addr_buf[20], src_port_buf[20], dest_port_buf[20], data_length_buf[20];

	strcpy(prio_buf,"00"); //hardcoded priority number 2
	itoa(*dest_addr,dest_addr_buf,2);
	itoa(*src_addr,src_addr_buf,2);
	itoa(*src_port,src_port_buf,2);
	itoa(*dest_port,dest_port_buf,2);
	itoa(*data_length, data_length_buf, 2);

	dest_addr_len = strlen(dest_addr_buf);
	src_addr_len = strlen(src_addr_buf);
	src_port_len = strlen(src_port_buf);
	dest_port_len = strlen(dest_port_buf);
	data_length_len = strlen(data_length_buf);

	char temp_csp_header_output[100];

	/* Copy the Source Addr in binary */
	strcpy(temp_csp_header_output,prio_buf);

	/* Concatenate the Source Addr in binary */
	if (src_addr_len==5) {
		strcat(temp_csp_header_output,src_addr_buf);
	} else {
		count = addr_original_bits - src_addr_len;
		while (count!=0) {
			strcat(temp_csp_header_output,"0");
			count = count - 1;
		}
		strcat(temp_csp_header_output,src_addr_buf);
	}

	/* Concatenate the Destination Addr in binary */
	if (dest_addr_len==5) {
		strcat(temp_csp_header_output,dest_addr_buf);
	} else {
		count = addr_original_bits - dest_addr_len;
		while (count!=0) {
			strcat(temp_csp_header_output,"0");
			count = count - 1;
		}
		strcat(temp_csp_header_output,dest_addr_buf);
	}

	/* Concatenate the Destination Port in binary */
	if (dest_port_len==6) {
		strcat(temp_csp_header_output,dest_port_buf);
	} else {
		count = port_original_bits - dest_port_len;
		while (count!=0) {
			strcat(temp_csp_header_output,"0");
			count = count - 1;
		}
		strcat(temp_csp_header_output,dest_port_buf);
	}

	/* Concatenate the Source Port in binary */
	if (src_port_len==6) {
		strcat(temp_csp_header_output,src_port_buf);
	} else {
		count = port_original_bits - src_port_len;
		while (count!=0) {
			strcat(temp_csp_header_output,"0");
			count = count - 1;
		}
		strcat(temp_csp_header_output,src_port_buf);
	}

	/* Pad the Reaserve, Hmac, Xtea, Rdp, CRC with 0 (Unused) */
	strcat(temp_csp_header_output,"00000000");
//-------------------------------------------------------------------
	char* binaryString = temp_csp_header_output;
	char value = (int)strtol(binaryString, NULL, 2);
	char hexString[12];
	sprintf(hexString, "%X", value);
	strcpy(csp_header_output,hexString);

//-------------------------------------------------------------------
	char data_length_h[10];
	strcpy(data_length_h,"");

	if (data_length_len==8) {
		strcat(data_length_h,data_length_buf);
	} else {
		count = length_original_bits - data_length_len;
		while (count!=0) {
			strcat(data_length_h,"0");
			count = count - 1;
		}
		strcat(data_length_h,data_length_buf);
	}
//------------------------------------------------------------------
	//strcat(temp_csp_header_output,"00000000"); 
	//little endian for last 8 bits of data length (put zero padding behind)
//------------------------------------------------------------------
	char* binaryString2 = data_length_h;
	int value2 = (int)strtol(binaryString2, NULL, 2);
	char hexString2[20];
	sprintf(hexString2, "%02X", value2);
	strcat(csp_header_output,hexString2);

	return 0;
/*
	//Slice into 4 bits and convert from binary string to binary int
	char temp_csp_header_byte_output[100];
	int temp_four_bits;

	int a=0, b=0, c=0;
	int len;

	len = strlen(temp_csp_header_output);

	while (a<=len) {
		if (b>3) {
			temp_csp_header_byte_output[b]='\0'; //terminate the string
			//printf("the value of csp_header_byte is %s\r\n",temp_csp_header_byte_output);
			temp_four_bits = atoi(temp_csp_header_byte_output);
			//printf("the value of converted is %d\r\n",my_temp_number);
			csp_header_byte_output[c] = temp_four_bits;

			b = 0;
			c = c+1;

			strcpy(temp_csp_header_byte_output,""); //clear buffer
		} else {
			temp_csp_header_byte_output[b] = temp_csp_header_output[a];
			a = a+1;
			b = b+1;
		}
	}
*/
}

