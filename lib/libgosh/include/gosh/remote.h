/**
 * @file redirect.h
 *
 * This implements access to GOSH or system shell over CSP
 *
 * @author Jeppe Ledet-Pedersen
 * Copyright 2015 GomSpace ApS. All rights reserved.
 */

#ifndef _GOSH_REMOTE_H_
#define _GOSH_REMOTE_H_

#include <stdbool.h>
#include <csp/csp.h>

#include <log/log.h>

LOG_GROUP_EXTERN(log_gosh_remote);

#define SHELL_INP_SIZE		80
#define SHELL_OUTP_SIZE		80

/* Types types */
#define GOSH_TYPE_SYS_SHELL_REQ	0 /* System (e.g. Linux) shell */
#define GOSH_TYPE_SYS_SHELL_REP	1 /* System (e.g. Linux) shell */

/* Shell request */
struct gosh_sys_shell_req {
	uint8_t type;		/* Must be GOSH_TYPE_SYS_SHELL_REQ */
	uint8_t reserved;
#define GOSH_FLAG_CHAR	(1 << 0)
#define GOSH_FLAG_DONE	(1 << 1)
	uint16_t flags;
	char input[SHELL_INP_SIZE];
} __attribute__((packed));

#define shell_request_size(_chars) (sizeof(struct gosh_sys_shell_req) - sizeof(((struct gosh_sys_shell_req *)0)->input) + _chars)

/* Shell reply */
struct gosh_sys_shell_rep {
	uint8_t type;		/* Must be GOSH_TYPE_SYS_SHELL_REP */
	uint8_t err;
	uint16_t flags;
	char output[SHELL_OUTP_SIZE];
} __attribute__((packed));

#define shell_reply_size(_chars) (sizeof(struct gosh_sys_shell_rep) - sizeof(((struct gosh_sys_shell_rep *)0)->output) + _chars)

int gosh_remote_shell_process(csp_conn_t *conn);

int gosh_remote_shell(unsigned int node, unsigned int port, unsigned int timeout, bool raw, char *command);

#endif /* _GOSH_REMOTE_H_ */
