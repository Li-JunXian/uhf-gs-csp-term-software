#include <string.h>
#include <stdlib.h>

#include <util/log.h>
#include <csp/csp_cmp.h>
#include <param/param.h>

#include <curl/curl.h>

#include <command/command.h>
#include <time.h>
#include "predict.h"
#include "serial_rotator.h"

/* GS100/AX100 configuration parameter*/
#define AX100_PORT_RPARAM	7	/* task_server remote param port */
#define AX100_V3_RXTX_FREQ	0x00	/* uint32_t param table addr */
#define AX100_V3_PARAM_RX	1	/* Param mem 1 RX */
#define AX100_V3_PARAM_TX	5	/* Param mem 5 TX */
#define AX100_V3_ADDRESS	29	/* AX100 Primary node address */
#define AX100_V3_TIMEOUT	1000	/* Response waiting timeout in ms */

/* UHF TXRX frequency carrier freqeuncy */
//#define Lumelite_1_TX		435200500
#define Lumelite_1_TX		437075000 // L4 TX
//#define Lumelite_1_RX		435200500
//#define Lumelite_1_RX		437425000 // Lapan A2 (IO-86) Beacon
#define Lumelite_1_RX		437075000 // L4 RX
#define Lumelite_2_TX		435999500
#define Lumelite_2_RX		435999500
#define Lumelite_3_TX		436800500
#define Lumelite_3_RX		436800500

/* Mapping satellite name to Norad elementid */
//#define Lumelite_1		41171 	//e.g. VELOX-II
//#define Lumelite_1		40931	// Lapan A2 (IO-86)
//#define Lumelite_1		35935 	
#define Lumelite_1		56309	// Lumelite 4
//#define Lumelite_1 		47311
#define Lumelite_2		41168 	//e.g. ATHENOXAT 1
#define Lumelite_3		41170 	//e.g. GALASSIA 
#define MAX_SAT_SIZE		3	/* Total satellite count */


/* Satellite TLE update*/
#define CUBESAT_TLE_URL "https://celestrak.org/NORAD/elements/gp.php?INTDES=2023-057&FORMAT=tle"
#define TLE_LINE_SIZE 		120	/* Length of TLE line */
#define TLE_UPDATE_INTERVAL 	86400	/* Update TLE 24 hrs */
#define DOPPLER_UPDATE_INTERVAL	2	/* Update doppler shift correction interval */
#define TNOW_OFFSET		1200	/* Time offset 20 mins for pre-AOS time tracking*/

/* UHF Ground Station Position*/
/* Reference location: https://inetapps.nus.edu.sg/fas/geog/stationInfo.aspx */
#define LAT 			1.299033
#define LON 			103.771625
#define ALT 			50	/* metre */
#define MIN_ELEVATION		1	/* degree */

/* Auto Ping request when elevation > 20 degree*/
#define MIN_PING_ELE		20


static char TLE1[TLE_LINE_SIZE] = "";
static char TLE2[TLE_LINE_SIZE] = "";
static int TXfreq = 0;
static int RXfreq = 0;
static uint32_t sat_no = 1;	// Initialisation tracking Lumelite 1

static void doppler_tracking(int txfreq, int rxfreq, uint32_t tnow);
int mcs_sat_sel(uint32_t sat_no_sel);
int ping_sat_func(void);

int ax100_set_tx_freq(uint8_t node, uint32_t timeout, uint32_t freq)
{
	if (freq == 0)
		return 0;

	uint16_t tx_freq_addr = AX100_V3_RXTX_FREQ;
	uint16_t tx_freq_table = AX100_V3_PARAM_TX;
	uint16_t port_rparam = AX100_PORT_RPARAM;

	if (rparam_set_uint32(&freq, tx_freq_addr, tx_freq_table, node, port_rparam, timeout) <= 0)
		return 0;

	return 1;
}

int ax100_set_rx_freq(uint8_t node, uint32_t timeout, uint32_t freq)
{
	if (freq == 0)
		return 0;

	uint16_t rx_freq_addr = AX100_V3_RXTX_FREQ;
	uint16_t rx_freq_table = AX100_V3_PARAM_RX;
	uint16_t port_rparam = AX100_PORT_RPARAM;

	if (rparam_set_uint32(&freq, rx_freq_addr, rx_freq_table, node, port_rparam, timeout) <= 0)
		return 0;

	return 1;
}


