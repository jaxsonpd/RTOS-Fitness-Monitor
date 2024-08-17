/** 
 * @file move_prompt_alert_state.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Implementation of state for setting user parameters
 */

#include <stdint.h>

#include "../hal/display_hal.h"
#include "move_prompt_alert_state.h"

void movePromptAlertState_enter(void) {
    display_line("7goal_reached", 0, ALIGN_CENTRE);
}

void movePromptAlertState_execute(void* args) {
    // TODO
}

void movePromptAlertState_exit(void) {
    display_clear();
}

state_t movePromptAlertState = { movePromptAlertState_enter,movePromptAlertState_execute,movePromptAlertState_exit };