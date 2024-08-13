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
#define BUF_SIZE 20 // WARNING: If this is set too high, we run out of heap space and the z-buffer gets garbled data

static CircBuf_t acclXBuffer;
static CircBuf_t acclYBuffer;
static CircBuf_t acclZBuffer;


/*******************************************
 *      Global functions
 *******************************************/
// Init the library
bool accl_init(void)
{
    accl_chip_init(); // Init the chip over I2C

    CircBuf_init(&acclXBuffer, BUF_SIZE);
    CircBuf_init(&acclYBuffer, BUF_SIZE);
    CircBuf_init(&acclZBuffer, BUF_SIZE);
    return true;
}

// Run periodically to store acceleration to the circular buffer
void accl_poll(void)
{
    int16_t acceleration[3];
    accl_data_get(acceleration);
    CircBuf_write(&acclXBuffer, acceleration[0]);
    CircBuf_write(&acclYBuffer, acceleration[1]);
    CircBuf_write(&acclZBuffer, acceleration[2]);
}

// Return the mean acceleration stored within the circular buffers
vector3_t accl_mean(void)
{
    // Sum with 32-bit ints to prevent overflow, then dividing the total sum for better accuracy
    int32_t result_x = 0;
    int32_t result_y = 0;
    int32_t result_z = 0;

    int32_t x_new = 0;
    int32_t y_new = 0;
    int32_t z_new = 0;

    uint8_t x_divisor = 0;
    uint8_t y_divisor = 0;
    uint8_t z_divisor = 0;

    uint8_t i = 0;
    for (i = 0; i < BUF_SIZE; i++) {
        x_new =  CircBuf_read(&acclXBuffer);
        if (x_new != BUF_EMPTY_VAL) {
            result_x = result_x + x_new;
            x_divisor++;
        }

        y_new = CircBuf_read(&acclYBuffer);
        if (y_new != BUF_EMPTY_VAL) {
            result_y = result_y + y_new;
            y_divisor++;
        }

        z_new = CircBuf_read(&acclZBuffer);
        if (z_new != BUF_EMPTY_VAL) {
            result_z = result_z + z_new;
            z_divisor++;
        }
    }
    int16_t acc_x = result_x / x_divisor;
    int16_t acc_y = result_y / y_divisor;
    int16_t acc_z = result_z / z_divisor;

    vector3_t result = v3_constructor(acc_x, acc_y, acc_z);

    return result;
}