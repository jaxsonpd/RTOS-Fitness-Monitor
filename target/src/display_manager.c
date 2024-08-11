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

#include "step_counter_comms.h"
#include "input_comms.h"
#include "serial_sender.h"
#include "display.h"

#include "display_manager.h"

#define LONG_PRESS_CYCLES 20

#define KM_TO_MILES 62/100 // Multiply by 0.6215 to convert, this should be good enough
#define MS_TO_KMH 36/10
#define M_PER_STEP 9 / 10

#define DEBUG_STEP_INCREMENT 100
#define DEBUG_STEP_DECREMENT 100

typedef enum
{
    DISPLAY_STEPS = 0,
    DISPLAY_DISTANCE,
    DISPLAY_SET_GOAL,
    DISPLAY_NUM_STATES, // Automatically enumerates to the number of display states there can be
} displayMode_t;

typedef enum
{
    UNITS_SI = 0,    // Steps  /km
    UNITS_ALTERNATE, // Percent/miles
    UNITS_NUM_TYPES,
} displayUnits_t;

inputCommMsg_t input_flags[NUM_MSGS] = {0};


void display_update_inputs(inputCommMsg_t msg);
void display_update(uint16_t secondsElapsed);
bool display_manager_init(void);

displayMode_t displayMode = DISPLAY_STEPS;
uint32_t currentGoal = 1000;
bool debugMode = false;
displayUnits_t displayUnits = UNITS_SI;
uint32_t stepsTaken = 0;
uint32_t newGoal = 3000;

