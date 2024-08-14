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

#define TICKS_TO_DS 1/100 ///< To convert from ticks to deci-seconds divide by 100

bool g_debug = false;
bool g_toggle = false;
displayUnits_t g_units = UNITS_SI;
uint32_t g_steps = 0;
bool g_input_flags[NUM_MSGS] = { 0 };
uint32_t g_goal = 1000;
uint32_t g_height = 175;
uint32_t g_weight = 70;
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

uint32_t display_info_get_ds(void) {
    return (uint32_t)(xTaskGetTickCount()*TICKS_TO_DS);
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

void display_info_set_toggle(bool toggle) {
    g_toggle = toggle;
}

bool display_info_get_toggle(void) {
    return g_toggle;
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

void display_info_set_height(uint32_t height) {
    g_height = height;
}

void display_info_set_weight(uint32_t weight) {
    g_weight = weight;
}

uint32_t display_info_get_height(void) {
    return g_height;
}

uint32_t display_info_get_weight(void) {
    return g_weight;
}
