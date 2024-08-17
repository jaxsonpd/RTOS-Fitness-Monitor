/** 
 * @file steps_state.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Implementation of state for setting user parameters
 */

#include <stdint.h>

#include "../comms/step_counter_comms.h"
#include "../hal/display_hal.h"
#include "steps_state.h"

void stepsState_enter(void) {
    display_line("0steps", 0, ALIGN_CENTRE);
}

char stepsState_execute(void* args) {
    uint32_t step_count = step_counter_get();
    display_value("steps: ", "", step_count, 2, ALIGN_CENTRE, false);
}

void stepsState_exit(void) {
    display_clear();
}

state_t stepsState = { stepsState_enter,stepsState_execute,stepsState_exit };