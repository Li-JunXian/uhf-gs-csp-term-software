/**
 * @file doppler_freq_correction.h
 */

#include <stddef.h>
#include <stdint.h>

/* Select satellite number */
int mcs_sat_sel(uint32_t sat_no_sel);

/* Check satellite tracking */
int mcs_sat_read(void);

/* Update TLE */
int updatetle(void);

/* Retrieve most recent doppler compensation parameters */
int doppler_get_tx_freq(void);
int doppler_get_rx_freq(void);
void doppler_get_tle(char *line1, size_t line1_len, char *line2, size_t line2_len);

/* Update doppler compensation parameters */
int doppler_set_tx_freq(uint32_t freq);
int doppler_set_rx_freq(uint32_t freq);

/* Set AX100 RX frequency*/
int ax100_set_rx_freq(uint8_t node, uint32_t timeout, uint32_t freq);

/* Set AX100 TX frequency*/
int ax100_set_tx_freq(uint8_t node, uint32_t timeout, uint32_t freq);

/* Configure LNA USBrelay feature*/
int lna_conf(int code);
