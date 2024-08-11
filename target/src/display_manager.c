/** 
 * @file display_manager.c
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-08-11
 * @brief Definitions for the display manager thread which write information to 
 * the OLED screen.
 */



#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/debug.h"
#include "driverlib/pin_map.h"
#include "utils/ustdlib.h"
#include "stdio.h"
#include "stdlib.h"
#include "OrbitOLED/OrbitOLEDInterface.h"
#include "utils/ustdlib.h"

#include "FreeRTOS.h"
#include "task.h"

#include "input_comms.h"
#include "serial_sender.h"

#include "fitness_monitor_main.h"
#include "display_manager.h"


//********************************************************
// Constants and static vars
//********************************************************

#define KM_TO_MILES 62/100 // Multiply by 0.6215 to convert, this should be good enough
#define MS_TO_KMH 36/10
#define TIME_UNIT_SCALE 60
#define DISPLAY_WIDTH 16

typedef enum {
    ALIGN_LEFT = 0,
    ALIGN_CENTRE,
    ALIGN_RIGHT,
} textAlignment_t;

typedef struct {
    uint32_t stepsTaken;
    uint32_t currentGoal;
    uint32_t newGoal;
    uint16_t secondsElapsed;
    displayMode_t displayMode;
} stepsInfo_t;

/*******************************************
 *      Local prototypes
 *******************************************/
static void display_line(char* inStr, uint8_t row, textAlignment_t alignment);
static void display_value(char* prefix, char* suffix, int32_t value, uint8_t row, textAlignment_t alignment, bool thousandsFormatting);
static void display_time(char* prefix, uint16_t time, uint8_t row, textAlignment_t alignment);


/*******************************************
 *      Global functions
 *******************************************/
void display_manager_thread(void* rtos_param) {
    display_manager_init();
    uint32_t seconds_elapsed = 0;

    for (;;) {
        uint8_t num_tries = 0;
        while (input_comms_num_msgs() > 0 && num_tries < 5) {
            inputCommMsg_t msg = input_comms_receive();

            display_update_state(msg, &deviceState);

            num_tries++;
        }

        seconds_elapsed = xTaskGetTickCount() / 1000;
        display_update(deviceState, seconds_elapsed);
    }
}

// Init the screen library
void display_manager_init(void) {
    OLEDInitialise();
}


#define LONG_PRESS_CYCLES 20
/**
 * @brief Update the device state based on the message
 * this function will be converted to work on the interal static device state
 * that will be implemented in display_manager.c in the future.
 * @param msg the comms message
 * @param deviceStateInfo the device state struct pointer
 */
void display_update_state(inputCommMsg_t msg, deviceStateInfo_t* deviceStateInfo) {
    displayMode_t currentDisplayMode = deviceStateInfo->displayMode;
    static bool allowLongPress = true;
    static uint16_t longPressCount = 0;

    switch (msg) {
    case (MSG_SCREEN_LEFT):
        deviceStateInfo->displayMode = (deviceStateInfo->displayMode + 1) % DISPLAY_NUM_STATES; // flicker when pressing button
        break;

    case (MSG_SCREEN_RIGHT):
        if (deviceStateInfo->displayMode > 0) {
            deviceStateInfo->displayMode--;
        } else {
            deviceStateInfo->displayMode = DISPLAY_NUM_STATES - 1;
        }
        break;

    case (MSG_LEFT_SWITCH_ON):
    case (MSG_RIGHT_SWITCH_ON):
        deviceStateInfo->debugMode = true;
        break;

    case (MSG_LEFT_SWITCH_OFF):
    case (MSG_RIGHT_SWITCH_OFF):
        deviceStateInfo->debugMode = false;
        break;

    case (MSG_UP_BUTTON_P):
        if (deviceStateInfo->debugMode) {
            deviceStateInfo->stepsTaken = deviceStateInfo->stepsTaken + DEBUG_STEP_INCREMENT;
        } else {
            if (deviceStateInfo->displayUnits == UNITS_SI) {
                deviceStateInfo->displayUnits = UNITS_ALTERNATE;
            } else {
                deviceStateInfo->displayUnits = UNITS_SI;
            }
        }
        break;

    case (MSG_DOWN_BUTTON_P):
        if (deviceStateInfo->debugMode) {
            if (deviceStateInfo->stepsTaken >= DEBUG_STEP_DECREMENT) {
                deviceStateInfo->stepsTaken = deviceStateInfo->stepsTaken - DEBUG_STEP_DECREMENT;
            } else {
                deviceStateInfo->stepsTaken = 0;
            }
        } else {
            if ((currentDisplayMode != DISPLAY_SET_GOAL) && (allowLongPress)) {
                longPressCount++;
                if (longPressCount >= LONG_PRESS_CYCLES) {
                    deviceStateInfo->stepsTaken = 0;
                    // flashMessage("Reset!");
                }
            } else {
                if ((currentDisplayMode == DISPLAY_SET_GOAL)) {
                    deviceStateInfo->currentGoal = deviceStateInfo->newGoal;
                    deviceStateInfo->displayMode = DISPLAY_STEPS;

                    allowLongPress = false; // Hacky solution: Protection against double-registering as a short press then a long press
                }
                longPressCount = 0;
            }

        }
        break;

    case (MSG_DOWN_BUTTON_R):
        allowLongPress = true;
        break;

    default:

        break;
    }


}



