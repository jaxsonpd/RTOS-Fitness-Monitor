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

typedef struct {
    state_t* currentState;
} device_state_t;

bool device_state_init(device_state_t* ds, state_t* initialState);
void device_state_set(device_state_t* ds, state_t* newState);
void device_state_execute(device_state_t* ds, void* args);

#endif // DEVICE_STATE_H_