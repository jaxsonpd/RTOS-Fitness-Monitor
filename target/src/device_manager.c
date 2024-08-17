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
#include "device_states/device_state_handler.h"
#include "device_manager.h"

#include "utility/person.h"
#include "device_info.h"
#include "hal/display_hal.h"

#include "FreeRTOS.h"
#include "task.h"

#include "comms/step_counter_comms.h"

#define NUM_SCREENS 6
#define INITIAL_STATE_ID 0

#define DEBUG_STEP_INCREMENT 100
#define DEBUG_STEP_DECREMENT 500

#define LONG_PRESS_TIME 3/2

static uint32_t g_reset_timeout = 0;

void update_inputs(device_state_t* device);
void handle_input(device_state_t* device, inputCommMsg_t msg);
void update_state(device_state_t* device, person_t* person);

bool device_manager_init(device_state_t* device) 
{
    display_init();
    return device_state_init(device, INITIAL_STATE_ID);
}

/**
 * @brief Update the step counter and check for workout start
 *
 * @return true if the workout is started
 */
bool update_steps(void) {
    static uint32_t old_steps = 0;;
    uint32_t new_steps = step_counter_get();

    if (new_steps > old_steps) {
        device_info_set_last_step_time(device_info_get_ds());
        if (device_info_get_workout_start_time() == 0) {
            device_info_set_workout_start_time(device_info_get_ds());
            device_info_set_last_step_time(device_info_get_ds());
            return true;
        }
    }

    old_steps = new_steps;

    return false;
}

void device_manager_thread(void* args) 
{
    device_state_t device;
    device_manager_init(&device);
    person_t person;
    person_init(&person);
    char status;

    for (;;) {
        update_steps();

        status = device_state_execute(&device,&person);
        device_info_clear_input_flags();
        if (status != STATE_FLASHING) {
            update_inputs(&device);
            update_state(&device,&person);
        }
        vTaskDelay(50);
    }
}

/**
 * @brief update the input flags and any state change based on inputs
 *
 */
void update_inputs(device_state_t* device) {
    uint8_t num_reads = 0;
    while (input_comms_num_msgs() > 0 && num_reads < 5) {
        inputCommMsg_t msg = input_comms_receive();
        handle_input(device, msg);
        device_info_set_input_flag(msg, 1);
        num_reads++;
    }
}

/**
 * @brief parses message to perform required behaviour from input
 *
 */
void handle_input(device_state_t* device, inputCommMsg_t msg) {
    stateID_t newID = device->currentID;
    switch (msg) {
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
        if (device_info_get_debug()) {
            step_counter_set(DEBUG_STEP_INCREMENT, true);
        } else {
            if (device_info_get_units() == UNITS_SI) {
                device_info_set_units(UNITS_ALTERNATE);
            } else {
                device_info_set_units(UNITS_SI);
            }
        }
        break;

    case (MSG_DOWN_BUTTON_P):
        g_reset_timeout = device_info_get_ds();

        if (device_info_get_debug()) {
            step_counter_set(DEBUG_STEP_DECREMENT, false);
        }
        break;

    case (MSG_DOWN_BUTTON_R):
        g_reset_timeout = 0;
        break;

    default:
        break;
    }
    if (newID != device->currentID) {
        device_state_set(device, newID);
    }
}

/**
 * @brief Check reached goal
 * @param reset if true reset the function
 *
 * @return true if goal has been reached for the first time
 */
bool check_goal_reached(bool reset, person_t* person) {
    static uint32_t last_goal = 0;

    if (reset) {
        last_goal = 0;
        return false;
    }

    if (step_counter_get() >= person->userGoal && last_goal != person->userGoal) {
        last_goal = person->userGoal;
        return true;
    }

    return false;
}

/**
 * @brief Check how long since last step
 * @param reset if true reset the function
 * 
 * @return true if the user has been inactive for too long
 */
bool check_inactive(bool reset, person_t* person) {

    if (reset) {
        return false;
    }

    if ((device_info_get_ds() - device_info_get_last_step_time()) > person->userActivityTimeout * S_TO_DS && device_info_get_last_step_time() != device_info_get_ds()) {
        return true;
    }   

    return false;
}

/**
 * @brief update states based on non-standard input conditions
 * handles states that are not accessed in the loop.
 */
void update_state(device_state_t* device, person_t* person) {
    stateID_t newID = device->currentID;
    if (check_goal_reached(false, person)) {
        newID = GOAL_REACHED_STATE_ID;
    }
    if (check_inactive(false, person)) {
        newID = MOVE_PROMPT_ALERT_STATE_ID;
    }
    if ((g_reset_timeout > 0) && ((device_info_get_ds() - g_reset_timeout) > LONG_PRESS_TIME * S_TO_DS)) {
        newID = RESET_STATE_ID;
        g_reset_timeout = 0;
    }
    if (newID != device->currentID) {
        device_state_set(device, newID);
    }
}