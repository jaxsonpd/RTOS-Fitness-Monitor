/** 
 * @file device_state.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Defines the behaviour of the display state interface
 */

#include <stdlib.h>

#include "state.h"
#include "device_state.h"

bool device_state_init(device_state_t* ds, state_t* initialState) 
{
    if (ds == NULL) {
        return 0;
    }
    if (initialState == NULL) {
        return 0;
    }
    ds->currentState = initialState;
    ds->currentState->Enter();
    return 1;
}
void device_state_set(device_state_t* ds, state_t* newState)
{
    if (ds->currentState != NULL) {
        ds->currentState->Exit();
    }
    ds->currentState = newState;
    ds->currentState->Enter();
}

void device_state_execute(device_state_t* ds, void* args)
{
    if (ds->currentState != NULL) {
        ds->currentState->Execute(args);
    }
    // Push to message queue?
}
