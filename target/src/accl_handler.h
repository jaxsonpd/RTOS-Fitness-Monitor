/** 
 * @file accl_handler.h
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief interface for handling accelerometer interactions
 */


#ifndef ACCL_HANDLER_H_
#define ACCL_HANDLER_H_

#include <stdbool.h>
#include "utility/vector3.h"

/**
 * @brief initialise the accelerometer peripheral via hal
 * also creates buffers to hold data
 * @return true if successful
 */
bool accl_init(void);

/**
 * @brief gets accelerometer reading periodically
 * writes to x/y/z buffers
 */
void accl_poll(void);

/**
 * @brief read data out of buffers, construct vector3_t of acceleration
 * @return acceleration average reading
 */
vector3_t accl_mean(void);

#endif /* ACCL_HANDLER_H_ */
