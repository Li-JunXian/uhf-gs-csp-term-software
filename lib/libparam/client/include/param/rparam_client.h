/**
 * GomSpace Parameter System
 *
 * Copyright 2014 GomSpace ApS. All rights reserved.
 */

#ifndef RPARAM_CLIENT_H_
#define RPARAM_CLIENT_H_

#include <param/rparam_internal.h>

/******************************************************
 **                     Utils
 ******************************************************/

#define RPARAM_QUIET 				1
#define RPARAM_NOT_QUIET 			0


/**
 * Download all values of the tables in a table index
 * @param index pointer to table index for which we want to download values
 * @param node CSP address of the node to query
 * @param port CSP port of the parameter server
 * @param timeout timeout on remote CSP calls
 * @returns 0 if table download succeeded, <0 otherwise
 */
int rparam_get_full_table(param_index_t* index, int node, int port, int index_id, int timeout);

/**
 * Set whether rparam API should print to stdout or not
 * @param quiet RPARAM_QUIET or RPARAM_NOT_QUIET
 */
void rparam_set_quiet(uint8_t quiet);

/**
 * Download a table specification from a remote node, store it in memory and save it to local file system
 * @param fname name of the file to store the table specification in
 * @param node CSP address of the node to query
 * @param port CSP port of the parameter server
 * @param index pointer table index memory to store the specification in
 * @param checksum pointer to memory to store the table specification checksum in
 * @returns CMD_ERROR_NONE on success, CMD_ERROR_FAIL otherwise
 */
int rparam_download_table_spec_from_remote_and_save_to_file(const char* fname, uint8_t node, uint8_t port, param_index_t* index, uint16_t* checksum);

/**
 * Download a table specification from a remote node, store it in memory and save it to local file system
 * @param fname name of the file to store the table specification in
 * @param node CSP address of the node to query
 * @param port CSP port of the parameter server
 * @param index pointer table index memory to store the specification in
 * @param index of the remove table to fetch
 * @param checksum pointer to memory to store the table specification checksum in
 * @returns CMD_ERROR_NONE on success, CMD_ERROR_FAIL otherwise
 */
int rparam_download_table_spec_from_remote_and_save_to_file2(const char* fname, uint8_t rparam_node, uint8_t rparam_port, param_index_t* index, uint8_t remote_table_id, uint16_t* checksum);

/**
 * Load a table specification from a local file and store it in memory
 * @param fname name of the file to load table specification from
 * @param index pointer to table index memory to store the specification in
 * @param checksum pointer to memory to store the table specification checksum in
 * @returns CMD_ERROR_NONE on success, CMD_ERROR_INVALID or CMD_ERROR_NOMEM otherwise
 */
int rparam_load_table_spec_from_file(const char* fname, param_index_t* index, uint16_t* checksum);


/******************************************************
 **           Memory and file manipulation
 ******************************************************/

/**
 * Copy from one memory area to another
 * @param node CSP address of the node to query
 * @param port CSP port of the parameter server
 * @param timeout timeout on remote CSP calls
 * @param from memory area to copy from
 * @param to memory area to copy to
 * @returns 1 on success, 0 otherwise
 */
int rparam_copy(uint8_t node, uint8_t port, uint32_t timeout, uint8_t from, uint8_t to);

/**
 * Save contents of a memory area to persistent storage
 * @param node CSP address of the node to query
 * @param port CSP port of the parameter server
 * @param timeout timeout on remote CSP calls
 * @param from memory area to save
 * @param to file id to save to
 * @returns 1 on success, 0 otherwise
 */
int rparam_save(uint8_t node, uint8_t port, uint32_t timeout, uint8_t from, uint8_t to);

/**
 * Load contents from persistent storage to memory
 * @param node CSP address of the node to query
 * @param port CSP port of the parameter server
 * @param timeout timeout on remote CSP calls
 * @param from file id to load from
 * @param to memory area to load to
 * @returns 1 on success, 0 otherwise
 */
int rparam_load(uint8_t node, uint8_t port, uint32_t timeout, uint8_t from, uint8_t to);

/**
 * Clear a file on persistent storage
 * @param node CSP address of the node to query
 * @param port CSP port of the parameter server
 * @param timeout timeout on remote CSP calls
 * @param id file id to clear
 * @returns 1 on success, 0 otherwise
 */
