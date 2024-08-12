/**
 * @file display_info.c
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-08-12
 * @brief Definitions for the display info interface
 */


#include <stdint.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"

#include "display_info.h"
#include "input_comms.h"

bool g_debug = false;
displayUnits_t g_units = UNITS_SI;
uint32_t g_steps = 0;
bool g_input_flags[NUM_MSGS] = { 0 };
uint32_t g_goal = 1000;
uint32_t g_workout_start_time = 0;

bool display_info_get_debug(void) {
    return g_debug;
}

displayUnits_t display_info_get_units(void) {
    return g_units;
}

uint32_t display_info_get_steps(void) {
    return g_steps;
}

bool display_info_get_input_flag(inputCommMsg_t msg) {
    return g_input_flags[msg];
}

uint32_t display_info_get_goal(void) {
    return g_goal;
}

uint32_t display_info_get_time(void) {
    return (uint32_t)(xTaskGetTickCount() / 1000);
}

uint32_t display_info_get_start(void) {
    return g_workout_start_time;
}

void display_info_set_start(uint32_t start_time) {
    g_workout_start_time = start_time;
}

void display_info_set_goal(uint32_t goal) {
    g_goal = goal;
}

void display_info_set_debug(bool debug) {
    g_debug = debug;
}

void display_info_set_units(displayUnits_t unit) {
    g_units = unit;
}

void display_info_set_steps(uint32_t steps) {
    g_steps = steps;
}

void display_info_set_input_flag(inputCommMsg_t msg, bool flag) {
    g_input_flags[msg] = flag;
}



