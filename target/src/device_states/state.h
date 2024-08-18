/** 
 * @file state.h
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Defines the state interface as per the state design pattern
 */

#ifndef STATE_H_
#define STATE_H_

#include <stdint.h>

/// State execute return codes for advanced behaviour
typedef enum stateStatus_e {
    STATE_SUCCESS = 0x00,   ///< State execute success, no advanced behaviour needed
    STATE_FINISHED,         ///< STATE_FLASHING state requests to exit
    STATE_FLASHING,         ///< State blocking input-based state changes until STATE_FINISHED
} stateStatus_t;

/**
 * @struct state_t
 * Structure to define the interface for a state to be entered, executed, and exited
 * Allows for reset
 */
typedef struct State {
    void (*Enter)(void);                        ///< Function to enter state, called once on state entry and sets up state behavior
    stateStatus_t (*Execute)(void* stateArgs);  ///< Function to execute state behavior, called periodically if state is active. Accepts arguments as a pointer
    void (*Exit)(void);                         ///< Function to exit state, called once on state exit. Does not have to clear static variables
    void (*Reset)(void);                        ///< Function to reset state to intial state, called when required
} state_t;

#endif // STATE_H_