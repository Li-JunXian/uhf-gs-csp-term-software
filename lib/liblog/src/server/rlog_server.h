/**
 * GomSpace Log System
 *
 * @author Kristian Bay
 * Copyright 2015 GomSpace ApS. All rights reserved.
 */

#ifndef RLOG_SERVER_H_
#define RLOG_SERVER_H_

#include <csp/csp.h>
#include <csp/arch/csp_thread.h>

void rlog_service_handler(csp_conn_t * conn, csp_packet_t * request_packet);

#endif /* RLOG_SERVER_H_ */
