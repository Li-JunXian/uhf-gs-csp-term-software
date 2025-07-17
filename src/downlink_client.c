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

#include "tcp_server.h"

#define LENGTH 32768

//#define MCS_ADDR "172.20.20.69"  //FF Flatsat PC
//#define MCS_ADDR "10.245.66.87" //P2 FlatsatPC
//#define MCS_ADDR "172.24.172.92" //S2S MCS Desktop WiFi (dynamic)
#define MCS_ADDR "10.245.64.218" //S2S MCS Desktop ethernet (dynamic)
#define MCS_PORT 1025
//#define MCS_PORT 9876

int ret = 1;
int sock = 0; //create checking integer variables

int * downlink_client(char * fs_name, int file_type) {

	printf("[TCP Client] MCS Client IP address: %s \n", client_addr);
	//printf("[TCP Client] MCS Client IP address: %s \n", MCS_ADDR);

	/* Start a new TCP connection if there is no exsiting connection*/
	if(ret == 1) 
	{
    		char dest_server_address[1024] = {0}; 
    		struct sockaddr_in addr_remote;

		printf("\n---SETUP A NEW TCP CONNECTION---\n");
	    	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
			printf("\n Socket creation error \n"); 
			return 0; 
	    	}

		/* Use the client to TCP server IP address as downlink receipent*/
	    	strcpy(dest_server_address,client_addr);
		//strcpy(dest_server_address,MCS_ADDR);

	    	addr_remote.sin_family = AF_INET;
	    	addr_remote.sin_port = htons(MCS_PORT);
	       
	    	/*pton(ipv4, string address input, binary address output)*/
	    	if(inet_pton(AF_INET, dest_server_address, &addr_remote.sin_addr)<=0) { 
			printf("\nInvalid address/ Address not supported \n"); 
			return 0; 
	    	}
	    	bzero(&(addr_remote.sin_zero), 8); 

	    	/* Connect to MCS tcp server */
	    	if (connect(sock, (struct sockaddr *)&addr_remote, sizeof(struct sockaddr)) < 0) { 
			printf("\nConnection Failed \n"); 
			return 0; 
	    	} else {
	    		printf("Connection Successful to MCS Server\n");
		}
		ret = 0;

	}

    	//Send File to Server
	char sdbuf[LENGTH]; 
	printf("[TCP Client] Sending to MCS Server.....");
	FILE *fs = fopen(fs_name, "r");
	if(fs == NULL)
	{
		printf("\nERROR: File %s not found.\n", fs_name);
		return 0; 
	}

	bzero(sdbuf, LENGTH); 
	int fs_block_sz; 
	while((fs_block_sz = fread(sdbuf, sizeof(char), LENGTH, fs)) > 0)
	{
	    if(send(sock, sdbuf, fs_block_sz, 0) < 0)
	    {
	        fprintf(stderr, "\nERROR: Failed to send file %s. (errno = %d)\n", fs_name, errno);
		ret = 1;
		close(sock);
	        break;
	    }
	    else {
		printf("Sent Successfully!\n\n");
	    }
	    bzero(sdbuf, LENGTH);
	}
	
	return 0; 	   
}
