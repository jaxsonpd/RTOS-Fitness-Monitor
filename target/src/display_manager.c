/**
 * @file display_manager.c
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-08-11
 * @brief Definitions for the display manager thread which write information to
 * the OLED screen.
 */



#include <stdint.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"

#include "utils/ustdlib.h"

#include "step_counter_comms.h"
#include "input_comms.h"
#include "serial_sender.h"
#include "display_hal.h"
#include "display_info.h"
#include "pot_comms.h"
#include "stopwatch.h"

#include "display_manager.h"

#define LONG_PRESS_TIME 3/2
#define FLASH_TIME 3/2

#define KM_TO_MILES 62/100 // Multiply by 0.6215 to convert, this should be good enough
#define MS_TO_KMH 36/10
#define KG_TO_LB 22/10
#define CM_TO_INCHES(x) (x*394)/1000
#define M_PER_STEP(x) (x*415)/100000
#define CAL_TO_KJ(x) (4184*x)/1000
#define M_KG_TO_CAL(x,y) (125*x*y)/100000 // https://www.analog.com/en/resources/analog-dialogue/articles/pedometer-design-3-axis-digital-acceler.html

#define DEBUG_STEP_INCREMENT 100
#define DEBUG_STEP_DECREMENT 100

#define STEP_GOAL_ROUNDING 100

#define POT_MAX 4095

#define GOAL_DEFAULT 1000
#define GOAL_MAX 20000
#define GOAL_POT_SCALE_COEFF GOAL_MAX / POT_MAX // in steps, adjusting to account for the potentiometer's maximum possible reading

#define WEIGHT_DEFAULT 70
#define WEIGHT_MAX 150
#define WEIGHT_POT_SCALE_COEFF WEIGHT_MAX / POT_MAX

#define HEIGHT_DEFAULT 150
#define HEIGHT_MAX 200
#define HEIGHT_POT_SCALE_COEFF HEIGHT_MAX / POT_MAX

typedef enum {
    DISPLAY_STEPS = 0,
    DISPLAY_DISTANCE,
    DISPLAY_SET_GOAL,
    DISPLAY_STOPWATCH,
    DISPLAY_SET_USER_PARAMETERS,
    DISPLAY_NUM_STATES, // Automatically enumerates to the number of display states there can be
    DISPLAY_FLASH_RESET,
    DISPLAY_FLASH_GOAL_REACHED
} displayMode_t;

void display_update_state(displayMode_t* display_mode, inputCommMsg_t msg);
void display_update(displayMode_t* display_mode);
bool display_manager_init(void);

void display_set_user_parameters(void);
void display_set_goal(void);
void display_steps(void);
void display_distance(void);
void display_reset_screen(void);
void display_goal_reached(void);

/**
 * @brief Update the step counter and check for workout start
 *
 * @return true if the workout is started
 */
bool update_steps(void) {
    static uint32_t old_steps = 0;
    uint32_t steps = display_info_get_steps();
    uint32_t new_steps = step_counter_get();
    display_info_set_steps(steps + new_steps);

    if (display_info_get_start() == 0 && display_info_get_steps() > old_steps) {
        display_info_set_start(display_info_get_ds());
        return true;
    }

    old_steps = display_info_get_steps();

    return false;
}

/**
 * @brief Check reached goal
 * @param reset if true reset the function
 *
 * @return true if goal has been reached for the first time
 */
bool check_goal_reached(bool reset) {
    static uint32_t last_goal = 0;

    if (reset) {
        last_goal = 0;
        return false;
    }

    if (display_info_get_steps() >= display_info_get_goal() && last_goal != display_info_get_goal()) {
        last_goal = display_info_get_goal();
        return true;
    }

    return false;
}

/**
 * @brief update the input flags and display state
 * @param display_mode a pointer to the display mode
 *
 */
void update_inputs(displayMode_t* display_mode) {
    uint8_t num_tries = 0;
    while (input_comms_num_msgs() > 0 && num_tries < 5) {
        inputCommMsg_t msg = input_comms_receive();

        display_info_set_input_flag(msg, 1);
        display_update_state(display_mode, msg);

        num_tries++;
    }
}

