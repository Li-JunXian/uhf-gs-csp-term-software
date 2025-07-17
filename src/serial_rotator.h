/**
 * @file serial_rotator.h
 */

/* Serial communication configuration setting */
int set_interface(int fd, int speed, int parity);

/* Set UHF antenna Azimuth and Elevation angles */
int serial_set_az_el(int azi,int ele);

/* Read for current UHF antenna Azimuth and Elevation angles */
int serial_read(void);
