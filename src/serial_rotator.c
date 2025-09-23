/* G-5500 antenna rotator - GS232B rotator computer controller */
// Pin 6 : Provide 2 to 4.5 VDC corresponding to 0 to 450 degree -- Azimuth
// Pin 1 : Provide 2 to 4.5 VDC corresponding to 0 to 180 degree -- Elevation
// Pin 4 : Connect to Pin 8 to rotate left (counter-clockwise)
// Pin 2 : Connect to Pin 8 to rotate right (clockwise)
// Pin 5 : Connect to Pin 8 to rotate down
// Pin 3 : Connect to Pin 8 to rotate up
// Pin 7 : Provide 13 V to 6 V at up to 200 mA
// Pin 8 : Common ground

/* G-5500 antenna rotator - Azimuth and Elevation rotator */
// A1 : Voltage reading at High
// A2 : Voltage reading at variable location
// A3 : Voltage reading at LOW (0V)
// A4, A5, A6 : Motor power (see G-5500 Schematic Diagram)
// E1 : Voltage reading at High
// E2 : Voltage reading at variable location
// E3 : Voltage reading at LOW (0V)
// E4, E5, E6 : Motor power (see G-5500 Schematic Diagram)

#include <fcntl.h> 
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <util/log.h>
#include <command/command.h>
#include <stdio.h>

#include "serial_rotator.h"

#define AZIMMUTH_ANGLE_OFFSET	80
//#define AZIMMUTH_ANGLE_OFFSET	0
#define MIN_ANGLE_THRESHOLD	2
// set AZEL CMD lock to '1' to prevent antenna movemnet else '0'
#define AZEL_TRACK_CMD_LOCK	0

static char *serial_port = "/dev/ttyUSB1";
static int azi_old = -1;
static int ele_old = -1;

int set_interface(int fd, int speed, int parity);
int serial_set_az_el(int azi,int ele);
int serial_read(void);

int set_interface(int fd, int speed, int parity)
{
        struct termios options;
        if (tcgetattr (fd, &options) != 0)
        {
                log_error("error in getting serial port configuration");
                return -1;
        }

        cfsetospeed (&options, speed);	// set output baud rate
        cfsetispeed (&options, speed);	// set input baud rate

        options.c_cflag = (options.c_cflag & ~CSIZE) | CS8;     // 8-bit chars

	options.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);

        options.c_lflag = 0;	// no signaling chars, no echo,no canonical processing

	options.c_oflag &= ~OPOST;

        options.c_cc[VMIN]  = 15;	// read number of bytes
        options.c_cc[VTIME] = 255;	// read timeout

        options.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff hardware control

        options.c_cflag |= (CLOCAL | CREAD);	// ignore modem controls options

	/* No parity (8N1) */
        options.c_cflag &= ~(PARENB | PARODD);      // shut off parity bits
        options.c_cflag |= parity;
        options.c_cflag &= ~CSTOPB;
        options.c_cflag &= ~CRTSCTS;

        if (tcsetattr (fd, TCSANOW, &options) != 0)	// set configured setting
        {
                log_error("error in setting serial port configuration");
                return -1;
        }
        return 0;
}

int serial_read()
{
	
	int fd = open (serial_port, O_RDWR | O_NOCTTY);
	if (fd < 0)
	{
        	log_error("error connecting GS232B serial port");
        	return 0;
	}else{
		log_info("GS232B serial port connected");
	}

	if (set_interface(fd, B4800, 0) != 0)  // set speed to 9600 bps, 8n1 (no parity)
	{
		log_error("error interface setup");
		return 0;
	}	

	tcflush(fd, TCIFLUSH); //dicard old data and flush the buffers

  	char read_buf[]= "C2\r";

	int wr = 0;
	int rd = 0;
	char buf [100] = "";

	wr = write(fd,&read_buf,sizeof(read_buf)-1);
	log_info("Send %d bytes: %s",wr,read_buf);
	//usleep(100);	// delay for GS232B to respond	
	
	rd = read (fd, buf, sizeof buf);  // read up to 100 characters
	log_info("Receive %d bytes: %s",rd, buf);

	close(fd);

	return 1;
}

int serial_set_az_el(int azi,int ele)
{
	/* Follow up actions Feb 2023: as the antenna is azimuth from 0 to 450 degree and	*/
	/* 90 degree on GS232B reading is 0 degree North. Need to configure acceptable		*/
	/* movment range according the AOS, LOS and MAX Elevation pre-computed values.		*/

	/* AZEL disable CMD line */
	if(AZEL_TRACK_CMD_LOCK == 1)
	{
		log_info("AZEL CMD locked.");
		return 1;
	}
	
	if (azi_old < 0 || ele_old < 0)
	{
		azi_old = azi;
		ele_old = ele;	
	}else{
		log_debug("PreAZ: %d PreEL: %d",azi_old,ele_old);
		if(abs(azi - azi_old) < MIN_ANGLE_THRESHOLD && abs(ele - ele_old) < MIN_ANGLE_THRESHOLD)
			return 0;
	}

	azi_old = azi;
	ele_old = ele;

	int fd = open (serial_port, O_RDWR | O_NOCTTY);
	if (fd < 0)
	{
        	log_error("error connecting GS232B serial port");
        	return 0;
	}

	if (set_interface(fd, B4800, 0) != 0)  // set speed to 9600 bps, 8n1 (no parity)
	{
		log_error("error interface setup");
		return 0;
	}	

	tcflush(fd, TCIFLUSH); //dicard old data and flush the buffers

	char set_az_el[] = "W150 000\r"; //default antenna orientation

	int wr = 0;
	
	sprintf(set_az_el+1,"%03d %03d\r", azi+AZIMMUTH_ANGLE_OFFSET, ele);


	log_info("%s",set_az_el);
	wr = write(fd,&set_az_el,sizeof(set_az_el)-1);
	if (wr == 0)
		log_error("Setting AZ:%d EL%d failed",azi,ele);

	close(fd);

	return 1;
}
/* ------------------------------------------------------------------------------- */
int read_azel(struct command_context *ctx)
{

	
	if(serial_read())
	{
		log_debug("read AZ EL success");
	}else{
		log_debug("read AZ EL failed");
	}

	return CMD_ERROR_NONE;
}

int set_azel(struct command_context *ctx)
{
	
	if (ctx -> argc != 3)
		return CMD_ERROR_SYNTAX;

	int azi = atoi(ctx->argv[1]);
	int ele = atoi(ctx->argv[2]);
	
	if(serial_set_az_el(azi,ele))
	{
		log_debug("Set AZ EL success");
	}else{
		log_debug("Set AZ EL failed");
	}

	return CMD_ERROR_NONE;
}

/* Create csp-term command line options */
command_t __root_command thread_command_azel[] = {
	{
		.name = "read_azel",
		.help = "read GS232B for current AZ EL",
		.handler = read_azel,
	},
};

command_t __root_command thread_command_azel_1[] = {
	{
		.name = "set_azel",
		.help = "set AZ EL (3-digit int per coordinate)",
		.handler = set_azel,
	},
};
