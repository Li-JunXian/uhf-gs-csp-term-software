#ifndef GSCRIPT_H_
#define GSCRIPT_H_

#include <stdint.h>

#define GSCRIPT_DFL_PORT 22

typedef enum __attribute__((__packed__)) gscript_req_types_e {
	GSCRIPT_REQ_TYPE_EXEC,
	GSCRIPT_REQ_TYPE_EXEC_FILE,
	GSCRIPT_REQ_TYPE_STOP,
} gscript_req_type;

typedef struct __attribute__((__packed__)) {
	gscript_req_type type;
	uint16_t stack;
	uint16_t priority;
	uint32_t when;
	char cmd[];
} gscript_req_exec;

typedef struct __attribute__((__packed__)) {
	gscript_req_type type;
} gscript_req_stop;

int gscript_exec(char * script, int is_file, int stack, int priority, int when);
void gscript_stop(void);

int gscript_exec_remote(int node, int port, char * script, int is_file, int stack, int priority, int when);
void gscript_stop_remote(int node, int port);

void gscript_service_handler(csp_conn_t * conn, csp_packet_t * packet);

#endif /* GSCRIPT_H_ */
