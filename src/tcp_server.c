/**
 * TCP Server for MCS command and data packet uplink
 *
 */
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
#include <sys/stat.h>
#include <poll.h>
#include <limits.h>

#include <command/command.h>
#include <util/log.h>

#include "send_packet.h"
#include "doppler_freq_correction.h"
 
#define LENGTH 512
#define MCS_PORT 1028
#define MCS_DATA_DIR "/home/rai/Desktop/GS_Server_Folder/Received_From_MCS/"
#define LOG 10
 
// IP address of E4 UHF
char MCS_CLIENT[256] = "10.245.64.218"; // IP address of MCS STAR

//Filename for the mcs uplink packet received from MCS
char fr_name[200]; 

//------------------TCP Server----------------//
int server_fd, newsocket, rcv_bytes, opt = 1, PORT;

fd_set master;    // master file descriptor list
fd_set read_fds;  // temp file descriptor list for select()
int fdmax;        // maximum file descriptor number
int newfd;        // newly accept()ed socket descriptor

struct sockaddr_in addr_local;
struct sockaddr_in addr_remote;
socklen_t addrlen = sizeof(struct sockaddr_in);

char msg_buffer[1024] = {0};
char buf[256];    // buffer for client data 

char client_addr[20] = {0};	// Global address contain client ip address

int nbytes;		//
int i,j,rv;		//

unsigned char filename[100], rcv_buffer[LENGTH], rcv_buffer_bytes[LENGTH];

void receive_file_from_mcs(char * fr_name);
	
void tcp_server(void)
{		
	FD_ZERO(&master);    	// clear the master list
	FD_ZERO(&read_fds);	// clear the reader fd
	/* Creating socket */
	if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0 )
	{
		printf("Socket failed");                           
		return;
	}
	
	/* Reuse address and port */
    	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    	{
        	perror("Set sockopt failed");
        	return;
    	}
	
	PORT = MCS_PORT;
	addr_local.sin_family = AF_INET;
	addr_local.sin_addr.s_addr = INADDR_ANY;
	addr_local.sin_port = htons(PORT);
	bzero(&(addr_local.sin_zero), 8);

	/* Bind socket to port */
	if( bind(server_fd, (struct sockaddr*)&addr_local, sizeof(struct sockaddr)) < 0)
	{
		printf("Binding Socket to the PORT failed");
		return;
	}
	/* Create upto 10 connection queue */
	if(listen(server_fd, LOG) < 0)
	{
		printf("Listening to the Connection Failed!");
		return;
	}
	
	/* Set the server_fd in the master set and update file descriptor */
    	FD_SET(server_fd, &master);
    	fdmax = server_fd;

	while(1)
	{
		// log_debug("[TCP Server] Waiting for Connection......");
		read_fds = master;
        	if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) 
		{
            		log_error("Failure in select() function");
            		return;
        	}		
		
		// run through the existing connections looking for data to read
        	for(i = 0; i <= fdmax; i++) {
            		if (FD_ISSET(i, &read_fds)) { 
				if (i == server_fd) {
					/* Handle new connection from MCS */
					addrlen = sizeof addr_remote;
					newfd = accept(server_fd,(struct sockaddr *)&addr_remote,&addrlen);

					if (newfd == -1) {
                        			log_error("Failure in accept() function");
            					return;
					} else {
						FD_SET(newfd, &master); 
                        			if (newfd > fdmax) {
                            				fdmax = newfd;
                        			}
                        			printf("[TCP Server] Connection Established from %s (MCS Client)\n", inet_ntoa(addr_remote.sin_addr));
						memset(client_addr,0,20);
						strcpy(client_addr, inet_ntoa(addr_remote.sin_addr));
                    			}
				} else {
					/* Handle data sent from MCS */
					memset(msg_buffer,0,1024);
                    			if ((nbytes = recv(i, msg_buffer, sizeof msg_buffer, 0)) <= 0) {
                        			/* Error or connection closed by MCS */
                        			if (nbytes == 0) {
                            				/* connection closed by MCS*/
                            				printf("Connection %d terminated by MCS\n", i);
                        			} else {
                            				log_error("Failure in recv() function, connection terminated...");
                        			}
                        			close(i);
                        			FD_CLR(i, &master); // remove fd from master set
                    			} else {
                        			/* Recevieve data from MCS */
                        			strcpy(fr_name,"");
						strcpy(fr_name,MCS_DATA_DIR);

						char time_data_get[100] = "";
						get_time(time_data_get);
						strcat(fr_name,time_data_get);
						strcat(fr_name,".bin");
						printf("Saving into file %s ......", fr_name);
	
						FILE *fr = fopen(fr_name, "wb");
						if(fr == NULL) {
							log_error("File %s Cannot be opened.\n", fr_name);
						} else {
							int write_sz = fwrite(msg_buffer, sizeof(char), nbytes, fr);
							if(write_sz < nbytes) {
								log_error("Saving Failed, file size error.\n");
		    					}
						}
						printf("Complete!");
						fclose(fr);

						send_packet(fr_name);
					}
                                }
			}
		}
	}
	return;
}

int retransmit_packet(struct command_context *ctx)
{
	log_debug("Latest packet:%s", fr_name);
	send_packet(fr_name);
	return CMD_ERROR_NONE;	
}
command_t __root_command packet_command[] = {
	{
		.name = "send_packet",
		.help = "Retransmission of latest packet saved from MCS",
		.handler = retransmit_packet,
	},
};
