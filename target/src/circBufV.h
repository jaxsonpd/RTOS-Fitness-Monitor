#ifndef circular_buffer_T_H_
#define circular_buffer_T_H_

// *******************************************************
// 
// circBufV.h
//
// Support for a circular buffer of vector3 (3*int16_t) values on the
//  Tiva processor.
// P.J. Bones UCECE, Tim Preston-Marshall
// Last modified:  3.5.2022
//
// FitnessThur9-1
// 
// *******************************************************
#include <stdint.h>


typedef struct{
    int16_t x;
    int16_t y;
    int16_t z;
} vector3_t;

// *******************************************************
// Buffer structure
typedef struct {
	uint32_t size;		// Number of entries in buffer
	uint32_t windex;	// index for writing, mod(size)
	uint32_t rindex;	// index for reading, mod(size)
	vector3_t *data;	// pointer to the data
} circBufVec_t;

// *******************************************************
// initCircBuf: Initialise the circBuf instance. Reset both indices to
// the start of the buffer.  Dynamically allocate and clear the the 
// memory and return a pointer for the data.  Return NULL if 
// allocation fails.
vector3_t* initVecCircBuf (circBufVec_t *buffer, uint32_t size);

// *******************************************************
// writeCircBuf: insert entry at the current windex location,
// advance windex, modulo (buffer size).
void writeVecCircBuf (circBufVec_t *buffer, vector3_t entry);

// *******************************************************
// readCircBuf: return entry at the current rindex location,
// advance rindex, modulo (buffer size). The function deos not check
// if reading has advanced ahead of writing.
vector3_t readVecCircBuf (circBufVec_t *buffer);

// *******************************************************
// freeCircBuf: Releases the memory allocated to the buffer data,
// sets pointer to NULL and other fields to 0. The buffer can
// re initialised by another call to initCircBuf().
void freeVecCircBuf (circBufVec_t *buffer);

#endif /*circular_buffer_T_H_*/
