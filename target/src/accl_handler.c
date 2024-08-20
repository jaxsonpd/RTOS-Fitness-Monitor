/** 
 * @file accl_handler.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief handles accelerometer interactions to get averaged readings
 */

#include <stdint.h>
#include <stdbool.h>

#include "assert.h"
#include "utility/circular_buffer.h"
#include "hal/accl_hal.h"
#include "accl_handler.h"

#define BUF_SIZE 20

static CircBuf_t acclXBuffer;
static CircBuf_t acclYBuffer;
static CircBuf_t acclZBuffer;

bool accl_init(void) {
    accl_chip_init();

    CircBuf_init(&acclXBuffer, BUF_SIZE);
    CircBuf_init(&acclYBuffer, BUF_SIZE);
    CircBuf_init(&acclZBuffer, BUF_SIZE);
    return true;
}

void accl_poll(void) {
    int16_t acceleration[3];
    accl_data_get(acceleration);
    CircBuf_write(&acclXBuffer, acceleration[0]);
    CircBuf_write(&acclYBuffer, acceleration[1]);
    CircBuf_write(&acclZBuffer, acceleration[2]);
}

vector3_t accl_mean(void) {
    int32_t result_x = 0;
    int32_t result_y = 0;
    int32_t result_z = 0;

    uint8_t x_divisor = 0;
    uint8_t y_divisor = 0;
    uint8_t z_divisor = 0;

    int32_t reading = 0;
    for (uint8_t i = 0; i < BUF_SIZE; i++) {
        reading =  CircBuf_read(&acclXBuffer);

        if (reading != BUF_EMPTY_VAL) {
            result_x = result_x + reading;
            x_divisor++;
        }

        reading = CircBuf_read(&acclYBuffer);
        if (reading != BUF_EMPTY_VAL) {
            result_y = result_y + reading;
            y_divisor++;
        }

        reading = CircBuf_read(&acclZBuffer);
        if (reading != BUF_EMPTY_VAL) {
            result_z = result_z + reading;
            z_divisor++;
        }
    }

    assert(x_divisor != 0);
    assert(y_divisor != 0);
    assert(z_divisor != 0);

    vector3_t result = v3_constructor(result_x / x_divisor, result_y / y_divisor, result_z / z_divisor);

    return result;
}