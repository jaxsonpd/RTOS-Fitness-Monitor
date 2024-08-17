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

#include "device_states/steps_state.h"
#include "device_states/distance_state.h"
#include "device_states/goal_state.h"
#include "device_states/move_prompt_state.h"
#include "device_states/stopwatch_state.h"
#include "device_states/user_parameters_state.h"

#include "device_info.h"
#include "hal/display_hal.h"

#include "FreeRTOS.h"
#include "task.h"

#define NUM_STATES 6
#define INITIAL_STATE_ID 0

state_t* states[NUM_STATES] = { &stepsState, &distanceState, &goalState, &stopwatchState, &movePromptState, &userParametersState };

void update_inputs(void);

bool device_manager_init(device_state_t* device) 
{
    display_init();
    return device_state_init(device, states[INITIAL_STATE_ID]);
}

void device_manager_thread(void* args) 
{
    device_state_t device;
    uint8_t current_state_id = INITIAL_STATE_ID;
    uint8_t new_state_id = current_state_id;
    device_manager_init(&device);

    for (;;) {
        update_inputs();
        if (device_info_get_input_flag(MSG_SCREEN_LEFT)) {
            if (current_state_id > 0) {
                new_state_id = current_state_id - 1;
            } else {
                new_state_id = NUM_STATES - 1;
            }
        } else if (device_info_get_input_flag(MSG_SCREEN_RIGHT)) {
            new_state_id = (current_state_id + 1) % NUM_STATES;
        }

        if (new_state_id != current_state_id) {
            device_state_set(&device, states[new_state_id]);
            current_state_id = new_state_id;
        }
        device_state_execute(&device,NULL);
        for (uint8_t i = 0; i < NUM_MSGS; i++) {
            device_info_set_input_flag(i, 0);
        }
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