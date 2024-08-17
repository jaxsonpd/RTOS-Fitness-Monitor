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

state_t* states[STATE_COUNT] = { &stepsState, &distanceState, &goalState, &stopwatchState, &movePromptState, &userParametersState, &goalReachedState, &movePromptAlertState };

bool device_state_init(device_state_t* ds, state_id_t initialID) 
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
void device_state_set(device_state_t* ds, state_id_t newID)
{
    if (ds->currentState != NULL) {
        ds->currentState->Exit();
    }
    ds->currentState = states[newID];
    ds->currentState->Enter();
}

void device_state_execute(device_state_t* ds, void* args)
{
    if (ds->currentState != NULL) {
        ds->currentState->Execute(args);
    }
    // Push to message queue?
}
