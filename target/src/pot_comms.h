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

bool pot_comms_init(void);
bool pot_set(uint32_t pot_value_passed);
uint32_t pot_get(void);


#endif // POT_COMMS_H