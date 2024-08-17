/** 
 * @file steps_state.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Implementation of state for setting user parameters
 */

#include <stdint.h>

#include "../hal/display_hal.h"
#include "distance_state.h"

void distanceState_enter(void) {
    display_line("1distance", 0, ALIGN_CENTRE);
}

char distanceState_execute(void* args) {
    // TODO
}

void distanceState_exit(void) {
    display_clear();
}

state_t distanceState = { distanceState_enter,distanceState_execute,distanceState_exit };