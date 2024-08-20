/** 
 * @file adc_handler.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief handles adc interactions to get an averaged reading
 */

#include <stdint.h>
#include <stdbool.h>

#include "hal/adc_hal.h"
#include "utility/circular_buffer.h"
#include "adc_handler.h"

#define ADC_BUF_SIZE 10
#define ADC_ID ADC_ID_0

static circBuf_t ADC_inBuffer;

void adc_callback(uint32_t adc_value) {
    circ_buf_write(&ADC_inBuffer, adc_value);
}

void adc_poll(void) {
    adc_hal_conversion(ADC_ID);
}

void adc_init(void) {
    circ_buf_init(&ADC_inBuffer, ADC_BUF_SIZE);
    adc_hal_register(ADC_ID, adc_callback);
}

uint32_t adc_read() {
    int32_t value;
    uint32_t sum = 0;
    uint32_t i = 0;

    while ((value = circ_buf_read(&ADC_inBuffer)) != BUF_EMPTY_VAL && i < ADC_BUF_SIZE)
    {
        sum += value;
        i++;
    }
    return sum / i;
}
