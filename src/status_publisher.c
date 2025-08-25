#include "status_publisher.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO "/tmp/gs-status-fifo"

void status_publisher_init(void) {
    unlink(FIFO);
    if (mkfifo(FIFO, 0666) && errno != EEXIST) {
        perror("mkfifo");
        exit(1);
    }
}

static int write_all(int fd, const char *buf, size_t len) {
    size_t off = 0;
    while (off < len) {
        ssize_t n = write(fd, buf + off, len - off);
        if (n > 0) { off += (size_t)n; continue; }
        if (n < 0 && errno == EINTR) continue;
        return -1;
    }
    return 0;
}

void status_publisher_send(const char *json) {
    int fd = -1;
    for (int i = 0; i < 5 && fd < 0; i++) {
        fd = open(FIFO, O_WRONLY | O_NONBLOCK);
        if (fd < 0 && (errno == ENXIO || errno == EAGAIN)) usleep(100000);
        else if (fd < 0) return;
    }
    if (fd < 0) return;
    if (write_all(fd, json, strlen(json)) == 0) write_all(fd, "\n", 1);
    close(fd);
}
