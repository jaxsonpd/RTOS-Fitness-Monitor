/**
 * @file potentiometer_comms.c
 * @author Daniel Hawes (dha144@uclive.ac.nz)
 * @date 2024-08-12
 * @brief Interface to handle communication of the step
 */

#ifdef POTENTIOMETER_COMMS_H
#define POTENTIOMETER_COMMS_H

#include <stdint.h>
#include <stdbool.h>

bool potentiometer_comms_init(void);
bool potentiometer_set(uint32_t potentiometerRead);
uint32_t potentiometer_get(void);


#endif // POTENTIOMETER_COMMS_H