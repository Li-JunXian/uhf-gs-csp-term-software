/*
 * color_printf.h
 *
 *  Created on: 11/02/2011
 *      Author: johan
 */

#ifndef COLOR_PRINTF_H_
#define COLOR_PRINTF_H_

#include <stdio.h>

typedef enum color_printf_e {
	/* Colors */
#define COLOR_COLORS 0x00ff
	COLOR_NONE = 0,
	COLOR_BLACK,
	COLOR_RED,
	COLOR_GREEN,
	COLOR_YELLOW,
	COLOR_BLUE,
	COLOR_MAGENTA,
	COLOR_CYAN,
	COLOR_WHITE,
	/* Attributes */
#define COLOR_ATTRS  0xff00
	COLOR_BOLD = 0x100,
} color_printf_t;

void color_printf(color_printf_t color_arg, const char * format, ...);

#endif /* COLOR_PRINTF_H_ */
