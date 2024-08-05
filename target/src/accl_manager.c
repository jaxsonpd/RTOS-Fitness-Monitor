/*
 * Acceleration Manager
 * Matt Suter, Tim Preston-Marshall, Daniel Rabbidge
 * ENCE361 2022
 *
 * FitnessThur9-1
 */

#include <stdint.h>
#include <stdbool.h>

#include "circular_buffer_T.h"

#include "accl_hal.h"

#include "accl_manager.h"

//********************************************************
// Constants and static vars
//********************************************************
#define BUF_SIZE 5 // WARNING: If this is set too high, we run out of heap space and the z-buffer gets garbled data

static CircBuf_t acclXBuffer;
static CircBuf_t acclYBuffer;
static CircBuf_t acclZBuffer;

/*******************************************
 *      Global functions
 *******************************************/
// Init the library
void acclInit(void)
{
    accl_chip_init(); // Init the chip over I2C

    CircBuf_init(&acclXBuffer, BUF_SIZE);
    CircBuf_init(&acclYBuffer, BUF_SIZE);
    CircBuf_init(&acclZBuffer, BUF_SIZE);
}

// Run periodically to store acceleration to the circular buffer
void acclProcess(void)
{
    int16_t acceleration[3];
    accl_data_get(acceleration);
    CircBuf_write(&acclXBuffer, acceleration[0]);
    CircBuf_write(&acclYBuffer, acceleration[1]);
    CircBuf_write(&acclZBuffer, acceleration[2]);
}

// Return the mean acceleration stored within the circular buffers
vector3_t acclMean(void)
{
    // Sum with 32-bit ints to prevent overflow, then dividing the total sum for better accuracy
    int32_t result_x = 0;
    int32_t result_y = 0;
    int32_t result_z = 0;

    uint8_t i = 0;
    for (i = 0; i < BUF_SIZE; i++) {
        result_x = result_x + CircBuf_read(&acclXBuffer);
        result_y = result_y + CircBuf_read(&acclYBuffer);
        result_z = result_z + CircBuf_read(&acclZBuffer);
    }

    vector3_t result = {0};
    result.x = result_x / BUF_SIZE;
    result.y = result_y / BUF_SIZE;
    result.z = result_z / BUF_SIZE;

    return result;
}