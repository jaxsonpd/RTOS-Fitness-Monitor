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

#define TICKS_TO_DS 1/100 ///< To convert from ticks to deci-seconds divide by 100

bool g_input_flags[NUM_MSGS] = { 0 };
bool g_alternate_mode = false;
bool g_debug_mode = false;
uint32_t g_last_step_time = 0;
uint32_t g_workout_start_time = 0;
units_t g_units = UNITS_SI;

uint32_t device_info_get_ds(void) {
    return (uint32_t)(xTaskGetTickCount()*TICKS_TO_DS);
}

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

void device_info_set_units(units_t units) {
    g_units = units;
}

units_t device_info_get_units(void) {
    return g_units;
}

void device_info_set_last_step_time(uint32_t time) {
    g_last_step_time = time;
}

uint32_t device_info_get_last_step_time(void) {
    return g_last_step_time;
}

void device_info_set_workout_start_time(uint32_t time) {
    g_workout_start_time = time;
}

uint32_t device_info_get_workout_start_time(void) {
    return g_workout_start_time;
}
