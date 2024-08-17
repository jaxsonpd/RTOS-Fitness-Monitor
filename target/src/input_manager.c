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

#include "hal/input_hal.h"
#include "comms/input_comms.h"

#include "input_manager.h"

/**
 * @brief Initialise the input manager thread
 *
 * @return true if successful
 */
bool input_manager_init(void) {
    bool result = true;
    result = input_init() & result;
    result = input_comms_init() & result;

    // Setup inital states
    for (uint8_t i = 0; i < DEBOUNCE_NUMBER; i++) {
        input_update();
    }

    // Make sure that switches are in correct state
    if (input_get(RIGHT_SWITCH)) {
        input_comms_send(MSG_RIGHT_SWITCH_ON);
    }

    if (input_get(LEFT_SWITCH)) {
        input_comms_send(MSG_LEFT_SWITCH_ON);
    }

    return result;
}

void input_manager_send_updates(void) {
    inputState_t input_state;

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
}

void input_manager_thread(void* rtos_param) {
    input_manager_init();

    for (;;) {
        input_update();

        input_manager_send_updates();

        vTaskDelay(10);
    }
}
