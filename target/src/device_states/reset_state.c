/** 
 * @file reset_state.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Implementation of state for setting user parameters
 */

#include <stdint.h>

#include "../hal/display_hal.h"
#include "../device_info.h"

#include "reset_state.h"


#define FLASH_LENGTH 15 // deci seconds

static uint32_t flash_time = 0;

void reset_state_enter(void) {
    display_line("reset", 1, ALIGN_CENTRE);
    flash_time = device_info_get_ds();
}

stateStatus_t reset_state_execute(void* args) {
    if ((device_info_get_ds() - flash_time) >= FLASH_LENGTH) {
        return STATE_FINISHED;
    }

    return STATE_SUCCESS;
}

void reset_state_exit(void) {
    display_clear();
}

state_t resetState = { reset_state_enter,reset_state_execute,reset_state_exit };