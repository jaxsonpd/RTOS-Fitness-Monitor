/**
 * @file fitness_monitor_main.c
 * @author Isaac Cone, Jack Duignan, Daniel Hawes
 * @brief ENCE464 fitness monitor project based on 
 * ENCE361 step counter.
 */

// Comment this out to disable serial plotting
// #define SERIAL_PLOTTING_ENABLED

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/debug.h"
#include "driverlib/pin_map.h"
#include "utils/ustdlib.h"
#include "stdio.h"
#include "stdlib.h"
#include "utils/ustdlib.h"
#include "math.h"
#include "ADC_read.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#ifdef SERIAL_PLOTTING_ENABLED
#include "serial_sender.h"
#endif // SERIAL_PLOTTING_ENABLED

#include "step_counter.h"
#include "step_counter_comms.h"

#include "input_manager.h"
#include "input_comms.h"

#include "display_manager.h"

#include "fitness_monitor_main.h"

/**********************************************************
 * Constants and types
 **********************************************************/
#define RATE_SYSTICK_HZ 1000
#define RATE_IO_HZ 75
#define RATE_ACCL_HZ 200
#define RATE_DISPLAY_UPDATE_HZ 5
#define FLASH_MESSAGE_TIME 3 / 2 // seconds

#ifdef SERIAL_PLOTTING_ENABLED
#define RATE_SERIAL_PLOT_HZ 100
#endif // SERIAL_PLOTTING_ENABLED

#define STEP_GOAL_ROUNDING 100
#define STEP_THRESHOLD_HIGH 270
#define STEP_THRESHOLD_LOW 235

#define TARGET_DISTANCE_DEFAULT 1000

#define POT_SCALE_COEFF 20000 / 4095 // in steps, adjusting to account for the potentiometer's maximum possible reading

/*******************************************
 *      Local prototypes
 *******************************************/
void SysTickIntHandler(void);
void initClock(void);
void initDisplay(void);

/*******************************************
 *      Globals
 *******************************************/
deviceStateInfo_t deviceState; // Stored as one global so it can be accessed by other helper libs within this main module


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

// Flash a message onto the screen, overriding everything else
void flashMessage(char* toShow) {
    deviceState.flashTicksLeft = RATE_DISPLAY_UPDATE_HZ * FLASH_MESSAGE_TIME;

    uint8_t i = 0;
    while (toShow[i] != '\0' && i < MAX_STR_LEN) {
        (deviceState.flashMessage)[i] = toShow[i];

        i++;
    }

    deviceState.flashMessage[i] = '\0';
}

/***********************************************************
 * Main Loop
 ***********************************************************/
