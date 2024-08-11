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

// void updateNewGoal(void) {
//     deviceStateInfo_t* deviceState = getDeviceState();

//     int32_t adc_reading = readADC();
//     if (adc_reading != 0) {
//         deviceState->newGoal = adc_reading * POT_SCALE_COEFF;
//         deviceState->newGoal = (deviceState->stateGoal / STEP_GOAL_ROUNDING) * STEP_GOAL_ROUNDING;
//     }

//     if (deviceState->newGoal == 0) {
//         deviceState->newGoal = STEP_GOAL_ROUNDING;
//     }
// }