void display_manager_thread(void* rtos_param) {
    displayMode_t display_mode = DISPLAY_STEPS;

    display_manager_init();


    for (;;) {
        update_inputs(&display_mode);

        // Allow for updates of on input tasks
        display_update_state(&display_mode, NO_MESSAGES);

        update_steps();

        if (check_goal_reached(false)) {
            display_mode = DISPLAY_FLASH_GOAL_REACHED;
        }

        display_update(&display_mode);

        for (uint8_t i = 0; i < NUM_MSGS; i++) {
            display_info_set_input_flag(i, 0);
        }

        vTaskDelay(1000 / 10);
    }
}

/**
 * @brief Initialise the display manager thread
 *
 * @return true if successful
 */
bool display_manager_init(void) {
    return display_init();
}


/**
 * @brief Update the global device state based on the message
 * per screen functions are handled in the respective screen module
 * itself
 * @param msg the comms message
 */
void display_update_state(displayMode_t* display_mode, inputCommMsg_t msg) {
    static uint32_t down_button_p_time = 0;

    switch (msg) {
    case (MSG_SCREEN_LEFT):
        *display_mode = (*display_mode + 1) % DISPLAY_NUM_STATES; // flicker when pressing button
        break;

    case (MSG_SCREEN_RIGHT):
        if (*display_mode > 0) {
            *display_mode -= 1;
        } else {
            *display_mode = DISPLAY_NUM_STATES - 1;
        }
        break;

    case (MSG_LEFT_SWITCH_ON):
        display_info_set_toggle(true);
        break;
    case (MSG_RIGHT_SWITCH_ON):
        display_info_set_debug(true);
        break;

    case (MSG_LEFT_SWITCH_OFF):
        display_info_set_toggle(false);
        break;
    case (MSG_RIGHT_SWITCH_OFF):
        display_info_set_debug(false);
        break;

    case (MSG_UP_BUTTON_P):
        if (display_info_get_debug()) {
            display_info_set_steps(display_info_get_steps() + DEBUG_STEP_INCREMENT);
        } else {
            if (display_info_get_units() == UNITS_SI) {
                display_info_set_units(UNITS_ALTERNATE);
            } else {
                display_info_set_units(UNITS_SI);
            }
        }
        break;

    case (MSG_DOWN_BUTTON_P):
        down_button_p_time = display_info_get_ds();

        if (display_info_get_debug()) {
            if (display_info_get_steps() >= DEBUG_STEP_DECREMENT) {
                display_info_set_steps(display_info_get_steps() - DEBUG_STEP_INCREMENT);
            } else {
                display_info_set_steps(0);
            }
        }
        break;

    case (MSG_DOWN_BUTTON_R):
        down_button_p_time = 0;
        break;

    default:

        break;
    }

    if ((down_button_p_time > 0) && ((display_info_get_ds() - down_button_p_time) > LONG_PRESS_TIME * S_TO_DS)) {
        *display_mode = DISPLAY_FLASH_RESET;
        display_info_set_steps(0);
        check_goal_reached(true);
        display_info_set_start(0);
        stopwatch_display(true);
        down_button_p_time = 0;
    }

}

/**
 * @brief Update the display
 *
 */
void display_update(displayMode_t* display_mode) {
    static bool first_time_flash = true;
    static uint32_t flash_start_time = 0;

    switch (*display_mode) {
    case DISPLAY_STEPS:
        display_steps();
        break;
    case DISPLAY_DISTANCE:
        display_distance();
        break;
    case DISPLAY_SET_GOAL:
        display_set_goal();
        break;
    case DISPLAY_SET_USER_PARAMETERS:
        display_set_user_parameters();
        break;
    case DISPLAY_FLASH_RESET:
        if (first_time_flash) {
            flash_start_time = display_info_get_ds();
            first_time_flash = false;
        }

        if (display_info_get_ds() - flash_start_time > FLASH_TIME * S_TO_DS) {
            *display_mode = DISPLAY_STEPS;
            first_time_flash = true;
        }

        display_reset_screen();
        break;
    case DISPLAY_FLASH_GOAL_REACHED:
        if (first_time_flash) {
            flash_start_time = display_info_get_ds();
            first_time_flash = false;
        }

        if (display_info_get_ds() - flash_start_time > FLASH_TIME * S_TO_DS) {
            *display_mode = DISPLAY_STEPS;
            first_time_flash = true;
        }
        display_goal_reached();
        break;
    case DISPLAY_STOPWATCH:
        stopwatch_display(false);
        break;

    default:

        break;
    }
}

