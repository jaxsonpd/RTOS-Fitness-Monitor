/** 
 * @file ADC_read.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-10
 * @brief Updated ADC_read source file abstracted from hardware.
 */

#include <stdint.h>
#include <stdbool.h>

#include "adc_hal.h"
#include "circular_buffer_T.h"
#include "adc_comms.h"
#include "FreeRTOS.h"
#include "task.h"

//*****************************************************************************
// Constants
//*****************************************************************************
#define ADC_BUF_SIZE 10
#define ADC_ID ADC_ID_0


#define STEP_GOAL_ROUNDING 100
#define POT_SCALE_COEFF 20000 / 4095 


//*****************************************************************************
// Global variables
//*****************************************************************************
static CircBuf_t ADC_inBuffer; // Buffer of size BUF_SIZE integers (sample values)

void callbackADC(uint32_t adc_value)
{
    CircBuf_write(&ADC_inBuffer, adc_value);
}

// Trigger ADC read
void pollADC(void)
{
    adc_hal_conversion(ADC_ID);
}

// Initialise ADC buffer and ADC peripheral
void initADC(void)
{
    CircBuf_init(&ADC_inBuffer, ADC_BUF_SIZE);
    adc_hal_register(ADC_ID, callbackADC);
}

uint32_t readADC()
{
    int32_t value;
    uint32_t sum = 0;
    uint32_t i = 0;

    // Read the current data from the buffer
    while ((value = CircBuf_read(&ADC_inBuffer)) != BUF_EMPTY_VAL &&
           i < ADC_BUF_SIZE)
    {

        sum += value;
        i++;
    }

    return sum / i;
}

bool adc_read_init(void) 
{
    // Initialize the ADC hardware or API
    bool result = true;
    // result = result && initADC();
    result = result && adc_comms_init(); // Initialize ADC communication mechanism
    return result;
}

void adc_read_thread(void* args) 
{
    if(adc_read_init() == false) {
        while(1);
    }

    uint32_t adc_value = 0;
    for (;;) {
        pollADC(); // Poll the ADC hardware or API
        adc_value = readADC(); // Read the ADC value

        if(adc_value != 0) {
            adc_set(adc_value); // Store the ADC value using comms
        }

        vTaskDelay(5); // Poll at specified rate
    }
}