/**
 * @file fitness_monitor_main.c
 * @author Isaac Cone, Jack Duignan, Daniel Hawes
 * @brief ENCE464 fitness monitor project based on 
 * ENCE361 step counter.
 */
// #define SERIAL_PLOTTING_ENABLED

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "serial_sender.h"

#include "hal/clock_hal.h"

#include "step_counter_manager.h"
#include "input_manager.h"
#include "pot_manager.h"
#include "display_manager.h"

/***********************************************************
 * Helper functions
 ***********************************************************/
// Read the current systick value, without mangling the data
unsigned long readCurrentTick(void) {
    return xTaskGetTickCount();
}

void vAssertCalled(const char* pcFile, unsigned long ulLine) {
    (void)pcFile; // unused
    (void)ulLine; // unused
    while (true)
        ;
}

int main(void) {
    // Init libs
    init_clock();
    #ifdef SERIAL_PLOTTING_ENABLED
    SerialInit ();
    #endif // SERIAL_PLOTTING_ENABLED
    xTaskCreate(&step_counter_thread, "step counter thread", 512, NULL, 1, NULL);
    xTaskCreate(&input_manager_thread, "input manager thread", 128, NULL, 1, NULL);
    xTaskCreate(&display_manager_thread, "display manager thread", 512, NULL, 1, NULL);
    xTaskCreate(&pot_read_thread, "pot read thread", 128, NULL, 1, NULL);
    vTaskStartScheduler();
    return 0;
}