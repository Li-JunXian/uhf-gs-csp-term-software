/**
 * NanoCom firmware
 *
 * @author Johan De Claville Christiansen
 * Copyright 2014 GomSpace ApS. All rights reserved.
 */

#include <stddef.h>
#include <csp/csp.h>
#include <csp/csp_endian.h>
#include <param/param.h>
#include <ax100.h>

int ax100_get_hk(param_index_t * mem, uint8_t node, uint32_t timeout) {

	uint8_t query_buf[RPARAM_QUERY_MAX_LEN+10] = {};
	rparam_query * query = (void *) &query_buf;

	query->action = RPARAM_GET;
	query->mem = AX100_PARAM_TELEM;
	query->length = 0;
	query->checksum = csp_hton16(0xB00B);

	if (csp_transaction(CSP_PRIO_HIGH, node, AX100_PORT_RPARAM, timeout, query, offsetof(rparam_query, addr), query_buf, -1) <= 0) {
		return -1;
	}

	/* We have a reply */
	query->length = csp_ntoh16(query->length);

	mem->table = ax100_telem;
	mem->count = AX100_TELEM_COUNT;

	if (query->action == RPARAM_REPLY) {
		param_deserialize(mem, query->packed, query->length, F_FROM_BIG_ENDIAN);
	}

	return 0;

}
