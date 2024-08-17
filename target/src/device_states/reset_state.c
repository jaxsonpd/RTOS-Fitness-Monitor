/** 
 * @file reset_state.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Implementation of state for setting user parameters
 */

#include <stdint.h>

#include "../hal/display_hal.h"
#include "reset_state.h"

uint32_t g_reset_count = 0;

void resetState_enter(void) {
    display_line("8reset", 0, ALIGN_CENTRE);
}

char resetState_execute(void* args) {
    if (g_reset_count++ < 1000) {
        return 0x02;
    } else {
        return 0x01;
    }
}

void resetState_exit(void) {
    g_reset_count = 0;
    display_clear();
}

state_t resetState = { resetState_enter,resetState_execute,resetState_exit };