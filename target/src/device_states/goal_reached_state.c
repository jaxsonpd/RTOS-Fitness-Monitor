/** 
 * @file goal_reached_state.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Implementation of state for setting user parameters
 */

#include <stdint.h>

#include "../hal/display_hal.h"
#include "../device_info.h"

#include "goal_reached_state.h"

#define FLASH_LENGTH 30 // 3 seconds

static uint32_t flash_time = 0;

void goal_reached_state_enter(void) {
    display_line("Goal Reached", 1, ALIGN_CENTRE);
    flash_time = device_info_get_ds();
}

stateStatus_t goal_reached_state_execute(void* args) {
    if ((device_info_get_ds() - flash_time) >= FLASH_LENGTH) {
        return STATE_FINISHED;
    }

    return STATE_SUCCESS;
}

void goal_reached_state_exit(void) {
    display_clear();
}

void goal_reached_state_reset(void) {
    flash_time = 0;
}

state_t goalReachedState = { goal_reached_state_enter,goal_reached_state_execute,goal_reached_state_exit,goal_reached_state_reset };