/** 
 * @file steps_state.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Implementation of state for setting user parameters
 */

#include <stdint.h>

#include "../hal/display_hal.h"
#include "steps_state.h"

void stepsState_enter(void) {
    display_line("0steps", 0, ALIGN_CENTRE);
}

void stepsState_execute(void* args) {
    // TODO
}

void stepsState_exit(void) {
    display_clear();
}

state_t stepsState = { stepsState_enter,stepsState_execute,stepsState_exit };