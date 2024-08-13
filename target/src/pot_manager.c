#include "pot_manager.h"

#include "adc_manager.h"
#include "pot_comms.h"  

#include "FreeRTOS.h"
#include "task.h"

bool pot_read_init(void) 
{
    // Initialize the ADC hardware or API
    bool result = true;
    // initADC();
    result = pot_comms_init() && result; // Initialize ADC communication mechanism
    return result;
}

void pot_read_thread(void* args) 
{
    if(pot_read_init() == false) {
        while(1);
    }

    uint32_t pot_value = 0;
    for (;;) {
        pollADC(); // Poll the ADC hardware or API
        pot_value = readADC(); // Read the ADC value

        if(pot_value != 0) {
            pot_set(pot_value); // Store the ADC value using comms
        }

        vTaskDelay(5); // Poll at specified rate
    }
}