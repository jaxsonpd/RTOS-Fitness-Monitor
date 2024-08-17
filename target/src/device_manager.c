/** 
 * @file device_manager.h
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Implementation of the device manager thread which handles the device state
 */

#include <stdbool.h>
#include <stdlib.h>

#include "device_states/state.h"

#include "device_states/state.h"
#include "device_states/device_state.h"
#include "device_manager.h"

#include "device_info.h"
#include "hal/display_hal.h"

#include "FreeRTOS.h"
#include "task.h"

#define NUM_SCREENS 6
#define INITIAL_STATE_ID 0

void update_inputs(void);
void process_inputs(device_state_t* device);

bool device_manager_init(device_state_t* device) 
{
    display_init();
    return device_state_init(device, INITIAL_STATE_ID);
}

void device_manager_thread(void* args) 
{
    device_state_t device;
    device_manager_init(&device);
    for (;;) {
        update_inputs();
        process_inputs(&device);
        device_state_execute(&device,NULL);
        device_info_clear_input_flags();
        vTaskDelay(5);
    }
}

/**
 * @brief update the input flags and display state
 *
 */
void update_inputs(void) {
    uint8_t num_tries = 0;
    while (input_comms_num_msgs() > 0 && num_tries < 5) {
        inputCommMsg_t msg = input_comms_receive();
        device_info_set_input_flag(msg, 1);
        num_tries++;
    }
}

void process_inputs(device_state_t* device) {
    // static uint32_t down_button_p_time = 0;
    state_id_t newID = device->currentID;
    for (uint16_t i = 0; i < NUM_MSGS; i++) {
        if (!device_info_get_input_flag(i)) {
            continue;
        }
        switch (i) {
        case (MSG_SCREEN_LEFT):
            if (device->currentID > 0) {
                newID = device->currentID - 1;
            } else {
                newID = NUM_SCREENS - 1;
            }
            break;

        case (MSG_SCREEN_RIGHT):
            newID = (device->currentID + 1) % NUM_SCREENS;
            break;

        case (MSG_LEFT_SWITCH_ON):
            device_info_set_alternate(true);
            break;
        case (MSG_RIGHT_SWITCH_ON):
            device_info_set_debug(true);
            break;

        case (MSG_LEFT_SWITCH_OFF):
            device_info_set_alternate(false);
            break;
        case (MSG_RIGHT_SWITCH_OFF):
            device_info_set_debug(false);
            break;

        case (MSG_UP_BUTTON_P):
            // if (device_info_get_debug()) {
                // display_info_set_steps(display_info_get_steps() + DEBUG_STEP_INCREMENT);
            // } else {
            //     if (display_info_get_units() == UNITS_SI) {
            //         display_info_set_units(UNITS_ALTERNATE);
            //     } else {
            //         display_info_set_units(UNITS_SI);
            //     }
            // }
            break;

        case (MSG_DOWN_BUTTON_P):
            // down_button_p_time = display_info_get_ds();

            // if (display_info_get_debug()) {
            //     if (display_info_get_steps() >= DEBUG_STEP_DECREMENT) {
            //         display_info_set_steps(display_info_get_steps() - DEBUG_STEP_INCREMENT);
            //     } else {
            //         display_info_set_steps(0);
            //     }
            // }
            break;

        case (MSG_DOWN_BUTTON_R):
            // down_button_p_time = 0;
            break;

        default:

            break;
        }

        // if ((down_button_p_time > 0) && ((display_info_get_ds() - down_button_p_time) > LONG_PRESS_TIME * S_TO_DS)) {
        //     *display_mode = DISPLAY_FLASH_RESET;
        //     display_info_set_steps(0);
        //     check_goal_reached(true);
        //     display_info_set_start(0);
        //     stopwatch_display(true);
        //     down_button_p_time = 0;
        // } 
    }
    if (newID != device->currentID) {
        device_state_set(device, newID);
        device->currentID = newID;
    }
}