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

bool device_info_get_input_flag(inputCommMsg_t msg) {
    return g_input_flags[msg];
}

void device_info_set_input_flag(inputCommMsg_t msg, bool flag) {
    g_input_flags[msg] = flag;
}