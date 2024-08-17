/** 
 * @file goal_reached_state.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Implementation of state for setting user parameters
 */

#include <stdint.h>

#include "../hal/display_hal.h"
#include "steps_state.h"

void goalReachedState_enter(void) {
    display_line("6goal_reached", 0, ALIGN_CENTRE);
}

void goalReachedState_execute(void* args) {
    // TODO
}

void goalReachedState_exit(void) {
    display_clear();
}

state_t goalReachedState = { goalReachedState_enter,goalReachedState_execute,goalReachedState_exit };