/**
 * @file device_state.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Defines the behaviour of the display state interface
 */

#include <stdlib.h>

#include "device_states/state.h"
#include "device_states/steps_state.h"
#include "device_states/distance_state.h"
#include "device_states/goal_state.h"
#include "device_states/move_prompt_state.h"
#include "device_states/stopwatch_state.h"
#include "device_states/user_parameters_state.h"
#include "device_states/goal_reached_state.h"
#include "device_states/move_prompt_alert_state.h"
#include "device_states/reset_state.h"

#include "device_state_handler.h"

state_t* states[STATE_COUNT] = { NULL };

bool device_state_init(deviceState_t* ds, stateID_t initialID) {
    // Base state order on enum position
    states[STEPS_STATE_ID] = &stepsState;
    states[DISTANCE_STATE_ID] = &distanceState;
    states[GOAL_STATE_ID] = &goalState;
    states[STOPWATCH_STATE_ID] = &stopwatchState;
    states[MOVE_PROMPT_STATE_ID] = &movePromptState;
    states[USER_PARAMETERS_STATE_ID] = &userParametersState;
    states[GOAL_REACHED_STATE_ID] = &goalReachedState;
    states[MOVE_PROMPT_ALERT_STATE_ID] = &movePromptAlertState;
    states[RESET_STATE_ID] = &resetState;

    if (ds == NULL) {
        return 0;
    }
    if (states[initialID] == NULL) {
        return 0;
    }

    for (uint8_t i = 0; i < STATE_COUNT; i++) {
        assert(states[i] != NULL);
    }

    ds->currentID = initialID;
    ds->currentState = states[initialID];
    ds->currentState->Enter();
    return 1;
}

bool device_state_set(deviceState_t* ds, stateID_t newID) {
    if (ds->currentState != NULL) {
        ds->currentState->Exit();
    }
    if (newID >= STATE_COUNT) {
        return 0;
    }
    ds->currentID = newID;
    ds->currentState = states[newID];
    ds->currentState->Enter();
    return 1;
}

stateStatus_t device_state_execute(deviceState_t* ds, void* args) {
    stateStatus_t status;
    if (ds->currentState != NULL) {
        status = ds->currentState->Execute(args);
    }

    switch (status) {
    case STATE_SUCCESS:
        break;

    case STATE_FINISHED: ;
    // End current flashing state
        bool success;
        if (ds->currentID == RESET_STATE_ID) {
            success = device_state_reset(ds);
        } else {
            success = device_state_set(ds, STEPS_STATE_ID);
        }
        assert(success);
        break;

    case STATE_FLASHING:
    // Block updates for flashing state

    default:
        break;
    }
    return status;
}

bool device_state_reset(deviceState_t* ds) {
    for (uint32_t i = 0; i < STATE_COUNT; i++) {
        states[i]->Reset();
    }
    return device_state_init(ds, 0);
}