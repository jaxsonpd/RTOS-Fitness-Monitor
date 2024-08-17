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
#include "utility/vector3.h"

bool accl_init(void);           // set up library
void accl_poll(void);           // Run periodically
vector3_t accl_mean(void);      // Return mean acceleration

#endif /* ACCL_MANAGER_H_ */
