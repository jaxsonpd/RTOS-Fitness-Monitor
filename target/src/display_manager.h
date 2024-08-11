/*
 * display_manager.h
 *
 * Handles drawing to the device's screen, given its current state
 *
 *  Created on: 23/03/2022
 *      Author: Matthew Suter
 *
 *  FitnessThur9-1
 */

#ifndef DISPLAY_MANAGER_H_
#define DISPLAY_MANAGER_H_

#include "input_comms.h"
#include "fitness_monitor_main.h"

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


void displayInit(void); // Initalize the display

/**
 * @brief Update the device state based on the message
 * this function will be converted to work on the interal static device state
 * that will be implemented in display_manager.c in the future.
 * @param msg the comms message
 * @param deviceStateInfo the device state struct pointer
 */
void display_update_state(inputCommMsg_t msg, deviceStateInfo_t* deviceStateInfo);

void displayUpdate(deviceStateInfo_t deviceState, uint16_t secondsElapsed); // Update the display, given the device's state and the length of the current workout

#define DISPLAY_WIDTH 16

#endif /* DISPLAY_MANAGER_H_ */
