/** 
 * @file accl_hal.h
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Hardware abstraction layer interface for an accelerometer
 */

#ifndef ACCL_HAL_H_
#define ACCL_HAL_H_

#include <stdint.h>

/** 
 * @brief Initialise the accelerometer
 */
void accl_chip_init(void);

/**
 * @brief Get acceleration data rom the accelerometer
 * @param acceleration pointer to array to fill with accelerometer data.
 */
void accl_data_get(int16_t* acceleration);

#endif /* ACCL_HAL_H_ */