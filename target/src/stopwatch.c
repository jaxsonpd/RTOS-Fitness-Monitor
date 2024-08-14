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

void stopwatch_display(void) {
    static bool running = false;
    static uint32_t start_time = 0;
    static uint32_t stop_time = 0;

    if (display_info_get_input_flag(MSG_DOWN_BUTTON_P)) {
        running = !running;

        if (running) {
            last_stops[1] = last_stops[0];
            last_stops[0] = (stop_time - start_time)*DS_TO_S;
            start_time = display_info_get_ds();
        } else {
            stop_time = display_info_get_ds();
        }
    }

    uint32_t run_time  = 0;
    if (running) {
        run_time = (display_info_get_ds() - start_time)*DS_TO_S;
    } else {
        run_time = (stop_time - start_time)*DS_TO_S;
    }


    display_line("Stopwatch", 0, ALIGN_CENTRE);
    display_time("Elapsed: ", run_time, 1, ALIGN_CENTRE);
    display_time("Stop -1: ", last_stops[0], 2, ALIGN_CENTRE);
    display_time("Stop -2: ", last_stops[1], 3, ALIGN_CENTRE);
}


