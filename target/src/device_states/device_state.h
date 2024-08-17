/** 
 * @file device_state.h
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Defines the device state interface
 */

#ifndef DEVICE_STATE_H_
#define DEVICE_STATE_H_

#include <stdbool.h>
#include "state.h"

typedef enum stateID_e {
    STEPS_STATE_ID = 0,
    DISTANCE_STATE_ID,
    GOAL_STATE_ID,
    STOPWATCH_STATE_ID,
    MOVE_PROMPT_STATE_ID,
    USER_PARAMETERS_STATE_ID,
    GOAL_REACHED_STATE_ID,
    MOVE_PROMPT_ALERT_STATE_ID,
    RESET_STATE_ID,
    STATE_COUNT,
} stateID_t;

typedef enum stateStatus_e {
    STATE_SUCCESS = 0x00,
    STATE_FINISHED,
    STATE_FLASHING,
} stateStatus_t;

typedef struct {
    stateID_t currentID;
    state_t* currentState;
} device_state_t;

extern state_t* states[STATE_COUNT];

bool device_state_init(device_state_t* ds, stateID_t initialID);
void device_state_set(device_state_t* ds, stateID_t newID);
char device_state_execute(device_state_t* ds, void* args);

#endif // DEVICE_STATE_H_