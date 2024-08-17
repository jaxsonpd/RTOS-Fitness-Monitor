/** 
 * @file user_parameters_state.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Implementation of state
 */

#include <stdint.h>

#include "utils/ustdlib.h"

#include "state.h"

#include "../hal/display_hal.h"
#include "../comms/pot_comms.h"
#include "../utility/person.h"
#include "../device_info.h"


#include "user_parameters_state.h"

#define WEIGHT_DEFAULT 70
#define WEIGHT_MAX 150
#define WEIGHT_POT_SCALE_COEFF WEIGHT_MAX / POT_MAX

#define HEIGHT_DEFAULT 150
#define HEIGHT_MAX 200
#define HEIGHT_POT_SCALE_COEFF HEIGHT_MAX / POT_MAX

#define KG_TO_LB 22/10
#define CM_TO_INCHES(x) (x*394)/1000
#define M_PER_STEP(x) (x*415)/100000


/** 
 * @brief Called once on state entry
 * 
 */
void user_parameters_state_enter(void) {
    display_line("Weight",0,ALIGN_CENTRE);
    display_line("Height",2,ALIGN_CENTRE);
}

/** 
 * @brief Called contionously while in state
 * @param args arguments from the state machine
 * 
 * @return the status of the state
 */
stateStatus_t user_parameters_state_execute(void* args) {
    person_t *person = (person_t *)args;

    uint32_t new_weight = WEIGHT_DEFAULT;
    uint32_t new_height = HEIGHT_DEFAULT;
    uint32_t adc_value = pot_get();

    if (adc_value != 0) {
        if (device_info_get_alternate()) { // Setting weight
            new_weight = adc_value * WEIGHT_POT_SCALE_COEFF;
        } else if (!device_info_get_alternate()) {
            new_height = adc_value * HEIGHT_POT_SCALE_COEFF;
        }
    }

    if (device_info_get_input_flag(MSG_DOWN_BUTTON_P) && !device_info_get_debug()) {
        if (device_info_get_alternate()) {
            person->userWeight = new_weight;
        } else if (!device_info_get_alternate()) {
            person->userHeight = new_height;
        }
    }
    

    // Generate weight line
    char toDraw[DISPLAY_WIDTH + 1]; // Must be one character longer to account for EOFs
    uint16_t weight = new_weight;
    uint16_t current_weight = person->userWeight;
    if (device_info_get_units() != UNITS_SI) {
        weight *= KG_TO_LB;
        current_weight *= KG_TO_LB;
    }
    if (device_info_get_alternate()) {
        usnprintf(toDraw, DISPLAY_WIDTH + 1, "%3d %s <- %3d %s", current_weight, 
                device_info_get_units() == UNITS_SI ? "kg" : "lb",
                weight, device_info_get_units() == UNITS_SI ? "kg" : "lb");
    } else {
        usnprintf(toDraw, DISPLAY_WIDTH + 1, "%3d %s", current_weight, 
                device_info_get_units() == UNITS_SI ? "kg" : "lb");
    }
    display_line(toDraw, 1, ALIGN_LEFT);

    // Generate height line
    uint16_t height = new_height;
    uint16_t current_height = person->userHeight;
    if (device_info_get_units() != UNITS_SI) {
        height = CM_TO_INCHES(height);
        current_height = CM_TO_INCHES(current_height);
    }
    if (!device_info_get_alternate()) {
        if (device_info_get_units() == UNITS_SI) {
            usnprintf(toDraw, DISPLAY_WIDTH + 1, "%3d cm <- %d cm", current_height, height);    
        } else {
            usnprintf(toDraw, DISPLAY_WIDTH + 1, "%d'%2d\"  <- %d'%2d\"", current_height / 12, current_height % 12, height / 12, height % 12);    
        }
    } else {
        if (device_info_get_units() == UNITS_SI) {
            usnprintf(toDraw, DISPLAY_WIDTH + 1, "%3d cm", current_height);    
        } else {
            usnprintf(toDraw, DISPLAY_WIDTH + 1, "%d'%2d\"", current_height / 12, current_height % 12);    
        }
    }
    display_line(toDraw, 3, ALIGN_LEFT);

    return STATE_SUCCESS;
}

/** 
 * @brief Called on state change
 * 
 */
void user_parameters_state_exit(void) {
    display_clear();
}

state_t userParametersState = {user_parameters_state_enter,user_parameters_state_execute,user_parameters_state_exit};