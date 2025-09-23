#include <util/sysfs_helper.h>
#include <log/log.h>

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

int sysfs_write_file(const char *path, const char *value)
{
	int fd, len, bytes;

	log_trace("sysfs: write %s to %s", value, path);

	fd = open(path, O_WRONLY);
	if (fd < 0)
		return -ENOENT;

	len = strlen(value);
	bytes = write(fd, value, len);
	close(fd);

	return len == bytes ? 0 : -EINVAL;
}
