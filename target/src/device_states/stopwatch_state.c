/** 
 * @file stopwatch_state.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Implementation of state for stopwatch
 */

#include <stdint.h>

#include "../hal/display_hal.h"
#include "../device_info.h"

#include "stopwatch_state.h"

/// States of the stopwatch
enum stopwatch_states {
    ZEROED,     ///< Cleared ready for next time
    RUNNING,    ///< Running timer
    PAUSED,     /// Stopped timer ready to be zeroed
    NUM_STATES
};

uint32_t last_stops[2] = { 0 };
enum stopwatch_states state = ZEROED;
bool first_run = false;

/**
 * @brief Update the stopwatch_state_machine
 * @param first_run true if the first run
 * @param state the state that the machine is in
 *
 * @return the run time of the stopwatch
 */
static uint32_t stopwatch_state_machine(bool first_run, enum stopwatch_states state) {
    static uint32_t start_time = 0;
    static uint32_t stop_time = 0;
    uint32_t run_time = 0;

    switch (state) {
    case (ZEROED):
        if (first_run) {
            last_stops[1] = last_stops[0];
            last_stops[0] = (stop_time - start_time);
        }

        run_time = 0;

        break;

    case (RUNNING):
        if (first_run) {
            start_time = device_info_get_ds();
        }

        run_time = (device_info_get_ds() - start_time);
        break;

    case (PAUSED):
        if (first_run) {
            stop_time = device_info_get_ds();
        }

        run_time = (stop_time - start_time);

        break;

    default:

        break;

    }

    return run_time;
}

void stopwatch_state_enter(void) {
    display_line("Stopwatch", 0, ALIGN_CENTRE);
}

stateStatus_t stopwatch_state_execute(void* args) {
    if (device_info_get_input_flag(MSG_DOWN_BUTTON_P) && !device_info_get_debug()) {
        state++;

        if (state >= NUM_STATES) {
            state = 0;
        }

        first_run = true;
    }

    uint32_t run_time = stopwatch_state_machine(first_run, state);
    first_run = false;

    display_line("Stopwatch", 0, ALIGN_CENTRE);
    display_time_ds("Elapsed: ", run_time, 1, ALIGN_CENTRE);
    display_time_ds("Stop-1: ", last_stops[0], 2, ALIGN_CENTRE);
    display_time_ds("Stop-2: ", last_stops[1], 3, ALIGN_CENTRE);

    return STATE_SUCCESS;
}

void stopwatch_state_exit(void) {
    display_clear();
}

void stopwatch_state_reset(void) {
    last_stops[0] = 0;
    last_stops[1] = 0;
    state = ZEROED;
    first_run = false;
}

state_t stopwatchState = { stopwatch_state_enter,stopwatch_state_execute,stopwatch_state_exit,stopwatch_state_reset };