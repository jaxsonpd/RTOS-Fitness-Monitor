/**
 * @file step_counter_comms.h
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-10-08
 * @brief Interface to handle communication of the step count
 */


#ifndef INPUT_COMMS_H
#define INPUT_COMMS_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief initialise the comms module
 *
 * @return true if successful
 */
bool step_counter_comms_init(void);

/**
 * @brief update the step count value
 * @param stepsAccumulated the amount to add to the total
 *
 * @return true if successfully updated
 */
bool step_counter_set(uint32_t stepsAccumulated);

/**
 * @brief get the step count value
 *
 * @return the step count value
 */
uint32_t step_counter_get(void);



#endif // INPUT_COMMS_H
