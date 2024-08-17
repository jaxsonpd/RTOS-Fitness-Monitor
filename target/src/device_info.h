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

#endif // DEVICE_INFO_H_