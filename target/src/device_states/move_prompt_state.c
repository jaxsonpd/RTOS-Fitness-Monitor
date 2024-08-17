/** 
 * @file move_prompt_state.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Implementation of state for setting user parameters
 */

#include <stdint.h>

#include "../hal/display_hal.h"
#include "move_prompt_state.h"

void movePromptState_enter(void) {
    display_line("4move prompt", 0, ALIGN_CENTRE);
}

char movePromptState_execute(void* args) {
    // TODO
}

void movePromptState_exit(void) {
    display_clear();
}

state_t movePromptState = { movePromptState_enter,movePromptState_execute,movePromptState_exit };