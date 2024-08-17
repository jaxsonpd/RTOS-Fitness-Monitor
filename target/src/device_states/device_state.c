/** 
 * @file device_state.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Defines the behaviour of the display state interface
 */

#include <stdlib.h>

#include "state.h"
#include "device_state.h"

#include "steps_state.h"
#include "distance_state.h"
#include "goal_state.h"
#include "move_prompt_state.h"
#include "stopwatch_state.h"
#include "user_parameters_state.h"
#include "goal_reached_state.h"
#include "move_prompt_alert_state.h"
#include "reset_state.h"

state_t* states[STATE_COUNT] = { &stepsState, &distanceState, &goalState, &stopwatchState, &movePromptState, &userParametersState, &goalReachedState, &movePromptAlertState, &resetState };

bool device_state_init(device_state_t* ds, stateID_t initialID) 
{
    if (ds == NULL) {
        return 0;
    }
    if (states[initialID] == NULL) {
        return 0;
    }
    ds->currentState = states[initialID];
    ds->currentState->Enter();
    return 1;
}
void device_state_set(device_state_t* ds, stateID_t newID)
{
    if (ds->currentState != NULL) {
        ds->currentState->Exit();
    }
    ds->currentID = newID;
    ds->currentState = states[newID];
    ds->currentState->Enter();
}

char device_state_execute(device_state_t* ds, void* args)
{
    char status;
    if (ds->currentState != NULL) {
        status = ds->currentState->Execute(args);
    }

    switch (status) {
    case STATE_SUCCESS:
        break;

    case STATE_FINISHED:
    // End current flashing state
        device_state_set(ds, STEPS_STATE_ID);
        break;

    case STATE_FLASHING:
    // Block updates for flashing state
        
    default:
        break;
    }
    return status;
}
