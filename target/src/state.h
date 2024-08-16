/** 
 * @file state.h
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Defines the state interface as per the state design pattern
 */

#ifndef STATE_H_
#define STATE_H_

#include <stdint.h>

typedef struct State {
    void (*Enter)(void);
    void (*Execute)(void);
    void (*Exit)(void);
} state_t;

#endif // STATE_H_