int rparam_clear(uint8_t node, uint8_t port, uint32_t timeout, uint8_t id);

/******************************************************
 **                Query interface
 ******************************************************/

/**
 * Add a 'get' query to the current query, after a succesfull rparam_query_send()
 * the parameter value can be read using rparam_queury_get_value()
 * @param index pointer to the local table index index
 * @param param_name name of the parameter to get
 * @returns CMD_ERROR_NONE on succces, CMD_ERROR_FAIL or CMD_ERROR_INVALID otherwise
 * @see rparam_query_get_value(), rparam_query_send()
 */
int rparam_query_get(param_index_t* index, char* param_name);

/**
 * Get a pointer to a queried value.
 * @param index pointer to the local table index
 * @param param_name name of the parameter
 * @param param_no parameter number, use 0 for non-array values
 * @returns NULL on error, a void* to the queried value otherwise
 */
int rparam_query_get_value(param_index_t* index, char* param_name, uint16_t param_no, void* val_p, uint16_t val_size);

/**
 * Add a 'set' query to the current query. Use rparam_query_send() to execute the set query.
 * @param index pointer to the local table index
 * @param param_name name of the parameter to set
 * @param values array of values to set, multiple values can be set for array type parameters
 * @value_count number of elements in values
 * @returns CMD_ERROR_NONE on succces, CMD_ERROR_FAIL or CMD_ERROR_INVALID otherwise
 * @see rparam_query_send()
 */
int rparam_query_set(param_index_t* index, char* param_name, char* values[], uint8_t value_count);

/**
 * Send the current query
 * @param index pointer to the local table index
 * @param node CSP address of the node to query
 * @param port CSP port of the parameter server
 * @checksum parameter table checksum
 * @returns CMD_ERROR_NONE on success, CMD_ERROR_FAIL otherwise
 */
int rparam_query_send(param_index_t* index, uint8_t node, uint8_t port, uint16_t checksum);

/**
 * Reset the current query
 */
void rparam_query_reset();

/**
 * Dump the current query to stdout
 * @param index pointer to local table index
 */
void rparam_query_print(param_index_t* index);



/******************************************************
 **              Getters and Setters
 ******************************************************/

/**
 * Remote getters for parameters.
 * @param out pointer to the output buffer/parameter
 * @param outlen length of the supplied out-buffer, only applies to rparam_get_string()
 * @param addr local address of the parameter
 * @param index_id id of the index in which the parameter table is located
 * @param node CSP address of the node to query
 * @param port CSP port of the parameter server
 * @param timeout timeout on remote CSP calls
 * @returns <0 on error in which case the contents of out is invalid, >0 on success
 */
static inline int rparam_get_string(char *out, int outlen, uint16_t addr, int index_id, int node, int port, int timeout) {
	return rparam_get_single(out, addr, PARAM_STRING, outlen, index_id, node, port, timeout);
}

/**
 * Remote setters for parameters.
 * @param in pointer to the buffer/parameter value to set
 * @param inlen length of the supplied in-buffer, only applies to rparam_set_string()
 * @param addr local address of the parameter
 * @param index_id id of the index in which the parameter table is located
 * @param node CSP address of the node to query
 * @param port CSP port of the parameter server
 * @param timeout timeout on remote CSP calls
 * @returns <0 on error in which case the contents of out is invalid, >0 on success
 */
