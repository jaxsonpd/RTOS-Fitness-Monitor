#include "math.h"
#include "potentiometer_read.h"
#include "potentiometer_comms.h"
#include "ADC_read.h"
#include "circular_buffer_T.h"

#include "FreeRTOS.h"
#include "task.h"

//*****************************************************************************
// Constants
//*****************************************************************************
#define POTENTIOMETER_BUF_SIZE 10
#define POTENTIOMETER_ID ADC_ID_0

#define STEP_GOAL_ROUNDING 100
#define POT_SCALE_COEFF 20000 / 4095

//*****************************************************************************
// Global variables
//*****************************************************************************
static CircBuf_t potentiometerBuffer; // Buffer of size BUF_SIZE integers (sample values)

void callbackPotentiometer(uint32_t adc_value)
{
    CircBuf_write(&potentiometerBuffer, adc_value);
}

void initPotentiometer(void) // formerly initADC
{
    CircBuf_init(&potentiometerBuffer, POTENTIOMETER_BUF_SIZE);
    adc_hal_register(POTENTIOMETER_ID, callbackPotentiometer); // updated names
}

void pollPotentiometer(void) // formerly pollADC
{
    adc_hal_conversion(POTENTIOMETER_ID); // using the updated potentiometer ID
}

uint32_t readPotentiometer(void) // formerly readADC
{
    int32_t value;
    uint32_t sum = 0;
    uint32_t i = 0;

    // Reading and averaging logic remains the same
    while ((value = CircBuf_read(&potentiometerBuffer)) != BUF_EMPTY_VAL && i < POTENTIOMETER_BUF_SIZE) {
        sum += value;
        i++;
    }

    return sum / i;
}

bool potentiometer_read_init(void) {
    bool result = true;
    result = result && initADC();
    result = result && potentiometer_comms_init();
}

void potentiometer_read_thread(void *args) 
{
    if (potentiometer_read_init() == false) {
        while(1);
    }

    uint32_t potentiometerValue = 0;
    for (;;) { 
        pollPotentiometer(); //pollADC();
        potentiometerValue = readPotentiometer(); //readADC()
        
        if (potentiometerValue != 0) {
            potentiometer_set(potentiometerValue);
        }

        vTaskDelay(5);
    }
}