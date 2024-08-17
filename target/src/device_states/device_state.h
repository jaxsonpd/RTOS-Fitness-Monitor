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

typedef enum stateID {
    STEPS_STATE_ID = 0,
    DISTANCE_STATE_ID,
    GOAL_STATE_ID,
    STOPWATCH_STATE_ID,
    MOVE_PROMPT_STATE_ID,
    USER_PARAMETERS_STATE_ID,
    GOAL_REACHED_STATE_ID,
    MOVE_PROMPT_ALERT_STATE_ID,
    STATE_COUNT,
} state_id_t;

typedef struct {
    state_id_t currentID;
    state_t* currentState;
} device_state_t;

extern state_t* states[STATE_COUNT];

bool device_state_init(device_state_t* ds, state_id_t initialID);
void device_state_set(device_state_t* ds, state_id_t newID);
void device_state_execute(device_state_t* ds, void* args);

#endif // DEVICE_STATE_H_