static void * tleupdate(int element_id)
{
	/* Two options are proposed: First one is to get all TLE file from Celetrak and
	* look for Norad ID; Second is to pull the TLE string file is directly 
	* from local server during LEOP phase (Until Celetrak have the TLE ready).
	*/
	log_debug("TLE update (Celestrack) starting ...");
	
	/* Norad elemetid for different satllite */
	uint32_t elementid = element_id;
	
	/* Download elementid url */
	char * element_url = CUBESAT_TLE_URL;

	log_debug("Element URL %s", element_url);

	//--------------------------------------------------------------------
	CURL *curl_handle = curl_easy_init();
	curl_easy_setopt(curl_handle, CURLOPT_URL, element_url);
	curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 0);
	//--------------------------------------------------------------------

	/* Open the file tle.txt*/
	FILE * tlefile = fopen("tle.txt", "wb+");
	//FILE * tlefile = fopen("tle.txt", "rb+");
	if (!tlefile) 
	{
		log_error("Error opening TLE file tle.txt");
	}	
	
	//--------------------------------------------------------------------
	/* Write all Sat TLE param into tle.txt file */
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, tlefile);

	CURLcode err = curl_easy_perform(curl_handle);
	curl_easy_cleanup(curl_handle);
	if (err) 
	{
		log_error("Error retrieving TLE from URL '%s'", element_url);
		fclose(tlefile);
		return NULL;
	}
	//--------------------------------------------------------------------	

	/* Updating tle.txt only*/
	if (elementid == 0) 
	{
		log_debug("Updating tle.txt complete. No target satellite selected");
		fclose(tlefile);
		return NULL;
	}

	/* Searching for target satellite TLE */
	char match0[20];
	char match1[20];
	sprintf(match0, "%"PRIu32"U ", elementid);
	sprintf(match1, "%"PRIu32" ", elementid);

	/* Reset to first line */
	rewind(tlefile);

	/* For each line */
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	while ((read = getline(&line, &len, tlefile)) != -1) 
	{
		strtok(line, "\r\n");
		if (strstr(line, match0)) 
		{
			log_info("TLE1: %s", line);
			strncpy(TLE1, line, len);
		}
		if (strstr(line, match1)) 
		{
			log_info("TLE2: %s", line);
			strncpy(TLE2, line, len);
		}
	}
	free(line);
	fclose(tlefile);

	if (strlen(TLE1) == 0)
	{
		log_error("Target satellite not found, Please verify element id");
		return NULL;
	}

	/* Input satellite TLE */	
	if (setTLE( TLE1, TLE2 ) == 0){
		log_error("Track satellite: Invalid TLE");
		log_error("TLE1: '%s'", TLE1 );
		log_error("TLE2: '%s'", TLE2 );
	}

	log_warning("TLE update done");
	return NULL;
}

int updatetle(void) {
	tleupdate(0);
	return 1;
}

//static void ground_pass_in_progress(long time_aos,long time_los, int azi_offset)
static void ground_pass_in_progress(long time_aos,long time_los)
{
	if ((time_los - time_aos) < 0)
	{
		log_error("LOS time before AOS time. Please verify.")
		return;
	}
	
	/* Get current time in CSP_timestamp*/
	csp_timestamp_t clock;
	clock_get_time(&clock);
	uint32_t tnow = clock.tv_sec;
	long tnowl = tnow;

	if (tnowl >= time_aos && tnowl <= time_los)
	{
		log_warning("Ground pass begin: %.24s", ctime((time_t *) &tnowl));
		
		while (tnowl < time_los)
		{
			clock_get_time(&clock);
			tnow = clock.tv_sec;
			tnowl = tnow;

			/* Command GS100 with doppler shift correction freq */
			doppler_tracking(TXfreq, RXfreq, tnow);
			sleep(DOPPLER_UPDATE_INTERVAL);
		}
		// Reset to idle pointing orientation (STAR centre) after ground pass
		//if(serial_set_az_el(70,0,0) < 1)
		if(serial_set_az_el(70,0) < 1)
			log_debug("AZEL reset AZ: 150 EL: 0 failed");
		log_warning("Ground pass ended: %.24s", ctime((time_t *) &tnowl));
	}
	return;
}

