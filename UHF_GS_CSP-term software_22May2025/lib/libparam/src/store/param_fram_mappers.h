/**
 * GomSpace Parameter System
 *
 * @author Søren Nøhr Christensen
 * Copyright 2014 GomSpace ApS. All rights reserved.
 */

#ifndef SRC_STORE_PARAM_FRAM_MAPPERS_H_
#define SRC_STORE_PARAM_FRAM_MAPPERS_H_


uint8_t param_fram_custom_mapper_three_file_sizes(unsigned long* physaddr, param_index_t* index, uint8_t file_id) {

/* File_id:                       Address:
		  0	|					| 0x0
			|					|
			|  8 x 1024B files  |
			|					|
		  7	|					| 0x1C00
		  8	|					| 0x2000
			|					|
			|  8 x 512B files	|
			|					|
		 15 |					| 0x2E00
		 16	|					| 0x3000
			|					|
			|					|
			|  48 x 256B files  |
			|					|
		 63 |					| 0x5F00
		    |                   | 0x6000
			|					|
			| Reserved for 		|
			| tables with		|
			| persistent params	|
			|					| 0x8000
			*/


	/* sanity checks */
	if (file_id > 63)
		return 1;
	if (index == NULL)
		return 2;

	/* handle 1024B files */
	if (file_id <= 7) {
		if (index->size > 1024)
			return 3;

		*physaddr = file_id * 1024;
	}

	/* handle 512B files */
	if (file_id > 7 && file_id <= 15) {
		if (index->size > 512)
			return 4;

		*physaddr = 0x2000 + ((file_id - 8) * 512);
	}

	/* handle 256B files */
	if (file_id > 15 && file_id <= 63) {
		if (index->size > 256)
			return 5;

		*physaddr = 0x3000 + ((file_id - 16) * 256);
	}


	return 0;
}




#endif /* SRC_STORE_PARAM_FRAM_MAPPERS_H_ */
