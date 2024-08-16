// /** 
//  * @file user_parameters_state.c
//  * @author Isaac Cone (ico29@uclive.ac.nz)
//  * @date 2024-08
//  * @brief Implementation of state for setting user parameters
//  */

// #include <stdint.h>

// #include "pot_comms.h"
// #include "input_comms.h"

// #include "user_parameters_state.h"

// #define WEIGHT_DEFAULT 70
// #define WEIGHT_MAX 150
// #define WEIGHT_POT_SCALE_COEFF WEIGHT_MAX / POT_MAX

// #define HEIGHT_DEFAULT 150
// #define HEIGHT_MAX 200
// #define HEIGHT_POT_SCALE_COEFF HEIGHT_MAX / POT_MAX

// void userParametersState_enter(void) {
    
// }

// /**
//  * @brief display the set user parameters screen
//  */
// void display_set_user_parameters(void) {
//     uint32_t new_weight = WEIGHT_DEFAULT;
//     uint32_t new_height = HEIGHT_DEFAULT;
//     uint32_t adc_value = pot_get();

//     if (adc_value != 0) {
//         if (display_info_get_toggle()) {
//             new_weight = adc_value * WEIGHT_POT_SCALE_COEFF;
//             new_height = display_info_get_height();
//         } else if (!display_info_get_toggle()) {
//             new_height = adc_value * HEIGHT_POT_SCALE_COEFF;
//             new_weight = display_info_get_weight();
//         }
//     }


//     if (display_info_get_input_flag(MSG_DOWN_BUTTON_P) && !display_info_get_debug() && display_info_get_toggle()) {
//         display_info_set_weight(new_weight);
//     } else if (display_info_get_input_flag(MSG_DOWN_BUTTON_P) && !display_info_get_debug() && !display_info_get_toggle()) {
//         display_info_set_height(new_height);
//     }

//     display_line("Weight",0,ALIGN_CENTRE);
//     display_line("Height",2,ALIGN_CENTRE);

//     char toDraw[OUTPUT_WIDTH + 1]; // Must be one character longer to account for EOFs
//     uint16_t weight = new_weight;
//     uint16_t current_weight = display_info_get_weight();
//     if (display_info_get_units() != UNITS_SI) {
//         weight *= KG_TO_LB;
//         current_weight *= KG_TO_LB;
//     }
//     if (display_info_get_toggle()) {
//         usnprintf(toDraw, OUTPUT_WIDTH + 1, "%d %s <- %d %s", current_weight, display_info_get_units() == UNITS_SI ? "kg" : "lb", weight, display_info_get_units() == UNITS_SI ? "kg" : "lb");
//     } else {
//         usnprintf(toDraw, OUTPUT_WIDTH + 1, "%d %s", current_weight, display_info_get_units() == UNITS_SI ? "kg" : "lb");
//     }
//     display_line(toDraw, 1, ALIGN_LEFT);

//     uint16_t height = new_height;
//     uint16_t current_height = display_info_get_height();
//     if (display_info_get_units() != UNITS_SI) {
//         height = CM_TO_INCHES(height);
//         current_height = CM_TO_INCHES(current_height);
//     }
//     if (!display_info_get_toggle()) {
//         usnprintf(toDraw, OUTPUT_WIDTH + 1, "%d %s <- %d %s", current_height, display_info_get_units() == UNITS_SI ? "cm" : "in", height, display_info_get_units() == UNITS_SI ? "cm" : "in");    
//     } else {
//         usnprintf(toDraw, OUTPUT_WIDTH + 1, "%d %s", current_height, display_info_get_units() == UNITS_SI ? "cm" : "in");    
//     }
//     display_line(toDraw, 3, ALIGN_LEFT);
// }