/**
 * @brief Display the screen showing the steps count
 *
 */
void display_steps(void) {
    display_line("", 0, ALIGN_CENTRE); // Clear the top line
    if (display_info_get_units() == UNITS_SI) {
        display_value("", "steps", display_info_get_steps(), 1, ALIGN_CENTRE, false);
    } else {
        display_value("", "% of goal", display_info_get_steps() * 100 / display_info_get_goal(), 1, ALIGN_CENTRE, false);
    }
    uint32_t workout_time = 0;
    uint32_t workout_start_time = display_info_get_start();
    if (workout_start_time != 0) {
        workout_time = display_info_get_ds() - workout_start_time;
    }

    display_time("Time:", workout_time * DS_TO_S, 2, ALIGN_CENTRE);
    display_line("", 3, ALIGN_CENTRE);
    display_line("", 3, ALIGN_CENTRE);
}

/**
 * @brief Display the screen showing the distance traveled
 *
 */
void display_distance(void) {
    uint32_t workout_time = 0;
    uint32_t workout_start_time = display_info_get_start();
    if (workout_start_time != 0) {
        workout_time = display_info_get_ds() - workout_start_time;
    }

    display_time("Time:", workout_time * DS_TO_S, 1, ALIGN_CENTRE);
    uint32_t mTravelled = display_info_get_steps() * M_PER_STEP(display_info_get_height());
    uint32_t calories = M_KG_TO_CAL(mTravelled,display_info_get_weight());

    // Protection against division by zero
    uint16_t speed;
    if (workout_time != 0) {
        speed = (mTravelled / (workout_time * DS_TO_S)) * MS_TO_KMH; // in km/h
    } else {
        speed = mTravelled * MS_TO_KMH; // if zero seconds elapsed, act as if it's at least one
    }

    if (display_info_get_units() == UNITS_SI) {
        display_value("Dist:", "km", mTravelled, 0, ALIGN_CENTRE, true);
        display_value("Speed", "kph", speed, 2, ALIGN_CENTRE, false);
        display_value("Energy:","kJ", CAL_TO_KJ(calories), 3, ALIGN_CENTRE, false);

    } else {
        display_value("Dist:", "mi", mTravelled * KM_TO_MILES, 0, ALIGN_CENTRE, true);
        display_value("Speed:", "mph", speed * KM_TO_MILES, 2, ALIGN_CENTRE, false);
        display_value("Energy:","Cal", calories, 3, ALIGN_CENTRE, false);
    }

    display_line("", 3, ALIGN_CENTRE);
}


/**
 * @brief display the set goal screen
 */
void display_set_goal(void) {
    uint32_t new_goal = GOAL_DEFAULT;
    uint32_t adc_value = pot_get();
    if (adc_value != 0) {
        new_goal = adc_value * GOAL_POT_SCALE_COEFF;
        new_goal = (new_goal / STEP_GOAL_ROUNDING) * STEP_GOAL_ROUNDING;
    }


    if (display_info_get_input_flag(MSG_DOWN_BUTTON_P) && !display_info_get_debug()) {
        display_info_set_goal(new_goal);
        // g_display_mode = DISPLAY_STEPS;
    }

    display_line("Set goal:", 0, ALIGN_CENTRE);
    display_value("Current:", "", display_info_get_goal(), 2, ALIGN_CENTRE, false);

    // Display the step/distance preview
    char toDraw[DISPLAY_WIDTH + 1]; // Must be one character longer to account for EOFs
    uint16_t distance = new_goal * M_PER_STEP(display_info_get_height()); // ===== NEEDS to be updated to new goal
    if (display_info_get_units() != UNITS_SI) {
        distance = distance * KM_TO_MILES;
    }

    // if <10 km/miles, use a decimal point. Otherwise display whole units (to save space)
    if (distance < 10 * 1000) {
        usnprintf(toDraw, DISPLAY_WIDTH + 1, "%d stps/%d.%01d%s", new_goal, distance / 1000, (distance % 1000) / 100, display_info_get_units() == UNITS_SI ? "km" : "mi");
    } else {
        usnprintf(toDraw, DISPLAY_WIDTH + 1, "%d stps/%d%s", new_goal, distance / 1000, display_info_get_units() == UNITS_SI ? "km" : "mi");
    }

    display_line(toDraw, 1, ALIGN_CENTRE);
    display_line("", 3, ALIGN_CENTRE);

}

