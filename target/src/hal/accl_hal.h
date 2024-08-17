/** 
 * @file accl_hal.h
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Hardware Abstraction Layer header for an I2C Accelerometer
 */

#ifndef ACCL_HAL_H_
#define ACCL_HAL_H_

#include <stdint.h>

/** 
 * @brief Initialise I2C peripheral and use it to enable and configure
 * the accelerometer.
 */
void accl_chip_init(void);

/**
 * @brief Get acceleration data over I2C from the accelerometer
 * @param acceleration pointer to array to fill with x,y,z accelerometer data.
 */
void accl_data_get(int16_t* acceleration);

#endif /* ACCL_HAL_H_ */