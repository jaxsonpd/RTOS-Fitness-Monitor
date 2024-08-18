/** 
 * @file move_prompt_alert_state.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Implementation of state for move alert
 */

#include <stdint.h>

#include "../hal/display_hal.h"
#include "../device_info.h"

#include "move_prompt_alert_state.h"

#define FLASH_LENGTH 5 // deci seconds

static uint32_t flash_time = 0;

void move_prompt_alert_state_enter(void) {
    display_line("Please Move", 1, ALIGN_CENTRE);
    flash_time = device_info_get_ds();
}

stateStatus_t move_prompt_alert_state_execute(void* args) {
    if ((device_info_get_ds() - flash_time) >= FLASH_LENGTH) {
        return STATE_FINISHED;
    }

    return STATE_SUCCESS;
}

void move_prompt_alert_state_exit(void) {
    display_clear();
}

void move_prompt_alert_state_reset(void) {
    flash_time = 0;
}

state_t movePromptAlertState = { move_prompt_alert_state_enter,move_prompt_alert_state_execute,move_prompt_alert_state_exit,move_prompt_alert_state_reset };