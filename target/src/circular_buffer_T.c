/** 
 * @file circular_buffer_T.c
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-07-22
 * @brief Support for a circular buffer of int32_t values
 * 
 * @cite P.J. Bones UCECE
 */

#include <stdint.h>
#include "stdlib.h"
#include "circular_buffer_T.h"

int32_t *CircBuf_init (CircBuf_t *buffer, uint32_t size) {
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
	return buffer->data;
}


void CircBuf_write (CircBuf_t *buffer, int32_t entry) {
	buffer->data[buffer->windex] = entry;
	buffer->windex++;
	if (buffer->windex >= buffer->size)
	   buffer->windex = 0;
}


int32_t CircBuf_read (CircBuf_t *buffer) {
	int32_t entry;
	
	entry = buffer->data[buffer->rindex];
	buffer->rindex++;
	if (buffer->rindex >= buffer->size)
	   buffer->rindex = 0;
    return entry;
}

void CircBuf_free (CircBuf_t * buffer) {
	buffer->windex = 0;
	buffer->rindex = 0;
	buffer->size = 0;
	free (buffer->data);
	buffer->data = NULL;
}

