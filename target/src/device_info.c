/**
 * @file device_info.c
 * @author Jack Duignan (Jdu80@uclive.ac.nz), Isaac Cone (ico29@uclive.ac.nz)
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

#define DEFAULT_INPUT_FLAG 0
#define DEFAULT_ALTERNATE_MODE false
#define DEFAULT_DEBUG_MODE false
#define DEFAULT_LAST_STEP_TIME 0
#define DEFAULT_WORKOUT_START_TIME 0
#define DEFAULT_UNITS UNITS_SI

bool g_input_flags[NUM_MSGS] = { DEFAULT_INPUT_FLAG };          ///< array of input flags from input comms
bool g_alternate_mode = DEFAULT_ALTERNATE_MODE;                 ///< holds generic display alternate mode value to toggle alternate behaviour
bool g_debug_mode = DEFAULT_DEBUG_MODE;                         ///< holds generic debug mode to toggle debug behaviour
uint32_t g_last_step_time = DEFAULT_LAST_STEP_TIME;             ///< holds the users last step time
uint32_t g_workout_start_time = DEFAULT_WORKOUT_START_TIME;     ///< holds the workout start time
units_t g_units = DEFAULT_UNITS;                                ///< holds the mode of units for user displaying

bool device_info_reset(void) {
    device_info_clear_input_flags();
    g_alternate_mode = DEFAULT_ALTERNATE_MODE;
    g_debug_mode = DEFAULT_DEBUG_MODE;
    g_last_step_time = device_info_get_ds();
    g_workout_start_time = DEFAULT_WORKOUT_START_TIME;
    g_units = DEFAULT_UNITS;
    return true;
}

uint32_t device_info_get_ds(void) {
    return (uint32_t)(xTaskGetTickCount() * TICKS_TO_DS);
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
