/**
 * @file color_printf.c
 * Prints with nice colors
 *
 * @author Johan De Claville Christiansen
 * Copyright 2011 GomSpace ApS. All rights reserved.
 */

#include <stdarg.h>

#include <util/log.h>
#include <util/color_printf.h>

void color_printf(color_printf_t color_arg, const char * format, ...) {

	va_list args;
	va_start(args, format);

	if ((color_arg & COLOR_ATTRS) == COLOR_BOLD) {
		printf("\033[1;");
	} else {
		printf("\033[0;");
	}

	switch(color_arg & COLOR_COLORS) {
	case COLOR_NONE:
		printf("0m");
		break;
	case COLOR_BLACK:
		printf("30m");
		break;
	case COLOR_RED:
		printf("31m");
		break;
	case COLOR_GREEN:
		printf("32m");
		break;
	case COLOR_YELLOW:
		printf("33m");
		break;
	case COLOR_BLUE:
		printf("34m");
		break;
	case COLOR_MAGENTA:
		printf("35m");
		break;
	case COLOR_CYAN:
		printf("36m");
		break;
	case COLOR_WHITE:
		printf("37m");
		break;
	default:
		break;
	}

	vprintf(format, args);
	printf("\033[0m");

    va_end(args);

}