void display_manager_thread(void* rtos_param) {
    display_manager_init();
    uint32_t seconds_elapsed = 0;

    for (;;) {
        uint8_t num_tries = 0;
        while (input_comms_num_msgs() > 0 && num_tries < 5) {
            inputCommMsg_t msg = input_comms_receive();

            input_flags[msg] = 1;
            display_update_inputs(msg);

            num_tries++;
        }

        stepsTaken += step_counter_get();
        seconds_elapsed = xTaskGetTickCount() / 1000;
        
        display_update(seconds_elapsed);

        for (uint8_t i = 0; i < NUM_MSGS; i++) {
            input_flags[i] = 0;
        }

        vTaskDelay(1000/5);
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
 * @brief Update the device state based on the message
 * this function will be converted to work on the interal static device state
 * that will be implemented in display_manager.c in the future.
 * @param msg the comms message
 * @param deviceStateInfo the device state struct pointer
 */
void display_update_inputs(inputCommMsg_t msg) {
    displayMode_t currentDisplayMode = displayMode;

    switch (msg) {
    case (MSG_SCREEN_LEFT):
        displayMode = (displayMode + 1) % DISPLAY_NUM_STATES; // flicker when pressing button
        break;

    case (MSG_SCREEN_RIGHT):
        if (displayMode > 0) {
            displayMode--;
        } else {
            displayMode = DISPLAY_NUM_STATES - 1;
        }
        break;

    case (MSG_LEFT_SWITCH_ON):
    case (MSG_RIGHT_SWITCH_ON):
        debugMode = true;
        break;

    case (MSG_LEFT_SWITCH_OFF):
    case (MSG_RIGHT_SWITCH_OFF):
        debugMode = false;
        break;

    case (MSG_UP_BUTTON_P):
        if (debugMode) {
            stepsTaken = stepsTaken + DEBUG_STEP_INCREMENT;
        } else {
            if (displayUnits == UNITS_SI) {
                displayUnits = UNITS_ALTERNATE;
            } else {
                displayUnits = UNITS_SI;
            }
        }
        break;

    case (MSG_DOWN_BUTTON_P):
        if (debugMode) {
            if (stepsTaken >= DEBUG_STEP_DECREMENT) {
                stepsTaken = stepsTaken - DEBUG_STEP_DECREMENT;
            } else {
                stepsTaken = 0;
            }
        } 
        break;

    default:

        break;
    }


}

/** 
 * @brief Display the screen showing the steps count
 * @param secondsElapsed the number of seconds elapsed since
 * the start of the workout
 * 
 */
void display_steps(uint16_t secondsElapsed) {
    display_line("", 0, ALIGN_CENTRE); // Clear the top line
    if (displayUnits == UNITS_SI) {
        display_value("", "steps", stepsTaken, 1, ALIGN_CENTRE, false);
    } else {
        display_value("", "% of goal", stepsTaken * 100 / currentGoal, 1, ALIGN_CENTRE, false);
    }
    display_time("Time:", secondsElapsed, 2, ALIGN_CENTRE);
}

/** 
 * @brief Display the screen showing the distance traveled
 * @param secondsElapsed the number of seconds elapsed since
 * the start of the workout
 * 
 */
void display_distance(uint16_t secondsElapsed) {
    uint32_t mTravelled = 0;

    display_time("Time:", secondsElapsed, 1, ALIGN_CENTRE);
    mTravelled = stepsTaken * M_PER_STEP;

    // Protection against division by zero
    uint16_t speed;
    if (secondsElapsed != 0) {
        speed = (mTravelled / secondsElapsed) * MS_TO_KMH; // in km/h
    } else {
        speed = mTravelled * MS_TO_KMH; // if zero seconds elapsed, act as if it's at least one
    }

    if (displayUnits == UNITS_SI) {
        display_value("Dist:", "km", mTravelled, 0, ALIGN_CENTRE, true);
        display_value("Speed", "kph", speed, 2, ALIGN_CENTRE, false);
    } else {
        display_value("Dist:", "mi", mTravelled * KM_TO_MILES, 0, ALIGN_CENTRE, true);
        display_value("Speed", "mph", speed * KM_TO_MILES, 2, ALIGN_CENTRE, false);
    }
}


/**
 * @brief display the set goal screen
 * @param secondsElapsed the number of seconds elapsed since
 * the start of the workout
 */
void display_set_goal(uint16_t secondsElapsed) {
    if (input_flags[MSG_DOWN_BUTTON_P] && !debugMode) {
        currentGoal = newGoal;
        displayMode = DISPLAY_STEPS;
    }
                    
    display_line("Set goal:", 0, ALIGN_CENTRE);
    display_value("Current:", "", currentGoal, 2, ALIGN_CENTRE, false);

    // Display the step/distance preview
    char toDraw[DISPLAY_WIDTH + 1]; // Must be one character longer to account for EOFs
    uint16_t distance = newGoal * M_PER_STEP; // ===== NEEDS to be updated to new goal
    if (displayUnits != UNITS_SI) {
        distance = distance * KM_TO_MILES;
    }

    // if <10 km/miles, use a decimal point. Otherwise display whole units (to save space)
    if (distance < 10 * 1000) {
        usnprintf(toDraw, DISPLAY_WIDTH + 1, "%d stps/%d.%01d%s", newGoal, distance / 1000, (distance % 1000) / 100, displayUnits == UNITS_SI ? "km" : "mi");
    } else {
        usnprintf(toDraw, DISPLAY_WIDTH + 1, "%d stps/%d%s", newGoal, distance / 1000, displayUnits == UNITS_SI ? "km" : "mi");
    }

    display_line(toDraw, 1, ALIGN_CENTRE);
}


/** 
 * @brief Update the display
 * @param secondsElapsed the number of seconds elapsed since
 * the start of the workout
 * 
 */
void display_update(uint16_t secondsElapsed) {
    // Check for flash message override
    // if (deviceState.flashTicksLeft != 0) {
    //     char* emptyLine = "                ";
    //     OLEDStringDraw(emptyLine, 0, 0);
    //     display_line(deviceState.flashMessage, 1, ALIGN_CENTRE);
    //     OLEDStringDraw(emptyLine, 0, 2);
    //     OLEDStringDraw(emptyLine, 0, 3);
    //     return;
    // }


    // uint32_t mTravelled = 0; // TODO: If I put this inside the case statement it won't compile. Work out why!

    switch (displayMode) {
    case DISPLAY_STEPS:
        display_steps(secondsElapsed);
        break;
    case DISPLAY_DISTANCE:
        display_distance(secondsElapsed);        

        break;
    case DISPLAY_SET_GOAL:
        display_set_goal(secondsElapsed);

        break;
    }
}
