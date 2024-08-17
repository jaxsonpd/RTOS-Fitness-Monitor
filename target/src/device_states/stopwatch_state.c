/** 
 * @file stopwatch_state.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Implementation of state for setting user parameters
 */

#include <stdint.h>

#include "../hal/display_hal.h"
#include "../stopwatch.h"
#include "stopwatch_state.h"

void stopwatchState_enter(void) {
    display_line("stopwatch", 0, ALIGN_CENTRE);
}

void stopwatchState_execute(void* args) {
    stopwatch_display(false);
}

void stopwatchState_exit(void) {
    display_clear();
}

state_t stopwatchState = { stopwatchState_enter,stopwatchState_execute,stopwatchState_exit };