void superloop(void* args) {
    unsigned long lastIoProcess = 0;
    unsigned long lastAcclProcess = 0;
    unsigned long lastDisplayProcess = 0;

#ifdef SERIAL_PLOTTING_ENABLED
    unsigned long lastSerialProcess = 0;
#endif // SERIAL_PLOTTING_ENABLED

    uint32_t stepsAccumulated = 0;

    // display_manager_init();
    initADC();

    while (1) {
        unsigned long currentTick = readCurrentTick();

        // Poll the potentiometer
        if (lastIoProcess + RATE_SYSTICK_HZ / RATE_IO_HZ < currentTick) {
            lastIoProcess = currentTick;

            pollADC();

            int32_t adc_reading = readADC();
            if (adc_reading != 0) {
                deviceState.newGoal = adc_reading * POT_SCALE_COEFF; // Set the new goal value, scaling to give the desired range
                deviceState.newGoal = (deviceState.newGoal / STEP_GOAL_ROUNDING) * STEP_GOAL_ROUNDING; // Round to the nearest 100 steps
            }
            
            if (deviceState.newGoal == 0) { // Prevent a goal of zero, instead setting to the minimum goal (this also makes it easier to test the goal-reaching code on a small but non-zero target)
                deviceState.newGoal = STEP_GOAL_ROUNDING;
            }
        }

        // Read and process the accelerometer
        if (lastAcclProcess + RATE_SYSTICK_HZ / RATE_ACCL_HZ < currentTick) {
            lastAcclProcess = currentTick;
            stepsAccumulated = step_counter_get();
            deviceState.stepsTaken = deviceState.stepsTaken + stepsAccumulated;

            // flash a message if the user has reached their goal
            if (deviceState.stepsTaken == deviceState.currentGoal && deviceState.flashTicksLeft == 0) {
                flashMessage("Goal reached!");
            }

            // Don't start the workout until the user begins walking
            if (deviceState.stepsTaken == 0) {
                deviceState.workoutStartTick = currentTick;
            }
        }

        // // Write to the display
        // if (lastDisplayProcess + RATE_SYSTICK_HZ / RATE_DISPLAY_UPDATE_HZ < currentTick) {
        //     lastDisplayProcess = currentTick;

        //     if (deviceState.flashTicksLeft > 0) {
        //         deviceState.flashTicksLeft--;
        //     }

        //     uint16_t secondsElapsed = (currentTick - deviceState.workoutStartTick) / RATE_SYSTICK_HZ;

        //     uint8_t num_tries = 0;
        //     while (input_comms_num_msgs() > 0 && num_tries < 5) {
        //         inputCommMsg_t msg = input_comms_receive();

        //         display_update_state(msg, &deviceState);

        //         num_tries++;
        //     }


        //     display_update(deviceState, secondsElapsed);
        // }

        // Send to USB via serial
        #ifdef SERIAL_PLOTTING_ENABLED
        // if (lastSerialProcess + RATE_SYSTICK_HZ/RATE_SERIAL_PLOT_HZ < currentTick) {
        //     lastSerialProcess = currentTick;

        //     SerialPlot(deviceState.stepsTaken);
        // }
        #endif // SERIAL_PLOTTING_ENABLED



        // Protection in the unlikely case the device is left running for long enough for the system tick counter to overflow
        // This prevents the last process ticks from being 'in the future', which would prevent the update functions from being called,
        // rendering the device inoperable.
        // This would take ~49 days, but is not impossible if the user forgets to turn it off before they put it away (assuming th battery lasts that long)
        if (currentTick < lastIoProcess) {
            lastIoProcess = 0;
        }

        if (currentTick < lastAcclProcess) {
            lastAcclProcess = 0;
        }

        if (currentTick < lastDisplayProcess) {
            lastDisplayProcess = 0;
        }

#ifdef SERIAL_PLOTTING_ENABLED
        if (currentTick < lastSerialProcess) {
            lastSerialProcess = 0;
        }
        #endif // SERIAL_PLOTTING_ENABLED
        vTaskDelay(5);
    }
}

int main(void) {
    // Device state
    // Omnibus struct that holds loads of info about the device's current state, so it can be updated from any function
    deviceState.displayMode = DISPLAY_STEPS;
    deviceState.stepsTaken = 0;
    deviceState.currentGoal = TARGET_DISTANCE_DEFAULT;
    deviceState.debugMode = false;
    deviceState.displayUnits = UNITS_SI;
    deviceState.workoutStartTick = 0;
    deviceState.flashTicksLeft = 0;
    deviceState.flashMessage = calloc(MAX_STR_LEN + 1, sizeof(char));

    // Init libs
    initClock();
    
    #ifdef SERIAL_PLOTTING_ENABLED
    SerialInit ();
    #endif // SERIAL_PLOTTING_ENABLED
    
    xTaskCreate(&superloop, "superloop", 512, NULL, 1, NULL);
    xTaskCreate(&step_counter_thread, "step counter thread", 512, NULL, 1, NULL);
    xTaskCreate(&input_manager_thread, "input manager thread", 128, NULL, 1, NULL);
    xTaskCreate(&display_manager_thread, "display manager thread", 512, NULL, 1, NULL);
    vTaskStartScheduler();
    return 0;
}