void doppler_init()
{
	//Exit auto tracking sequence
	return;
	
	log_warning("Initialising doppler shift correction operation...");
	
	/* Get current time in CSP_timestamp*/
	csp_timestamp_t clock;
	clock_get_time(&clock);
	uint32_t tnow = clock.tv_sec;
	log_debug("Time now is: %.24s %u", ctime((time_t *) &tnow), tnow);

	/*Set latitude, longitude and altitude for the UHF ground station */
	setStation(LAT, LON, ALT);

	/* Update TLE for the target satellite.  */
	mcs_sat_sel(sat_no); 
	
	long time_aos_prev = 0;

	while (1)
	{			
		/* Estimate next ground pass AOS and LOS time*/
		set_calc_time(tnow - (TNOW_OFFSET));
		PreCalc();
		Calc();

		/* Find time for begin and end of pass */
		long time_aos = floor(86400.0 * (3651.0 + NextAOS()));
		long time_los = floor(86400.0 * (3651.0 + FindLOS2()));

		/* Find AZ at AOS */
		set_calc_time(time_aos);
		PreCalc();
		Calc();
		int azi_aos = get_azi();

		/* Find AZ at LOS */
		set_calc_time(time_los);
		PreCalc();
		Calc();
		int azi_los = get_azi();
		
		/* Find Max Elevation */
		long time_maxele = (time_aos+time_los)/2;
		set_calc_time(time_maxele);
		PreCalc();
		Calc();
		int azi_m = get_azi();
		int ele_m = get_ele();
		// Offset for azimuth rotation range
		//int azi_offset;
		if(time_aos != time_aos_prev)
		{
			log_info("AOS: %.24s %lu @ %u deg azimuth", ctime((time_t *) &time_aos), time_aos, azi_aos);
			log_info("LOS: %.24s %lu @ %u deg azimuth", ctime((time_t *) &time_los), time_los, azi_los);
			log_info("Max Elevation: %u deg elevation %u deg azimuth @ %.24s", ele_m, azi_m, ctime((time_t *) &time_maxele));
		
			log_warning("Ground pass initialisation done.");
	
			/* Determine if ground pass is skipped: min elevation */
			log_debug("Preparing for next ground pass ...")
		}

		time_aos_prev = time_aos;
		sleep(1);
		// ground_pass_in_progress(time_aos,time_los, azi_offset);
		ground_pass_in_progress(time_aos,time_los);

		/* Update tnow */
		clock_get_time(&clock);
		tnow = clock.tv_sec;

		
		//long tnowl = tnow;
		//log_debug("Current time: %.24s", ctime((time_t *) &tnowl));
		//log_debug("Next LOS: %.24s", ctime((time_t *) &time_los));
		//log_info("Current satellite number: %u", sat_no)

		/* Auto increment sat_no to track for next ground pass*/
		//if ( (tnow) > time_los)
		//{
		//	if( (sat_no + 1) <= MAX_SAT_SIZE )
		//		sat_no += 1;
		//	else
		//		sat_no = 1;
		//	log_debug("Tracking Satellite No. %d in next ground pass", sat_no)
		//	mcs_sat_sel(sat_no);
		//}
		/* Check if MCS update sat_no */
		// check sat_select file, if len() != 0 or sat_no != sat
	}
	
	return;	
}

