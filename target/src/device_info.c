/**
 * @file device_info.c
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-08-12
 * @brief Definitions for the device info interface
 */


#include <stdint.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"

#include "device_info.h"
#include "comms/input_comms.h"

bool g_input_flags[NUM_MSGS] = { 0 };
bool g_alternate_mode = false;
bool g_debug_mode = false;

bool device_info_get_input_flag(inputCommMsg_t msg) {
    return g_input_flags[msg];
}

void device_info_set_input_flag(inputCommMsg_t msg, bool flag) {
    g_input_flags[msg] = flag;
}

void device_info_clear_input_flags(void) {
    for (uint8_t i = 0; i < NUM_MSGS; i++) {
        device_info_set_input_flag(i, 0);
    }
}

void device_info_set_alternate(bool status) {
    g_alternate_mode = status;
}

bool device_info_get_alternate(void) {
    return g_alternate_mode;
}

void device_info_set_debug(bool status) {
    g_debug_mode = status;
}

bool device_info_get_debug(void) {
    return g_debug_mode;
}