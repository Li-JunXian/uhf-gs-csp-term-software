/**
 * GomSpace Parameter System
 *
 * @author Johan De Claville Christiansen
 * Copyright 2014 GomSpace ApS. All rights reserved.
 */

#ifndef PARAM_STORE_H_
#define PARAM_STORE_H_

/**
 * Storage backend prototypes
 */
int param_save(param_index_t * from, uint8_t to_file_id);
int param_load(uint8_t from_file_id, param_index_t * to);
void param_clear(uint8_t file_id);
void param_write_persist(uint16_t addr, uint16_t framaddr, size_t size, void * item, uint8_t flags);
void param_init_persist(param_index_t * mem);

int param_save_file(const param_table_t table[], size_t table_count, param_mem from, char * to);
int param_load_file(const param_table_t table[], size_t table_count, char * from, param_mem to);


/*
 * If the standard layout of 32x1024B files does not match your task it is possible to setup a custom file layout. The custom layout should
 * be implemented in a function with this signature.

 * @param physaddr pointer to a long that will be set to a physical address in the storage medium
 * @param index pointer to the index that is to be written out to storage
 * @param file_id id of the file to be written
 * @return must return 0 on OK, >0 otherwise.
 */
typedef uint8_t (*param_custom_file_mapper)(unsigned long* physaddr, param_index_t* index, uint8_t file_id);

/*
 * Set a custom function for mapping files in the storage medium, currently only implemented for
 * the param_fram.c backend.
 *
 * The custom function must set physaddr to an address where the index will be written, it must also check
 * index->size to make sure the index fits
 */
void param_store_set_custom_file_mapper(param_custom_file_mapper mapping_function);

/**
 * Load parameters with fallback function
 * @param mem pointer to memory to load
 * @param fno first try this file
 * @param fno_dfl then try this file
 * @param fallback finally call fallback
 * @param name pointer to config name
 * @returns param_config_id 0 = fallback, 1 = default, 2 = stored
 */
param_config_id param_load_fallback(param_index_t * mem, uint8_t fno, uint8_t fno_dfl, void (*fallback)(void), char * name);

#endif /* PARAM_STORE_H_ */
