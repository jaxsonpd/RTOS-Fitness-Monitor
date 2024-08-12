/*
 * accl_manager.h
 *
 *  Created on: 23/03/2022
 *      Author: Matt Suter
 *
 *  FitnessThur9-1
 */

#ifndef ACCL_MANAGER_H_
#define ACCL_MANAGER_H_

#include <stdint.h> // needed to init the vector type
#include <stdbool.h>

typedef struct{
    int16_t x;
    int16_t y;
    int16_t z;
} vector3_t;

bool acclInit(void);        // set up library
void acclPoll(void);     // Run periodically
vector3_t acclMean(void);   // Return mean acceleration


#endif /* ACCL_MANAGER_H_ */
