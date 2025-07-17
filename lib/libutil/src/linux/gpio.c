#include <util/gpio.h>
#include <util/sysfs_helper.h>

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <log/log.h>

LOG_GROUP(soft_gpio, "gpio");
#define LOG_DEFAULT soft_gpio

int gpio_set(struct sysfs_gpio * gpio, bool on)
{
	char *value, *dir;

	value = on ? "1" : "0";

	log_trace("Setting GPIO %s to %s", gpio->export, value);

	/* Export GPIO if not available */
	if (!gpio->exported || access(gpio->value, W_OK) != 0) {
		log_debug("GPIO %s not exported", gpio->export);

		/* Try to unexport first */
		sysfs_write_file("/sys/class/gpio/unexport", gpio->export);

		if (sysfs_write_file("/sys/class/gpio/export", gpio->export)) {
			log_warning("failed to export GPIO %s: %s", gpio->export, strerror(errno));
			return -errno;
		}

		/* Glitch-free output set */
		dir = on ? "high" : "low";
		if (sysfs_write_file(gpio->dir, dir)) {
			log_warning("failed to set GPIO %s direction: %s", gpio->export, strerror(errno));
			return -errno;
		}

		gpio->exported = true;
	}

	return sysfs_write_file(gpio->value, value);
}
