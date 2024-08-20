/**
 * @file fitness_monitor_main.c
 * @author Isaac Cone, Jack Duignan, Daniel Hawes
 * @brief ENCE464 fitness monitor project based on 
 * ENCE361 step counter.
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "hal/clock_hal.h"

#include "step_counter_manager.h"
#include "input_manager.h"
#include "pot_manager.h"
#include "device_manager.h"

void vAssertCalled(const char* pcFile, unsigned long ulLine) {
    (void)pcFile; // unused
    (void)ulLine; // unused
    while (true)
        ;
}

int main(void) {
    // Init libs
    init_clock();

    xTaskCreate(&step_counter_thread, "step counter thread", 128, NULL, 4, NULL);
    xTaskCreate(&input_manager_thread, "input manager thread", 64, NULL, 3, NULL);
    xTaskCreate(&device_manager_thread, "device manager thread", 512, NULL, 2, NULL);
    xTaskCreate(&pot_read_thread, "pot read thread", 64, NULL, 3, NULL);
    vTaskStartScheduler();
    return 0;
}