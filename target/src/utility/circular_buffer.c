/** 
 * @file circular_buffer.c
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-07-22
 * @brief Support for a circular buffer of int32_t values
 * 
 * @cite P.J. Bones UCECE
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "stdlib.h"
#include "circular_buffer.h"

int32_t *circ_buf_init (circBuf_t *buffer, uint32_t size) {
	if (size == 0) {
		return NULL;
	} else if (size > 2000) {
		return NULL;
	}
	
	buffer->windex = 0;
	buffer->rindex = 0;
	buffer->size = size;
	buffer->data = 
        (int32_t *) calloc (size, sizeof(int32_t));

	for (int i = 0; i < buffer->size; i++) {
		buffer->data[i] = BUF_EMPTY_VAL;
	}
	return buffer->data;
}


bool circ_buf_write (circBuf_t *buffer, int32_t entry) {
	if (buffer->data[buffer->windex] != BUF_EMPTY_VAL) {
		// Don't over write buffer
		return false;
	}
	if (entry == BUF_EMPTY_VAL) {
		entry = entry + 1; 
	}

	buffer->data[buffer->windex] = entry;
	buffer->windex++;
	if (buffer->windex >= buffer->size)
	   buffer->windex = 0;

	return true;
}


int32_t circ_buf_read (circBuf_t *buffer) {
	int32_t entry;
	
	if (buffer->data[buffer->rindex] == BUF_EMPTY_VAL && buffer->rindex == buffer->windex) {
		return BUF_EMPTY_VAL;
		printf("Read Failed\n");
	}

	entry = buffer->data[buffer->rindex];
	buffer->data[buffer->rindex] = BUF_EMPTY_VAL;
	buffer->rindex++;
	
	if (buffer->rindex >= buffer->size)
	   buffer->rindex = 0;
    
	return entry;
}

void circ_buf_free (circBuf_t * buffer) {
	buffer->windex = 0;
	buffer->rindex = 0;
	buffer->size = 0;
	free(buffer->data);
	buffer->data = NULL;
}