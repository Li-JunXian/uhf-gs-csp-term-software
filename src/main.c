/**
 * CSP-Terminal
 *
 * @author Johan De Claville Christiansen
 * Copyright 2010-2015 GomSpace ApS. All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
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

//---------------------------------------------------------------------------------------------
const vmem_t vmem_map[] = {{0}};

static void print_help(void) {
	printf(" usage: csp-term <-d|-c|-z> [optargs]\r\n");
	printf("  -d DEVICE,\tSet device (default: /dev/ttyUSB0)\r\n");
	printf("  -c DEVICE,\tSet can device (default: can0)\r\n");
	printf("  -z SERVER,\tSet ZMQ server (default: localhost)\r\n");
	printf("  -a ADDRESS,\tSet address (default: 8)\r\n");
	printf("  -b BAUD,\tSet baud rate (default: 500000)\r\n");
	printf("  -h,\t\tPrint help and exit\r\n");
}

static void exithandler(void) {
	console_exit();
}

static void sig_handler(int signo, siginfo_t *si, void *context)
{
	console_exit();

	printf("\n");

	exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------
//START OF MAIN FUNCTION
int main(int argc, char * argv[]) {

	struct sigaction sa;

	/* Config */
	uint8_t addr = 8;

	/* KISS STUFF */
	char * device = "/dev/ttyUSB0";
	uint32_t baud = 500000;
	uint8_t use_kiss = 0;

	/* CAN STUFF */
	char * ifc = "can0";
	uint8_t use_can = 0;

	/* ZMQ STUFF */
	char zmqhost[100] = "localhost";
	uint8_t use_zmq = 0;

	/* Console exit */
	atexit(exithandler);

	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = sig_handler;
	sigaction(SIGSEGV, &sa, NULL);
	sigaction(SIGFPE,  &sa, NULL);
	sigaction(SIGINT,  &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);

	/**
	 * Parser
	 **/
	int c;
	while ((c = getopt(argc, argv, "a:b:c:d:hz:")) != -1) {
		switch (c) {
		case 'a':
			addr = atoi(optarg);
			break;
		case 'b':
			baud = atoi(optarg);
			break;
		case 'c':
			use_can = 1;
			ifc = optarg;
			break;
		case 'd':
			device = optarg;
			use_kiss = 1;
			break;
		case 'h':
			print_help();
			exit(0);
		case 'z':
			strcpy(zmqhost, optarg);
			use_zmq = 1;
			break;
		case '?':
			return 1;
		default:
			exit(EXIT_FAILURE);
		}
	}

	/**
	 * CSP / Network
	 */
	csp_set_hostname("csp-term");
	csp_set_model("CSP Term");
	csp_set_revision(CSPTERM_VERSION);
	//csp_buffer_init(400, 512);
	csp_buffer_init(1024, 1024);
	csp_init(addr);
	log_csp_init();
	csp_rdp_set_opt(6, 30000, 16000, 1, 8000, 3);

	/**
	 * KISS interface
	 */
	if (use_kiss == 1) {
		static csp_iface_t csp_if_kiss;
		static csp_kiss_handle_t csp_kiss_driver;
		static const char * kiss_name = "KISS";
		csp_route_set(CSP_DEFAULT_ROUTE, &csp_if_kiss, CSP_NODE_MAC);

		csp_kiss_init(&csp_if_kiss, &csp_kiss_driver, usart_putc, usart_insert, kiss_name);
		struct usart_conf conf = {.device = device, .baudrate = baud};
		usart_init(&conf);
		void my_usart_rx(uint8_t * buf, int len, void * pxTaskWoken) {
			csp_kiss_rx(&csp_if_kiss, buf, len, pxTaskWoken);
		}
		usart_set_callback(my_usart_rx);
	}

	/**
	 * ZMQ interface
	 */
	if (use_zmq == 1) {
		csp_zmqhub_init(addr, zmqhost);
		csp_route_set(CSP_DEFAULT_ROUTE, &csp_if_zmqhub, CSP_NODE_MAC);
	}

	/**
	 * CAN Interface
	 */
	if (use_can == 1) {
		struct csp_can_config conf = {.ifc = ifc};
		csp_can_init(CSP_CAN_MASKED, &conf);
		csp_route_set(CSP_DEFAULT_ROUTE, &csp_if_can, CSP_NODE_MAC);
	}

	/**
	 * liblog setup
	 */
	#define LOG_STORE_SIZE 0x200
	static uint8_t buf[LOG_STORE_SIZE] = {};
	log_store_init((vmemptr_t) buf, LOG_STORE_SIZE);

	/**
	 * Parameter system
	 */
	param_index_set(0, (param_index_t) {.table = param_test, .count = PARAM_TEST_COUNT, .physaddr = malloc(PARAM_TEST_SIZE), .size = PARAM_TEST_SIZE});

	/**
	 * Tasks
	 */

	/* Router */
	csp_route_start_task(1000, 0);

	/* CSP task server thread*/
	printf("Running modified csp-term\r\n");
	void * task_server(void * parameters); //running the server to receive data from satellite RF transmission
	static pthread_t handle_server;
	pthread_create(&handle_server, NULL, task_server, NULL);

	/* Satellite TLE update thread */
	void * tleupdate_init();
	static pthread_t handle_tleupdate;
	pthread_create(&handle_tleupdate, NULL, tleupdate_init, NULL);		// (Comment out to disable antenna tracking)
	
	/* Doppler shift compensation thread */
	void * doppler_init();
	static pthread_t handle_doppler;
	pthread_create(&handle_doppler, NULL, doppler_init, NULL);		// (Comment out to disable antenna tracking)

	// Tcp server thread
	void * tcp_server();
	static pthread_t handle_tcp;
	pthread_create(&handle_tcp, NULL, tcp_server, NULL);
	
	/* Debug console thread */
	command_init();
	console_init();
	console_set_hostname("csp-term");
	static pthread_t handle_console;
	pthread_create(&handle_console, NULL, debug_console, NULL);

	/* Wait here for console to end */
	pthread_join(handle_console, NULL);
	pthread_join(handle_server, NULL);
	pthread_join(handle_doppler, NULL);					// (Disable antenna tracking)
	pthread_join(handle_tleupdate, NULL);					// (Disable antenna tracking)
	pthread_join(handle_tcp, NULL);

	return 0;

}
//END OF MAIN FUNCTION
