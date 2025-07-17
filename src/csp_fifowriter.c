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
#include <sys/types.h>
#include <poll.h>
#include <limits.h>
#include <time.h>

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


int csp_fifowriter(char * string_input) //writes into the fifo with the selected character
{

	int fd2; //fd1 for fifo
	char * fifo_down = "/home/kelvin/Desktop/GS_Server_Folder/fifo_downlink"; // FIFO file path

	// mkfifo(<pathname>, <permission>) 
	mkfifo(fifo_down, 0666);
 
	// Open FIFO for write only
	//fd2 = open(fifo_down, O_CREAT | O_WRONLY);
	fd2 = open(fifo_down, O_CREAT, O_WRONLY);
	if (fd2<0){
		printf("fifo_downlink file cannot be created\r\n");
		}

	char arr3[200];
	strcpy(arr3,""); //reset
	strcpy(arr3,string_input); 

	int w_fd2 = write(fd2, arr3, 200);
	if(w_fd2 == 0)
		log_error("Error: write fd2 fail.Please check...");
	close(fd2); 
	return 0;
}
