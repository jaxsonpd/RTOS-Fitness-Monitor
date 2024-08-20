/**
 * @file move_prompt_state.c
 * @author Daniel Hawes (dha144@uclive.ac.nz), Isaac Cone (ico29@uclive.ac.nz), Jack Duignan (jdu80@uclive.ac.nz)
 * @date 2024-08
 * @brief Implementation of state for setting move prompt
 */

#include <stdint.h>

#include "utils/ustdlib.h"

#include "../hal/display_hal.h"
#include "../device_info.h"
#include "../comms/pot_comms.h"
#include "../utility/person.h"

#include "move_prompt_state.h"

#define PROMPT_TO_MOVE_TIME_DEFAULT 100 // 100 seconds
#define PROMPT_TO_MOVE_TIME_MAX 3590 // 1 hour - 10 secs
#define PROMPT_TO_MOVE_TIME_ROUNDING 10 // Increment by 10 seconds 
#define PROMPT_TO_MOVE_TIME_POT_SCALE_COEFF PROMPT_TO_MOVE_TIME_MAX / POT_MAX

void move_prompt_state_enter(void) {
    display_line("Set prompt time:", 0, ALIGN_CENTRE);
    display_line("", 3, ALIGN_CENTRE);
}

stateStatus_t move_prompt_state_excute(void* args) {
    person_t* person = (person_t*)args;

    uint32_t new_prompt_time = PROMPT_TO_MOVE_TIME_DEFAULT;
    uint32_t dial_value = pot_get();

    if (dial_value != 0) {
        new_prompt_time = dial_value * PROMPT_TO_MOVE_TIME_POT_SCALE_COEFF;
        new_prompt_time = ((new_prompt_time / PROMPT_TO_MOVE_TIME_ROUNDING) * PROMPT_TO_MOVE_TIME_ROUNDING + 10);
    } else {
        new_prompt_time = PROMPT_TO_MOVE_TIME_ROUNDING;
    }

    if (device_info_get_input_flag(MSG_DOWN_BUTTON_P) && !device_info_get_debug()) {
        device_info_set_last_step_time(device_info_get_ds());
        person->userActivityTimeout = new_prompt_time;
    }

    display_time("", new_prompt_time, 1, ALIGN_CENTRE);
    display_time("Current:", person->userActivityTimeout, 2, ALIGN_CENTRE);
    display_time("Inactive:", (device_info_get_ds() - device_info_get_last_step_time()) * DS_TO_S, 3, ALIGN_CENTRE);

    return STATE_SUCCESS;
}

void move_prompt_state_exit(void) {
    display_clear();
}

void move_prompt_state_reset(void) {
    device_info_set_last_step_time(device_info_get_ds());
}
state_t movePromptState = { move_prompt_state_enter,move_prompt_state_excute,move_prompt_state_exit,move_prompt_state_reset };