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

/*Create own command */
#include <command/command.h>

void reverse_String(char Str[], int i, int len);

// Implementation of itoa() 
char * itoa(int num, char* str, int base) 
{ 
    int i = 0; 
    bool isNegative = false;
    int len;
  
    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0) 
    { 
        str[i++] = '0'; 
        str[i] = '\0'; 
        return str; 
    } 
  
    // In standard itoa(), negative numbers are handled only with  
    // base 10. Otherwise numbers are considered unsigned. 
    if (num < 0 && base == 10) 
    { 
        isNegative = true; 
        num = -num; 
    } 
  
    // Process individual digits 
    while (num != 0) 
    { 
        int rem = num % base; 
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0'; 
        num = num/base; 
    } 
  
    // If number is negative, append '-' 
    if (isNegative) 
        str[i++] = '-';
 
  
    str[i] = '\0'; // Append string terminator

    //Reverse the output to get the correct output
    len = strlen(str);
    reverse_String(str, 0, len -1); 

    return NULL;
}

void reverse_String(char Str[], int i, int len)
{
	char temp;
	temp = Str[i];
	Str[i] = Str[len - i];
	Str[len - i] = temp;
	
  	if (i == len/2)
  	{
		return;
  	}
  	reverse_String(Str, i + 1, len);
}


