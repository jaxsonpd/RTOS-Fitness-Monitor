/**
 * @file device_info.h
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-08-12
 * @brief Struct and interface to information for the device
 */


#ifndef DEVICE_INFO_H_
#define DEVICE_INFO_H_


#include <stdint.h>
#include <stdbool.h>

#include "comms/input_comms.h"

#define DS_TO_S 1/10 ///< divide by ten to convert from deci-seconds to seconds
#define S_TO_DS 10 ///< multiply by ten to convert from seconds to deci-seconds

typedef enum {
    UNITS_SI = 0,    // Steps  /km
    UNITS_ALTERNATE, // Percent/miles
    UNITS_NUM_TYPES,
} units_t;

uint32_t device_info_get_ds(void);

/**
 * @brief Get the relevant input flag
 * @param msg input flag to check
 *
 * @return true if flag is set
 */
bool device_info_get_input_flag(inputCommMsg_t msg);

/**
 * @brief Set the relevant input flag
 * @param msg the flag to set
 * @param flag the value to set it to
 */
void device_info_set_input_flag(inputCommMsg_t msg, bool flag);

/**
 * @brief Clear all input flags
 */
void device_info_clear_input_flags(void);

/**
 * @brief Set alternate flag for state logic
 */
void device_info_set_alternate(bool status);

/**
 * @brief Get alternate flag for state logic
 */
bool device_info_get_alternate(void);

/**
 * @brief Set debug flag for state logic
 */
void device_info_set_debug(bool status);

/**
 * @brief Get debug flag for state logic
 */
bool device_info_get_debug(void);


/**
 * @brief Set the display units
 * @param unit the unit to set the display too
 *
 */
void device_info_set_units(units_t units);

/**
 * @brief Get the units of the display
 *
 * @return the displays units
 */
units_t device_info_get_units(void);

/**
 * @brief Set the last step time
 * @param time the unit to set the display too
 *
 */
void device_info_set_last_step_time(uint32_t time);

/**
 * @brief Get the last step time
 *
 * @return the last step time
 */
uint32_t device_info_get_last_step_time(void);

/**
 * @brief Set the workout start time
 * @param time time to set
 *
 */
void device_info_set_workout_start_time(uint32_t time);

/**
 * @brief Get the workout start time
 *
 * @return workout start time
 */
uint32_t device_info_get_workout_start_time(void);

#endif // DEVICE_INFO_H_