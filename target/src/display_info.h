/**
 * @file display_info.h
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-08-12
 * @brief Struct and interface to information for the displays
 */


#ifndef DISPLAY_INFO_H
#define DISPLAY_INFO_H


#include <stdint.h>
#include <stdbool.h>

#include "input_comms.h"

#define DS_TO_S 1/10 ///< divide by ten to convert from deci-seconds to seconds
#define S_TO_DS 10 ///< multiply by ten to convert from seconds to deci-seconds

typedef enum {
    UNITS_SI = 0,    // Steps  /km
    UNITS_ALTERNATE, // Percent/miles
    UNITS_NUM_TYPES,
} displayUnits_t;

/**
 * @brief Get the debug state
 *
 * @return true if in debug
 */
bool display_info_get_debug(void);

/**
 * @brief Get the units of the display
 *
 * @return the displays units
 */
displayUnits_t display_info_get_units(void);

/**
 * @brief Get the current step count
 *
 * @return the number of steps taken
 */
uint32_t display_info_get_steps(void);

/**
 * @brief Get the relevant input flag
 * @param msg input flag to check
 *
 * @return true if flag is set
 */
bool display_info_get_input_flag(inputCommMsg_t msg);

/**
 * @brief Get the current goal
 *
 * @return the current goal in steps
 */
uint32_t display_info_get_goal(void);

/**
 * @brief Get the number of 100s of milli seconds (deci-seconds) since the start of the RTOS
 *
 * @return the number of 100s milli seconds (deci-seconds)
 */
uint32_t display_info_get_ds(void);

/**
 * @brief Get the time that the workout was started
 *
 * @return the time in deci-seconds, 0 workout if hasn't started
 */
uint32_t display_info_get_start(void);

/**
 * @brief Get the time of the last step
 *
 * @return the time of the last step in deci-seconds
 */
uint32_t display_info_get_last_step_time(void);

/**
 * @brief Set the time that the workout was started
 * @param start_time the time the workout was started in deci-seconds
 * set this to 0 if the workout is inactive
 *
 */
void display_info_set_start(uint32_t start_time);


/**
 * @brief Set the current goal
 * @param goal the new goal to set to
 *
 */
void display_info_set_goal(uint32_t goal);

/**
 * @brief Set the debug flag
 * @param debug the value to set the debug flag to
 *
 */
void display_info_set_debug(bool debug);

/**
 * @brief Set the displas units
 * @param unit the unit to set the display too
 *
 */
void display_info_set_units(displayUnits_t unit);

/**
 * @brief Set the step count
 * @param steps the number of steps taken
 *
 */
void display_info_set_steps(uint32_t steps);

/**
 * @brief Set the relevant input flag
 * @param msg the flag to set
 * @param flag the value to set it to
 */
void display_info_set_input_flag(inputCommMsg_t msg, bool flag);

/**
 * @brief Set the time of the last step
 * @param time the time of the last step in deci-seconds
 */
void display_info_set_last_step_time(uint32_t time);


#endif // DISPLAY_INFO_H