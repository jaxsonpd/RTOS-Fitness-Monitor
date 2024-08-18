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

/// IDs of possible states defined in their own .h files
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

/**
 * @struct deviceState_t
 *  A structure to hold the current state and corresponding ID for a device
 *  
 */
typedef struct {
    stateID_t currentID;        ///< stateID of current state
    state_t* currentState;      ///< current state
} deviceState_t;

/// Array of all possible states for the device. States are defined in their own .h files
extern state_t* states[STATE_COUNT];

/** 
 * @brief Initialise deviceState_t instance to start at the specified initial ID.
 * Checks for invalid parameters provided.
 * @param ds pointer to a deviceState_t instance to be initialised
 * @param initialID changeable id to intialise the device to
 * 
 * @return true if succesful
 */
bool device_state_init(deviceState_t* ds, stateID_t initialID);

/** 
 * @brief Set device to the state with the defined new id
 * Checks for invalid parameters provided.
 * @param ds pointer to a deviceState_t instance to be initialised
 * @param newID id for the state the device will be set to
 * 
 * @return true if succesful
 */

bool device_state_set(deviceState_t* ds, stateID_t newID);

/** 
 * @brief Execute the behavior defined by the current state
 * 
 * @param ds device holding state to be executed
 * @param args generic pointer to arguments passed in to the state
 * 
 * @return status code from execution to allow for advanced behaviour
 */
stateStatus_t device_state_execute(deviceState_t* ds, void* args);

/** 
 * @brief Reset the device state to the initial value
 * 
 * @param ds the device to be reset
 * 
 * @return true if reset successful
 */
bool device_state_reset(deviceState_t* ds);

#endif // DEVICE_STATE_H_