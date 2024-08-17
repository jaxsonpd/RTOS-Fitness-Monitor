/** 
 * @file steps_state.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Implementation of state for setting user parameters
 */

#include <stdint.h>

#include "../comms/step_counter_comms.h"
#include "../hal/display_hal.h"
#include "../device_info.h"
#include "../utility/person.h"

#include "steps_state.h"

/** 
 * @brief Called once on state entry
 * 
 */
void steps_state_enter(void) {
    display_line("", 0, ALIGN_CENTRE);
    display_line("", 3, ALIGN_CENTRE);
}

/** 
 * @brief Called contionously while in state
 * @param args arguments from the state machine
 * 
 * @return the status of the state
 */
stateStatus_t steps_state_execute(void* args) {
    person_t *person = (person_t *)args;
    
    if (device_info_get_units() == UNITS_SI) {
        display_value("", "steps", step_counter_get(), 1, ALIGN_CENTRE, false);
    } else {
        display_value("", "% of goal", step_counter_get() * 100 / person->userGoal, 1, ALIGN_CENTRE, false);
    }

    uint32_t workout_time = 0;
    uint32_t workout_start_time = device_info_get_workout_start_time();
    if (workout_start_time != 0) {
        workout_time = device_info_get_ds() - workout_start_time;
    }

    display_time("Time:", workout_time * DS_TO_S, 2, ALIGN_CENTRE);

    return STATE_SUCCESS;
}

/** 
 * @brief Called on state change
 * 
 */
void steps_state_exit(void) {
    display_clear();
}

state_t stepsState = { steps_state_enter,steps_state_execute,steps_state_exit };