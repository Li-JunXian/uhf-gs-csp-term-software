#include "status_publisher.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO "/tmp/gs_status_fifo"

void status_publisher_init(void) {
    unlink(FIFO);
    if (mkfifo(FIFO, 0666) && errno != EEXIST) {
        perror("mkfifo");
        exit(1);
    }
}

void status_publisher_send(const char *json) {
    int fd = open(FIFO, O_WRONLY | O_NONBLOCK);
    if (fd < 0) return;           // no reader yet
    write(fd, json, strlen(json));
    write(fd, "\n", 1);
    close(fd);
}
