/**
 * @file fitness_monitor_main.c
 * @author Isaac Cone, Jack Duignan, Daniel Hawes
 * @brief ENCE464 fitness monitor project based on 
 * ENCE361 step counter.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "driverlib/sysctl.h"


#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "serial_sender.h"

#include "step_counter_manager.h"
#include "input_manager.h"
#include "pot_manager.h"
#include "display_manager.h"


// #define RATE_SYSTICK_HZ 1000
// #define RATE_IO_HZ 75
// #define RATE_ACCL_HZ 200
// #define RATE_DISPLAY_UPDATE_HZ 5
// #define FLASH_MESSAGE_TIME 3 / 2 // seconds

void SysTickIntHandler(void);
void initClock(void);
void initDisplay(void);

/***********************************************************
 * Initialisation functions
 ***********************************************************/

void initClock(void) {
    // Set the clock rate to 20 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
        SYSCTL_XTAL_16MHZ);
}

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
    initClock();
    
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