// Update the display, called on a loop
void display_update(deviceStateInfo_t deviceState, uint16_t secondsElapsed) {
    // Check for flash message override
    if (deviceState.flashTicksLeft != 0) {
        char* emptyLine = "                ";
        OLEDStringDraw(emptyLine, 0, 0);
        display_line(deviceState.flashMessage, 1, ALIGN_CENTRE);
        OLEDStringDraw(emptyLine, 0, 2);
        OLEDStringDraw(emptyLine, 0, 3);
        return;
    }


    uint32_t mTravelled = 0; // TODO: If I put this inside the case statement it won't compile. Work out why!

    switch (deviceState.displayMode) {
    case DISPLAY_STEPS:
        display_line("", 0, ALIGN_CENTRE); // Clear the top line
        if (deviceState.displayUnits == UNITS_SI) {
            display_value("", "steps", deviceState.stepsTaken, 1, ALIGN_CENTRE, false);
        } else {
            display_value("", "% of goal", deviceState.stepsTaken * 100 / deviceState.currentGoal, 1, ALIGN_CENTRE, false);
        }
        display_time("Time:", secondsElapsed, 2, ALIGN_CENTRE);
        break;
    case DISPLAY_DISTANCE:
        display_time("Time:", secondsElapsed, 1, ALIGN_CENTRE);
        mTravelled = deviceState.stepsTaken * M_PER_STEP;

        // Protection against division by zero
        uint16_t speed;
        if (secondsElapsed != 0) {
            speed = (mTravelled / secondsElapsed) * MS_TO_KMH; // in km/h
        } else {
            speed = mTravelled * MS_TO_KMH; // if zero seconds elapsed, act as if it's at least one
        }

        if (deviceState.displayUnits == UNITS_SI) {
            display_value("Dist:", "km", mTravelled, 0, ALIGN_CENTRE, true);
            display_value("Speed", "kph", speed, 2, ALIGN_CENTRE, false);
        } else {
            display_value("Dist:", "mi", mTravelled * KM_TO_MILES, 0, ALIGN_CENTRE, true);
            display_value("Speed", "mph", speed * KM_TO_MILES, 2, ALIGN_CENTRE, false);
        }

        break;
    case DISPLAY_SET_GOAL:
        display_line("Set goal:", 0, ALIGN_CENTRE);
        display_value("Current:", "", deviceState.currentGoal, 2, ALIGN_CENTRE, false);

        // Display the step/distance preview
        char toDraw[DISPLAY_WIDTH + 1]; // Must be one character longer to account for EOFs
        uint16_t distance = deviceState.newGoal * M_PER_STEP;
        if (deviceState.displayUnits != UNITS_SI) {
            distance = distance * KM_TO_MILES;
        }

        // if <10 km/miles, use a decimal point. Otherwise display whole units (to save space)
        if (distance < 10 * 1000) {
            usnprintf(toDraw, DISPLAY_WIDTH + 1, "%d stps/%d.%01d%s", deviceState.newGoal, distance / 1000, (distance % 1000) / 100, deviceState.displayUnits == UNITS_SI ? "km" : "mi");
        } else {
            usnprintf(toDraw, DISPLAY_WIDTH + 1, "%d stps/%d%s", deviceState.newGoal, distance / 1000, deviceState.displayUnits == UNITS_SI ? "km" : "mi");
        }

        display_line(toDraw, 1, ALIGN_CENTRE);

        break;
    }
}


