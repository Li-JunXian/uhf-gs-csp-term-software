#ifndef PARAM_RPARAM_INTERNAL_H_
#define PARAM_RPARAM_INTERNAL_H_

#include <inttypes.h>
#include <param/param_types.h>

void cmd_rparam_setup(void);
int rparam_get_single(void * out, uint16_t addr, param_type_t type, int size, int mem_id, int node, int port, int timeout);
int rparam_set_single(void * in, uint16_t addr, param_type_t type, int size, int mem_id, int node, int port, int timeout);
int rparam_command(uint8_t node, uint8_t port, uint32_t timeout, uint8_t action, uint8_t from, uint8_t to);
int rparam_query_is_set();


#endif /* PARAM_RPARAM_INTERNAL_H_ */
