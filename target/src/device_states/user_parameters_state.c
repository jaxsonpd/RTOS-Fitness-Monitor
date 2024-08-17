/** 
 * @file user_parameters_state.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Implementation of state
 */

#include <stdint.h>

#include "../hal/display_hal.h"
#include "user_parameters_state.h"

void userParametersState_enter(void) {
    display_line("user parameters", 0, ALIGN_CENTRE);
}

void userParametersState_execute(void* args) {
    // TODO
}

void userParametersState_exit(void) {
    display_clear();
}

state_t userParametersState = {userParametersState_enter,userParametersState_execute,userParametersState_exit};