static inline int rparam_set_string(char *in, int inlen, uint16_t addr, int index_id, int node, int port, int timeout) {
        return rparam_set_single(in, addr, PARAM_STRING, inlen, index_id, node, port, timeout);
}
static inline int rparam_get_uint8(uint8_t * out, uint16_t addr, int index_id, int node, int port, int timeout) {
        return rparam_get_single(out, addr, PARAM_UINT8, sizeof(uint8_t), index_id, node, port, timeout);
}
static inline int rparam_set_uint8(uint8_t * in, uint16_t addr, int index_id, int node, int port, int timeout) {
        return rparam_set_single(in, addr, PARAM_UINT8, sizeof(uint8_t), index_id, node, port, timeout);
}
static inline int rparam_get_uint16(uint16_t * out, uint16_t addr, int index_id, int node, int port, int timeout) {
        return rparam_get_single(out, addr, PARAM_UINT16, sizeof(uint16_t), index_id, node, port, timeout);
}
static inline int rparam_set_uint16(uint16_t * in, uint16_t addr, int index_id, int node, int port, int timeout) {
        return rparam_set_single(in, addr, PARAM_UINT16, sizeof(uint16_t), index_id, node, port, timeout);
}
static inline int rparam_get_uint32(uint32_t * out, uint16_t addr, int index_id, int node, int port, int timeout) {
        return rparam_get_single(out, addr, PARAM_UINT32, sizeof(uint32_t), index_id, node, port, timeout);
}
static inline int rparam_set_uint32(uint32_t * in, uint16_t addr, int index_id, int node, int port, int timeout) {
        return rparam_set_single(in, addr, PARAM_UINT32, sizeof(uint32_t), index_id, node, port, timeout);
}
static inline int rparam_get_uint64(uint64_t * out, uint16_t addr, int index_id, int node, int port, int timeout) {
        return rparam_get_single(out, addr, PARAM_UINT64, sizeof(uint64_t), index_id, node, port, timeout);
}
static inline int rparam_set_uint64(uint64_t * in, uint16_t addr, int index_id, int node, int port, int timeout) {
        return rparam_set_single(in, addr, PARAM_UINT64, sizeof(uint64_t), index_id, node, port, timeout);
}
static inline int rparam_get_int8(int8_t * out, uint16_t addr, int index_id, int node, int port, int timeout) {
        return rparam_get_single(out, addr, PARAM_INT8, sizeof(int8_t), index_id, node, port, timeout);
}
static inline int rparam_set_int8(int8_t * in, uint16_t addr, int index_id, int node, int port, int timeout) {
        return rparam_set_single(in, addr, PARAM_INT8, sizeof(int8_t), index_id, node, port, timeout);
}
static inline int rparam_get_int16(int16_t * out, uint16_t addr, int index_id, int node, int port, int timeout) {
        return rparam_get_single(out, addr, PARAM_INT16, sizeof(int16_t), index_id, node, port, timeout);
}
static inline int rparam_set_int16(int16_t * in, uint16_t addr, int index_id, int node, int port, int timeout) {
        return rparam_set_single(in, addr, PARAM_INT16, sizeof(int16_t), index_id, node, port, timeout);
}
static inline int rparam_get_int32(int32_t * out, uint16_t addr, int index_id, int node, int port, int timeout) {
        return rparam_get_single(out, addr, PARAM_INT32, sizeof(int32_t), index_id, node, port, timeout);
}
static inline int rparam_set_int32(int32_t * in, uint16_t addr, int index_id, int node, int port, int timeout) {
        return rparam_set_single(in, addr, PARAM_INT32, sizeof(int32_t), index_id, node, port, timeout);
}
static inline int rparam_get_int64(int64_t * out, uint16_t addr, int index_id, int node, int port, int timeout) {
        return rparam_get_single(out, addr, PARAM_INT64, sizeof(int64_t), index_id, node, port, timeout);
}
static inline int rparam_set_int64(int64_t * in, uint16_t addr, int index_id, int node, int port, int timeout) {
        return rparam_set_single(in, addr, PARAM_INT64, sizeof(int64_t), index_id, node, port, timeout);
}
static inline int rparam_get_float(float * out, uint16_t addr, int index_id, int node, int port, int timeout) {
        return rparam_get_single(out, addr, PARAM_FLOAT, sizeof(float), index_id, node, port, timeout);
}
static inline int rparam_set_float(float * in, uint16_t addr, int index_id, int node, int port, int timeout) {
        return rparam_set_single(in, addr, PARAM_FLOAT, sizeof(float), index_id, node, port, timeout);
}
static inline int rparam_get_double(double * out, uint16_t addr, int index_id, int node, int port, int timeout) {
        return rparam_get_single(out, addr, PARAM_DOUBLE, sizeof(double), index_id, node, port, timeout);
}
static inline int rparam_set_double(double * in, uint16_t addr, int index_id, int node, int port, int timeout) {
        return rparam_set_single(in, addr, PARAM_DOUBLE, sizeof(double), index_id, node, port, timeout);
}

#endif /* RPARAM_CLIENT_H_ */
