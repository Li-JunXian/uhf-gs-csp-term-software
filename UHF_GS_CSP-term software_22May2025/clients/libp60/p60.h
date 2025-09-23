#ifndef _P60_H_
#define _P60_H_

#define P60_PORT_RPARAM        7
#define P60_PORT_GNDWDT_RESET  9
#define P60_PORT_CMDCONTROL    10
#define P60_PORT_GSCRIPT       22

/** FRAM MEMORY MAP */
#define P60_FRAM_BOARD         0x0000

/** FRAM FILENAMES */
#define P60_FNO_BOARD          0
#define P60_FNO_BOARD_DFL      4

#define P60_FRAM_WP_BEGIN     (0x1000)
#define P60_FRAM_WP_END       (0x1C00 - 1)

/** GND WD FRAM ADDR **/
#define P60_FRAM_GNDWDT        0x1F00

/** PARAM INDEX MAP */
#define P60_BOARD_PARAM        0

#define DEVICE_FM24CL64B       0

typedef enum {
	UNKNOWN_RST = 0,
	GND_WDT_RST,
	I2C_WDT_RST,
	CAN_WDT_RST,
	EXT_HARD_RST,
	EXT_SOFT_RST,
} p60_reset_cause_t;

extern const uint8_t board_fallback_type;
extern const uint8_t csp_fallback_addr;
extern const uint8_t board_rs422_mode;

extern void module_init_early(void);
extern void module_init(void);
extern void wdt_gnd_clear(void);
extern uint16_t command_control(uint8_t *packet, uint16_t length);
extern void module_task(void * pvParameters);

#endif /* _P60_H_ */
