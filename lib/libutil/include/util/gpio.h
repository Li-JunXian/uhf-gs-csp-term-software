#ifndef __UTIL_GPIO_H__
#define __UTIL_GPIO_H__

#include <stdbool.h>

struct sysfs_gpio {
	const char *export;
	const char *dir;
	const char *value;
	bool exported;
};

int gpio_set(struct sysfs_gpio * gpio, bool on);

#endif /* __UTIL_GPIO_H__ */
