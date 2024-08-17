/** 
 * @file steps_state.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Implementation of state for setting user parameters
 */

#include <stdint.h>

#include "../hal/display_hal.h"
#include "goal_state.h"

void goalState_enter(void) {
    display_line("2goal", 0, ALIGN_CENTRE);
}

void goalState_execute(void* args) {
    // TODO
}

void goalState_exit(void) {
    display_clear();
}

state_t goalState = { goalState_enter,goalState_execute,goalState_exit };