static void doppler_tracking(int txfreq, int rxfreq, uint32_t tnow)
{
	

	/* Update predicted timestamp */
	set_calc_time(tnow);

	/**	This function copies TLE data from PREDICT's sat structure
	*	to the SGP4 single dimensioned tle structure.
	*	Must be run once before doing calc.	*/
	PreCalc();

	/* Repetively calculate and update the sat info */	
	Calc();
	
	/* Get ground pass parameter */
	double az = get_azi();
	double el = get_ele();
	double satlat = get_satlat();
	double satlon = get_satlon();
	double satalt = get_satalt();
	double satvel = get_satvel();

	/* Track satellite with AZ and EL */
	int azi = az;
	int eli = el;
	
	if (eli > 90) {
		log_debug("AZEL is not set (>90 deg elevation)");
	} else { 
		serial_set_az_el(azi,eli);
		//serial_set_az_el(azi,eli,azi_offset);	
	}
	
	/* Compute doppler frequency shift */
	uint32_t rx_freq = 0;
	rx_freq = comp_dopp_frq(rxfreq,0);

	uint32_t tx_freq = 0;
	tx_freq = comp_dopp_frq(txfreq,1);

	log_info("AZ: %f, EL: %f, RX: %"PRIu32" TX: %"PRIu32, az, el, rx_freq, tx_freq);
	log_info("Sat Latitude: %f, Longitude: %f, Altitude: %f km, Velocity: %f km/s", satlat, satlon, satalt, satvel);
	
	/* Impose minimum elevation -> start of Ground pass */
	if (el < MIN_ELEVATION){
		log_debug("Not avalible for ground pass tracking (Min_elevation %d deg)", MIN_ELEVATION);
	} else {
		/* Configure the GS100 TXRX frequency */
		if (!ax100_set_tx_freq(AX100_V3_ADDRESS, AX100_V3_TIMEOUT, tx_freq))
			log_error("Failed to set Doppler Correction TX Freq. Please check GS100 avaliability");
		if (!ax100_set_rx_freq(AX100_V3_ADDRESS, AX100_V3_TIMEOUT, rx_freq))
			log_error("Failed to set Doppler Correction RX Freq. Please check GS100 avaliability");
	}
	/* Auto ping satellite when elevation > MIN_PING_ELE*/
	if(el > MIN_PING_ELE) {
		ping_sat_func();
	}

	return;
}

int ping_sat_func(void){
	log_debug("Ping satellite at node address 25 ...");
	csp_packet_t *packet = csp_buffer_get(SIZE_MAX);
	packet->length = 1;
	packet->data[0] = 0x00;
	//csp_sendto(pri, dst_node, dst_port, src_port, flag, packet, timeout)
	if(csp_sendto(2, 25, 1, 17, 0, packet, 1000) == -1){
		log_debug("Fail to send PING 25 csp packet");
	}
	return 0;
}

int mcs_sat_sel(uint32_t sat_no_sel)
{
	uint8_t node = AX100_V3_ADDRESS;
	uint32_t timeout = AX100_V3_TIMEOUT;
	int element_id = 0;
	
	/* Read parameter file for sat_no */
	if( (sat_no_sel) <= MAX_SAT_SIZE )
		sat_no = sat_no_sel;
	else 
		log_error("Invalid selection. Please select from Sat 1 upto Sat %d", MAX_SAT_SIZE);

	switch (sat_no)
	{
	case 2:
		TXfreq = Lumelite_2_TX;
		RXfreq = Lumelite_2_RX;
		element_id = Lumelite_2;
		break;
	case 3:		
		TXfreq = Lumelite_3_TX;
		RXfreq = Lumelite_3_RX;
		element_id = Lumelite_3;
		break;
	default:
		TXfreq = Lumelite_1_TX;
		RXfreq = Lumelite_1_RX;
		element_id = Lumelite_1;
		break;
	}


	/* Get latest TLE of the selected satellite */
	log_info("Satellite Element ID: %d",element_id);
	tleupdate(element_id);

	if (!ax100_set_tx_freq(node, timeout, TXfreq))
		return 0;
	if (!ax100_set_rx_freq(node, timeout, RXfreq))
		return 0;
	//log_info("Select Lumelite %u TX: %d Rx: %d", sat_no, TXfreq, RXfreq);
	log_info("Select Lumelite 4 TX: %d Rx: %d", TXfreq, RXfreq);

	return 1;
}

int mcs_sat_read(void)
{
	log_debug("MCS_SAT_READ: Tracking Satellite %d\n", sat_no);
	return 	sat_no;
}

int lna_conf(int code)
{
	log_debug("LNA_function");
	int state = 0;
	if(code == 0)
		state = system("sudo usbrelay");
	else if (code == 1)
		state = system("sudo usbrelay R_1=1");
	else
		state = system("sudo usbrelay R_1=0");

	if(state == 0)
		return 0;
	else
		return 1;
}

void tleupdate_init()
{
	//Exit tle update
	return;

	int init_run = 1;

	while(1) 
	{
		/* update tle.txt file every 24 hrs*/
		if (init_run)
		{
			log_debug("Auto TLE update in the next 24 hrs ...");
			sleep(TLE_UPDATE_INTERVAL);
			init_run = 0;
		} else {
			init_run = 1;
			tleupdate(0);
		}
	}

	log_error("Unexpected termination of auto-TLE update function. Please restart");
	return;
}

