/**
 * @file input_manager.c
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-08-04
 * @brief Contains the thread that handles input from various IO
 *
 * @cite Daniel Rabbidge
 */

#include <stdint.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "IO_input.h"
#include "input_comms.h"

#include "step_counter_main.h"
#include "input_manager.h"

 //********************************************************
 // Constants and static vars
 //********************************************************
#define LONG_PRESS_CYCLES 20

static uint16_t longPressCount = 0;
static bool allowLongPress = true;

//********************************************************
// Init buttons and switch I/O handlers
//********************************************************
void btnInit(void) {
    input_init(true);
}

//********************************************************
// Run at a fixed rate, modifies the device's state depending on button presses
//********************************************************
void btnUpdateState(deviceStateInfo_t* deviceStateInfo) {
    input_update();

    displayMode_t currentDisplayMode = deviceStateInfo->displayMode;

    // Changing screens
    if (input_check(LEFT_BUTTON) == PUSHED) {
        deviceStateInfo->displayMode = (deviceStateInfo->displayMode + 1) % DISPLAY_NUM_STATES; // flicker when pressing button
    } else if (input_check(RIGHT_BUTTON) == PUSHED) {
        // Can't use mod, as enums behave like an unsigned int, so (0-1)%n != n-1
        if (deviceStateInfo->displayMode > 0) {
            deviceStateInfo->displayMode--;
        } else {
            deviceStateInfo->displayMode = DISPLAY_NUM_STATES - 1;
        }
    }

    // Enable/Disable test mode
    if (input_get(RIGHT_SWITCH)) {
        deviceStateInfo->debugMode = true;
    } else {
        deviceStateInfo->debugMode = false;
    }

    // Usage of UP and DOWN buttons
    if (deviceStateInfo->debugMode) {
        // TEST MODE OPERATION
        if (input_check(UP_BUTTON) == PUSHED) {
            deviceStateInfo->stepsTaken = deviceStateInfo->stepsTaken + DEBUG_STEP_INCREMENT;
        }

        if (input_check(DOWN_BUTTON) == PUSHED) {
            if (deviceStateInfo->stepsTaken >= DEBUG_STEP_DECREMENT) {
                deviceStateInfo->stepsTaken = deviceStateInfo->stepsTaken - DEBUG_STEP_DECREMENT;
            } else {
                deviceStateInfo->stepsTaken = 0;
            }
        }
    } else {
        // NORMAL OPERATION

        // Changing units
        if (input_check(UP_BUTTON) == PUSHED) {
            if (deviceStateInfo->displayUnits == UNITS_SI) {
                deviceStateInfo->displayUnits = UNITS_ALTERNATE;
            } else {
                deviceStateInfo->displayUnits = UNITS_SI;
            }
        }

        // Resetting steps and updating goal with long and short presses
        if ((input_get(DOWN_BUTTON) == true) && (currentDisplayMode != DISPLAY_SET_GOAL) && (allowLongPress)) {
            longPressCount++;
            if (longPressCount >= LONG_PRESS_CYCLES) {
                deviceStateInfo->stepsTaken = 0;
                // flashMessage("Reset!");
            }
        } else {
            if ((currentDisplayMode == DISPLAY_SET_GOAL) && input_check(DOWN_BUTTON) == PUSHED) {
                deviceStateInfo->currentGoal = deviceStateInfo->newGoal;
                deviceStateInfo->displayMode = DISPLAY_STEPS;

                allowLongPress = false; // Hacky solution: Protection against double-registering as a short press then a long press
            }
            longPressCount = 0;
        }

        if (input_check(DOWN_BUTTON) == RELEASED) {
            allowLongPress = true;
        }
    }
}

/**
 * @brief Initialise the input manager thread
 *
 * @return true if successful
 */
bool input_manager_init(void) {
    bool result = true;
    result = input_init(true);
    result = input_comms_init();

    return result;
}

void input_manager_thread(void *rtos_param) {
    BaseType_t xStatus;
    input_state_t input_state;

    input_manager_init();

    if (input_get(RIGHT_SWITCH)) { // Make sure that can have debug enabled on startup
        input_comms_send(MSG_RIGHT_SWITCH_ON);
    }

    for (;;) {
        input_update();

        // Change screens
        if (input_check(LEFT_BUTTON) == PUSHED) {
            input_comms_send(MSG_SCREEN_LEFT);
        }

        if (input_check(RIGHT_BUTTON) == PUSHED) {
            input_comms_send(MSG_SCREEN_RIGHT);
        }

        // Up down buttons
        input_state = input_check(UP_BUTTON);
        if (input_state == PUSHED) {
            input_comms_send(MSG_UP_BUTTON_P);
        } else if (input_state == RELEASED) {
            input_comms_send(MSG_UP_BUTTON_R);
        }

        input_state = input_check(DOWN_BUTTON);
        if (input_state == PUSHED) {
            input_comms_send(MSG_DOWN_BUTTON_P);
        } else if (input_state == RELEASED) {
            input_comms_send(MSG_DOWN_BUTTON_R);
        }

        // Switch
        input_state = input_check(RIGHT_SWITCH);
        if (input_state == PUSHED) {
            input_comms_send(MSG_RIGHT_SWITCH_ON);
        } else if (input_state == RELEASED) {
            input_comms_send(MSG_RIGHT_SWITCH_OFF);
        }

        vTaskDelay(10);
    }
}