/**
 * @brief display the set user parameters screen
 */
void display_set_user_parameters(void) {
    uint32_t new_weight = WEIGHT_DEFAULT;
    uint32_t new_height = HEIGHT_DEFAULT;
    uint32_t adc_value = pot_get();

    if (adc_value != 0) {
        if (display_info_get_toggle()) {
            new_weight = adc_value * WEIGHT_POT_SCALE_COEFF;
            new_height = display_info_get_height();
            // new_weight = (new_weight / STEP_GOAL_ROUNDING) * STEP_GOAL_ROUNDING;
        } else if (!display_info_get_toggle()) {
            new_height = adc_value * HEIGHT_POT_SCALE_COEFF;
            new_weight = display_info_get_weight();
            // new_height = (new_weight / STEP_GOAL_ROUNDING) * STEP_GOAL_ROUNDING;
        }
    }


    if (display_info_get_input_flag(MSG_DOWN_BUTTON_P) && !display_info_get_debug() && display_info_get_toggle()) {
        display_info_set_weight(new_weight);
    } else if (display_info_get_input_flag(MSG_DOWN_BUTTON_P) && !display_info_get_debug() && !display_info_get_toggle()) {
        display_info_set_height(new_height);
    }

    display_line("Weight",0,ALIGN_CENTRE);
    display_line("Height",2,ALIGN_CENTRE);

    char toDraw[DISPLAY_WIDTH + 1]; // Must be one character longer to account for EOFs
    uint16_t weight = new_weight;
    uint16_t current_weight = display_info_get_weight();
    if (display_info_get_units() != UNITS_SI) {
        weight *= KG_TO_LB;
        current_weight *= KG_TO_LB;
    }
    if (display_info_get_toggle()) {
        usnprintf(toDraw, DISPLAY_WIDTH + 1, "%d %s <- %d %s", current_weight, display_info_get_units() == UNITS_SI ? "kg" : "lb", weight, display_info_get_units() == UNITS_SI ? "kg" : "lb");
    } else {
        usnprintf(toDraw, DISPLAY_WIDTH + 1, "%d %s", current_weight, display_info_get_units() == UNITS_SI ? "kg" : "lb");
    }
    display_line(toDraw, 1, ALIGN_LEFT);

    uint16_t height = new_height;
    uint16_t current_height = display_info_get_height();
    if (display_info_get_units() != UNITS_SI) {
        height = CM_TO_INCHES(height);
        current_height = CM_TO_INCHES(current_height);
    }
    if (!display_info_get_toggle()) {
        usnprintf(toDraw, DISPLAY_WIDTH + 1, "%d %s <- %d %s", current_height, display_info_get_units() == UNITS_SI ? "cm" : "in", height, display_info_get_units() == UNITS_SI ? "cm" : "in");    
    } else {
        usnprintf(toDraw, DISPLAY_WIDTH + 1, "%d %s", current_height, display_info_get_units() == UNITS_SI ? "cm" : "in");    
    }
    display_line(toDraw, 3, ALIGN_LEFT);
}

/**
 * @brief Display the reset message
 *
 */
void display_reset_screen(void) {
    char* emptyLine = "                ";
    display_line(emptyLine, 0, ALIGN_LEFT);
    display_line("RESET", 1, ALIGN_CENTRE);
    display_line(emptyLine, 2, ALIGN_LEFT);
    display_line(emptyLine, 3, ALIGN_LEFT);
}

/**
 * @brief Display the goal reached message
 *
 */
void display_goal_reached(void) {
    char* emptyLine = "                ";
    display_line(emptyLine, 0, ALIGN_LEFT);
    display_line("GOAL REACHED!", 1, ALIGN_CENTRE);
    display_line(emptyLine, 2, ALIGN_LEFT);
    display_line(emptyLine, 3, ALIGN_LEFT);
}