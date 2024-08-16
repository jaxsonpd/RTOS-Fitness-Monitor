/** 
 * @file device_state.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Defines the behaviour of the display state interface
 */

#include <stdlib.h>

#include "state.h"
#include "device_state.h"

void device_state_init(device_state_t* ds, state_t* initialState) 
{
    ds->currentState = initialState;
    ds->currentState->Enter();
}
void device_state_set(device_state_t* ds, state_t* newState)
{
    if (ds->currentState != NULL) {
        ds->currentState->Exit();
    }
    ds->currentState = newState;
    ds->currentState->Enter();
}

void device_state_execute(device_state_t* ds)
{
    if (ds->currentState != NULL) {
        ds->currentState->Execute();
    }
    // Push to message queue
}
