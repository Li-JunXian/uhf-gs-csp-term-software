/**
 * @file doppler_freq_correction.h
 */

/* Select satellite number */
int mcs_sat_sel(uint32_t sat_no_sel);

/* Check satellite tracking */
int mcs_sat_read(void);

/* Update TLE */
int updatetle(void);

/* Set AX100 RX frequency*/
int ax100_set_rx_freq(uint8_t node, uint32_t timeout, uint32_t freq);

/* Set AX100 TX frequency*/
int ax100_set_tx_freq(uint8_t node, uint32_t timeout, uint32_t freq);

/* Configure LNA USBrelay feature*/
int lna_conf(int code);
