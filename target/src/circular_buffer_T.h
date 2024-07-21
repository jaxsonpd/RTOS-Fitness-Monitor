/** 
 * @file circular_buffer_T.h
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-07-22
 * @brief Declarations for a circular buffer of uint32_t values
 * 
 * @cite P.J. Bones
 */

#ifndef circular_buffer_T_H_
#define circular_buffer_T_H_

#include <stdint.h>

// *******************************************************
// Buffer structure
typedef struct {
	uint32_t size;		// Number of entries in buffer
	uint32_t windex;	// index for writing, mod(size)
	uint32_t rindex;	// index for reading, mod(size)
	uint32_t *data;		// pointer to the data
} CircBuf_t;


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
int32_t *CircBuf_init (CircBuf_t *buffer, uint32_t size);


/** 
 * @brief Insert an entry at the current write index location
 * 
 * This does not check for overwriting but should
 * @param buffer the circular buffer to write to
 * @param entry the entry to write to
 * 
 */
void CircBuf_write (CircBuf_t *buffer, int32_t entry);


/** 
 * @brief Read from the circular buffer at the current write index
 * 
 * This function does not check for rindex overtake but should
 * @param buffer the buffer to read from
 * 
 * @return the entry at the write index
 */
int32_t CircBuf_read (CircBuf_t *buffer);


/** 
 * @brief Releases the memory allocated to the buffer data,
 * sets pointer to NULL and other fields to 0. The buffer can
 * re initialised by another call to initCircBuf().
 * 
 * @param buffer the buffer to write to
 * 
 */
void CircBuf_free (CircBuf_t *buffer);

#endif /*circular_buffer_T_H_*/
