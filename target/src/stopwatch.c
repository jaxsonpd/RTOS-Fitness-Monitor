/** 
 * @file stopwatch.c
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-08-14
 * @brief Implementation of the stopwatch screen
 */


#include <stdint.h>
#include <stdbool.h>

#include "display_info.h"
#include "display.h"

#include "stopwatch.h"

uint32_t last_stops[2] = {0};

enum stopwatch_states {
    ZEROED,
    RUNNING,
    PAUSED,
    NUM_STATES
};

void stopwatch_display(void) {
    static enum stopwatch_states states = ZEROED;
    static bool first_run = false;
    static uint32_t start_time = 0;
    static uint32_t stop_time = 0;

    if (display_info_get_input_flag(MSG_DOWN_BUTTON_P)) {
        states++;

        if (states >= NUM_STATES) {
            states = 0;
        } 

        first_run = true;
    }

    uint32_t run_time  = 0;
    switch (states) {
    case (ZEROED):
        if (first_run) {
            last_stops[1] = last_stops[0];
            last_stops[0] = (stop_time - start_time);

            first_run = false;
        }
        
        run_time = 0;

        break;
    
    case (RUNNING):
        if (first_run) {
            start_time = display_info_get_ds();

            first_run = false;
        }

        run_time = (display_info_get_ds() - start_time);
        break;

    case (PAUSED):
        if (first_run) {
            stop_time = display_info_get_ds();

            first_run = false;
        }

        run_time = (stop_time - start_time);

        break;

    default:
        
        break;
        
    }


    display_line("Stopwatch", 0, ALIGN_CENTRE);
    display_time_ds("Elapsed: ", run_time, 1, ALIGN_CENTRE);
    display_time_ds("Stop -1: ", last_stops[0], 2, ALIGN_CENTRE);
    display_time_ds("Stop -2: ", last_stops[1], 3, ALIGN_CENTRE);
}


