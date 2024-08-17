/** 
 * @file state.h
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Defines the state interface as per the state design pattern
 */

#ifndef STATE_H_
#define STATE_H_

#include <stdint.h>

typedef enum stateStatus_e {
    STATE_SUCCESS = 0x00,
    STATE_FINISHED,
    STATE_FLASHING,
} stateStatus_t;

typedef struct State {
    void (*Enter)(void);
    stateStatus_t (*Execute)(void* stateArgs);
    void (*Exit)(void);
} state_t;

#endif // STATE_H_