/** 
 * @file circular_buffer.h
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-07-22
 * @brief Declarations for a circular buffer of uint32_t values
 * 
 * This buffer implementation uses data overwrite after read to detect 
 * write and read index overflows.
 * 
 * @cite P.J. Bones
 */

#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_

#include <stdint.h>
#include <stdbool.h>

#define BUF_EMPTY_VAL INT32_MIN

/**
 * @struct circBuf_t
 *  A structure to store information about a input mode (button or switch)
 *  
 */
typedef struct {
	uint32_t size;		// Number of entries in buffer
	uint32_t windex;	// index for writing, mod(size)
	uint32_t rindex;	// index for reading, mod(size)
	int32_t *data;		// pointer to the data
} circBuf_t;


/** 
 * @brief Initialise the circBuf instance reset both write
 * and read indexes to the start of the buffer return NULL
 * if allocation is invalid
 * 
 * @param buffer the pointer to the circular buffer
 * @param size the size of the buffer max: 2000
 * 
 * @return the pointer to the buffers data array
 */
int32_t *circ_buf_init (circBuf_t *buffer, uint32_t size);


/** 
 * @brief Insert an entry at the current write index location
 * 
 * This function checks for overwriting and will not write if
 * data loss will occur.
 * @param buffer the circular buffer to write to
 * @param entry the entry to write to if this is INT32_MIN it will
 * be replaced with INT32_MIN - 1
 * 
 * @return false if buffer is full
 */
bool circ_buf_write (circBuf_t *buffer, int32_t entry);


/** 
 * @brief Read from the circular buffer at the current write index
 * 
 * This function checks for write index overtake and will return
 * BUF_EMPTY_VAL when this occurs.
 * 
 * @param buffer the buffer to read from
 * 
 * @return the entry at the write index
 */
int32_t circ_buf_read (circBuf_t *buffer);


/** 
 * @brief Releases the memory allocated to the buffer data,
 * sets pointer to NULL and other fields to 0. The buffer can
 * re initialised by another call to init_circ_buf.
 * 
 * @param buffer the buffer to write to
 * 
 */
void circ_buf_free (circBuf_t *buffer);

#endif /*CIRCULAR_BUFFER_H_*/
