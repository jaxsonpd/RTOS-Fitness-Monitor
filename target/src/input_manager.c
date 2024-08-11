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

#include "input_manager.h"

/**
 * @brief Initialise the input manager thread
 *
 * @return true if successful
 */
bool input_manager_init(void) {
    bool result = true;
    result = input_init();
    result = input_comms_init();

    return result;
}

void input_manager_thread(void* rtos_param) {
    BaseType_t xStatus;
    input_state_t input_state;

    input_manager_init();

    // Setup inital states
    for (uint8_t i = 0; i < DEBOUNCE_NUMBER; i++) {
        input_update();
    }

    if (input_get(RIGHT_SWITCH)) { // Make sure that can have debug enabled on startup
        input_comms_send(MSG_RIGHT_SWITCH_ON);
    }

    if (input_get(LEFT_SWITCH)) { // Make sure that can have debug enabled on startup
        input_comms_send(MSG_LEFT_SWITCH_ON);
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

        input_state = input_check(LEFT_SWITCH);
        if (input_state == PUSHED) {
            input_comms_send(MSG_LEFT_SWITCH_ON);
        } else if (input_state == RELEASED) {
            input_comms_send(MSG_LEFT_SWITCH_OFF);
        }

        vTaskDelay(10);
    }
}
