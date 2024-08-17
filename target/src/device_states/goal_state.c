/** 
 * @file steps_state.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Display the set goal screen
 */

#include <stdint.h>

#include "utils/ustdlib.h"

#include "../hal/display_hal.h"
#include "../comms/pot_comms.h"
#include "../device_info.h"
#include "../utility/person.h"

#include "goal_state.h"

#define KM_TO_MILES 62/100 // Multiply by 0.6215 to convert, this should be good enough

#define GOAL_DEFAULT 1000
#define GOAL_MAX 19900 // 20000 - 1000
#define GOAL_POT_SCALE_COEFF GOAL_MAX / POT_MAX // in steps, adjusting to account for the potentiometer's maximum possible reading

#define M_PER_STEP(x) (x*415)/100000

#define STEP_GOAL_ROUNDING 100

#define NOISE_MARGIN 100

/** 
 * @brief Called once on state entry
 * 
 */
void goal_state_enter(void) {
    display_line("Set goal:", 0, ALIGN_CENTRE);
    display_line("", 3, ALIGN_CENTRE);
}

/** 
 * @brief Called continuously while in state
 * @param args arguments from the state machine
 * 
 * @return the status of the state
 */
stateStatus_t goal_state_execute(void* args) {
    person_t *person = (person_t *)args;

    // Get new goal
    uint32_t new_goal = GOAL_DEFAULT;
    uint32_t adc_value = pot_get();
    if (adc_value != 0) {
        new_goal = adc_value * GOAL_POT_SCALE_COEFF;
        new_goal = (new_goal / STEP_GOAL_ROUNDING) * STEP_GOAL_ROUNDING + STEP_GOAL_ROUNDING;
    }

    // Check if new goal has been set
    if (device_info_get_input_flag(MSG_DOWN_BUTTON_P) && !device_info_get_debug()) {
        person->userGoal = new_goal;
        // g_display_mode = DISPLAY_STEPS;
    }

    // Display the step/distance preview
    char new_goal_line[DISPLAY_WIDTH + 1]; // Must be one character longer to account for EOFs
    uint16_t distance = new_goal * M_PER_STEP(person->userHeight); // ===== NEEDS to be updated to new goal
    if (device_info_get_units() != UNITS_SI) {
        distance = distance * KM_TO_MILES;
    }

    // if <10 km/miles, use a decimal point. Otherwise display whole units (to save space)
    if (distance < 10 * 1000) {
        usnprintf(new_goal_line, DISPLAY_WIDTH + 1, "%d stps/%d.%01d%s", new_goal, distance / 1000, (distance % 1000) / 100, device_info_get_units() == UNITS_SI ? "km" : "mi");
    } else {
        usnprintf(new_goal_line, DISPLAY_WIDTH + 1, "%d stps/%d%s", new_goal, distance / 1000, device_info_get_units() == UNITS_SI ? "km" : "mi");
    }
    

    display_line(new_goal_line, 1, ALIGN_CENTRE);
    display_value("Current:", "", person->userGoal, 2, ALIGN_CENTRE, false);

    return STATE_SUCCESS;
}

/** 
 * @brief Called on state change
 * 
 */
void goal_state_exit(void) {
    display_clear();
}

state_t goalState = { goal_state_enter,goal_state_execute,goal_state_exit };