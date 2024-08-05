/** 
 * @file accl_hal.h
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Hardware Abstraction Layer header for accelerometer
 */

#ifndef ACCL_HAL_H_
#define ACCL_HAL_H_

#include <stdint.h>

void initAcclChip(void);

void getAcclData(int16_t*);

#endif /* ACCL_HAL_H_ */