/* -------------------------------------------------------------------------------- */
/* csp-term command line function */
int tx_freq_set(struct command_context *ctx)
{
	log_info("Set Tx Frequency");
	
	if (ctx -> argc != 2)
		return CMD_ERROR_SYNTAX;
	int tx_freq = atoi(ctx->argv[1]);
	
	uint8_t node = AX100_V3_ADDRESS;
	uint32_t timeout = AX100_V3_TIMEOUT;

	if (!ax100_set_tx_freq(node, timeout, tx_freq))
		return CMD_ERROR_FAIL;

	return CMD_ERROR_NONE;
}

int rx_freq_set(struct command_context *ctx)
{
	log_info("Set Rx Frequency");
	
	if (ctx -> argc != 2)
		return CMD_ERROR_SYNTAX;
	int rx_freq = atoi(ctx->argv[1]);
	
	uint8_t node = AX100_V3_ADDRESS;
	uint32_t timeout = AX100_V3_TIMEOUT;

	if (!ax100_set_rx_freq(node, timeout, rx_freq))
		return CMD_ERROR_FAIL;

	return CMD_ERROR_NONE;
}

int dop_test(struct command_context *ctx)
{
	/* Initiallising dopper ground pass operation */
	doppler_init();
	
	return CMD_ERROR_NONE;
}

int sel_sat(struct command_context *ctx)
{
	if (ctx -> argc != 2)
		return CMD_ERROR_SYNTAX;

	int sat_no_sel = atoi(ctx->argv[1]);

	if (!mcs_sat_sel(sat_no_sel))
		return CMD_ERROR_FAIL;

	return CMD_ERROR_NONE;	
}

int lna_read(struct command_context *ctx)
{
	int st = lna_conf(0);
	if(st == 1)
		log_debug("Unable to config LNA usbrelay, please check");
	return CMD_ERROR_NONE;	
}
int lna_on(struct command_context *ctx)
{
	int st = lna_conf(1);
	if(st == 1)
		log_debug("Unable to config LNA usbrelay, please check");
	return CMD_ERROR_NONE;	
}
int lna_off(struct command_context *ctx)
{
	int st = lna_conf(2);
	if(st == 1)
		log_debug("Unable to config LNA usbrelay, please check");
	return CMD_ERROR_NONE;	
}

int read_sat(struct command_context *ctx)
{
	mcs_sat_read();
	return CMD_ERROR_NONE;	
}
int ping_sat(struct command_context *ctx)
{
	ping_sat_func();
	return CMD_ERROR_NONE;	
}
/* ------------------------------------------------------------------------------*/
/* csp-term command line options */
command_t __root_command doppler_command[] = {
	{
		.name = "tx_freq_set",
		.help = "Set GS Primary AX100 TX frequency",
		.handler = tx_freq_set,
	},
};

command_t __root_command doppler_command1[] = {
	{
		.name = "rx_freq_set",
		.help = "Set GS Primary AX100 RX frequency",
		.handler = rx_freq_set,
	},
};

command_t __root_command doppler_command2[] = {
	{
		.name = "mcs_sat_sel",
		.help = "Select satellite to track",
		.handler = sel_sat,
	},
};

command_t __root_command dopp_command[] = {
	{
		.name = "doppler",
		.help = "Doppler correction",
		.handler = dop_test,
	},
};
/* Command to send a customed Ping comamnd to satellite*/
command_t __root_command pingsat_command[] = {
	{
		.name = "ping_sat",
		.help = "Send a customed satllite ping",
		.handler = ping_sat,
	},
};
/* Command to read,enable and disable lna usb relay*/
command_t __root_command lna_command[] = {
	{
		.name = "lna_read",
		.help = "Read state of LNA usbrelay",
		.handler = lna_read,
	},
};
command_t __root_command lna_command1[] = {
	{
		.name = "lna_on",
		.help = "Enable LNA usbrelay switch",
		.handler = lna_on,
	},
};
command_t __root_command lna_command2[] = {
	{
		.name = "lna_off",
		.help = "disable LNA usbrelay switch",
		.handler = lna_off,
	},
};