/** 
 * @brief Draw a line to the OLED display
 * @param inStr the string to display
 * @param row the row to display on
 * @param alignment the alignment of the text
 * 
 */
static void display_line(char* inStr, uint8_t row, textAlignment_t alignment) {
    // Get the length of the string, but prevent it from being more than 16 chars long
    uint8_t inStrLength = 0;
    while (inStr[inStrLength] != '\0' && inStrLength < DISPLAY_WIDTH) {
        inStrLength++;
    }

    // Create a 16-char long array to write to
    uint8_t i = 0;
    char toDraw[DISPLAY_WIDTH + 1]; // Must be one character longer to account for EOFs
    for (i = 0; i < DISPLAY_WIDTH; i++) {
        toDraw[i] = ' ';
    }
    toDraw[DISPLAY_WIDTH] = '\0'; // Set the last character to EOF

    // Set the starting position based on the alignment specified
    uint8_t startPos = 0;
    switch (alignment) {
    case ALIGN_LEFT:
        startPos = 0;
        break;
    case ALIGN_CENTRE:
        startPos = (DISPLAY_WIDTH - inStrLength) / 2;
        break;
    case ALIGN_RIGHT:
        startPos = (DISPLAY_WIDTH - inStrLength);
        break;
    }

    // Copy the string we were given onto the 16-char row
    for (i = 0; i < inStrLength; i++) {
        toDraw[i + startPos] = inStr[i];
    }

    OLEDStringDraw(toDraw, 0, row);
}



/** 
 * @brief Display a value with a prefix and a suffix with the ability to 
 * display 1000s floats
 * @param prefix text to place before value
 * @param suffix test to place after the value
 * @param row the row to place the text on
 * @param alignment the alignment of the text
 * @param formateThousands wether to display as a float
 * 
 */
static void display_value(char* prefix, char* suffix, int32_t value, uint8_t row, textAlignment_t alignment, bool thousandsFormatting) {
    char toDraw[DISPLAY_WIDTH + 1]; // Must be one character longer to account for EOFs

    if (thousandsFormatting) {
        // Print a number/1000 to 3dp, with decimal point and sign
        // Use a mega cool ternary operator to decide whether to use a minus sign
        usnprintf(toDraw, DISPLAY_WIDTH + 1, "%s%c%d.%03d %s", prefix, value < 0 ? '-' : ' ', abs(value / 1000), abs(value) % 1000, suffix);
    } else {
        usnprintf(toDraw, DISPLAY_WIDTH + 1, "%s %d %s", prefix, value, suffix); // Can use %4d if we want uniform spacing
    }

    display_line(toDraw, row, alignment);
}

/** 
 * @brief Display a given number of seconds as ether mm:ss or hh:mm:ss
 * @param prefix Text to display before the time
 * @param time The time to display in seconds
 * @param row The row to display on
 * @param alignment how to align the text
 * 
 */
static void display_time(char* prefix, uint16_t time, uint8_t row, textAlignment_t alignment) {
    char toDraw[DISPLAY_WIDTH + 1]; // Must be one character longer to account for EOFs
    uint16_t minutes = (time / TIME_UNIT_SCALE) % TIME_UNIT_SCALE;
    uint16_t seconds = time % TIME_UNIT_SCALE;
    uint16_t hours = time / (TIME_UNIT_SCALE * TIME_UNIT_SCALE);

    if (hours == 0) {
        usnprintf(toDraw, DISPLAY_WIDTH + 1, "%s %01d:%02d", prefix, minutes, seconds);
    } else {
        usnprintf(toDraw, DISPLAY_WIDTH + 1, "%s %01d:%02d:%02d", prefix, hours, minutes, seconds);
    }

    display_line(toDraw, row, alignment);
}

