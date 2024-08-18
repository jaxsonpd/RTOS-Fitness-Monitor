/**
 * @file pot_manager.c
 * @author Daniel Hawes (dha144@uclive.ac.nz)
 * @date 2024-08-12
 * @brief Implementation for the potentiometer manager
 */

#include "pot_manager.h"

#include "adc_manager.h"
#include "comms/pot_comms.h"  

#include "FreeRTOS.h"
#include "task.h"

bool pot_read_init(void) {
    bool result = true;
    result = pot_comms_init() && result; // Initialize ADC communication mechanism
    return result;
}

void pot_read_thread(void* args) {
    if (pot_read_init() == false) {
        while (1);
    }

    initADC();

    uint32_t pot_value = 0;
    for (;;) {
        pollADC(); 
        pot_value = readADC(); 

        if (pot_value != 0) {
            pot_set(pot_value); 
        }

        vTaskDelay(20); // Poll at specified rate
    }
}