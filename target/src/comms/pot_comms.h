/**
 * @file pot_comms.h
 * @author Daniel Hawes (dha144@uclive.ac.nz)
 * @date 2024-08-12
 * @brief Interface to handle communication of the step
 */

#ifndef POT_COMMS_H
#define POT_COMMS_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initialise the potentiometer comms module
 * 
 * @return true if successful
 */
bool pot_comms_init(void);

/**
 * @brief Set the potentiometer value
 * @param pot_value_passed the value to set the potentiometer to
 * 
 * @return true if successfully set
 */
bool pot_set(uint32_t pot_value_passed);

/**
 * @brief Get the potentiometer value
 * 
 * @return the potentiometer value
 */
uint32_t pot_get(void);


#endif // POT_COMMS_H