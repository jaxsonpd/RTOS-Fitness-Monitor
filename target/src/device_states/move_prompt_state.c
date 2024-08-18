/** 
 * @file move_prompt_state.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Implementation of state for setting user parameters
 */

#include <stdint.h>

#include "utils/ustdlib.h"

#include "../hal/display_hal.h"
#include "../device_info.h"
#include "../comms/pot_comms.h"
#include "../utility/person.h"

#include "move_prompt_state.h"

#define PROMPT_TO_MOVE_TIME_DEFAULT 100 // 10 seconds
#define PROMPT_TO_MOVE_TIME_MAX 3600 // 1 hour
#define PROMPT_TO_MOVE_TIME_ROUNDING 10
#define PROMPT_TO_MOVE_TIME_POT_SCALE_COEFF PROMPT_TO_MOVE_TIME_MAX / POT_MAX

void move_prompt_state_enter(void) {
    display_line("Set prompt time:", 0, ALIGN_CENTRE);
}

stateStatus_t move_prompt_state_excute(void* args) {
    person_t *person = (person_t *)args;

    uint32_t new_prompt_time = PROMPT_TO_MOVE_TIME_DEFAULT;
    uint32_t adc_value = pot_get();

    if (adc_value != 0) {
        new_prompt_time = adc_value * PROMPT_TO_MOVE_TIME_POT_SCALE_COEFF;
        new_prompt_time = (new_prompt_time / PROMPT_TO_MOVE_TIME_ROUNDING) * PROMPT_TO_MOVE_TIME_ROUNDING;
    }

    if (device_info_get_input_flag(MSG_DOWN_BUTTON_P) && !device_info_get_debug()) {
        person->userActivityTimeout = new_prompt_time;
    }

    display_value("Current:", "", person->userActivityTimeout, 2, ALIGN_CENTRE, false);

    char toDraw[DISPLAY_WIDTH + 1]; // Must be one character longer to account for EOFs
    usnprintf(toDraw, DISPLAY_WIDTH + 1, "%d s", new_prompt_time);
    display_line(toDraw, 1, ALIGN_CENTRE);
    display_line("", 3, ALIGN_CENTRE);

    return STATE_SUCCESS;
}

void move_prompt_state_exit(void) {
    display_clear();
}

void move_prompt_state_reset(void) {

}
state_t movePromptState = { move_prompt_state_enter,move_prompt_state_excute,move_prompt_state_exit,move_prompt_state_reset };