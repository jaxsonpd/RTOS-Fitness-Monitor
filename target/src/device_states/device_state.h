/** 
 * @file device_state.h
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Defines the device state interface
 */

#ifndef DEVICE_STATE_MANAGER_H_
#define DEVICE_STATE_MANAGER_H_

#include "state.h"

typedef struct {
    state_t* currentState;
} device_state_t;

void device_state_init(device_state_t* ds, state_t* initialState);
void device_state_set(device_state_t* ds, state_t* newState);
void device_state_execute(device_state_t* ds);

#endif // DEVICE_STATE_MANAGER_H_