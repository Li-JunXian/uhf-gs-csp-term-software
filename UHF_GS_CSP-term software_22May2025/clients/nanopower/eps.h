#ifndef INCLUDE_EPS_H_
#define INCLUDE_EPS_H_

#include <param/param.h>

/** FRAM MEMORY MAP (From 0x6000 to 0x7FFF is write-protected) */
#define EPS_FRAM_HK				0x5000
#define EPS_FRAM_GNDWDT			0x5F00

#define EPS_FRAM_LOG			0x3000
#define EPS_FRAM_LOG_SIZE		0x1000

/** FRAM FILENAMES */
#if (PARAM_MAX_FILESIZE != 0x400)
#error "Invalid filesize"
#endif

#define EPS_FNO_PARAM			0
#define EPS_FNO_BOARD			2
#define EPS_FNO_CAL				3

#define EPS_FNO_PARAM_DFL		24
#define EPS_FNO_BOARD_DFL		26
#define EPS_FNO_CAL_DFL			27

/** PARAM INDEX MAP */
#define EPS_PARAM				0
#define EPS_HK					1
#define EPS_BOARD				2
#define EPS_CAL					3

#define EPS_SCRATCH_SIZE		0xA0
#define EPS_SCRATCH				4

#endif /* INCLUDE_EPS_H_ */
