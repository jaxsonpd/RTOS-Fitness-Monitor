/** 
 * @file adc_handler.h
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief interface for handling adc interactions
 */

#ifndef ADC_HANDLER_H_
#define ADC_HANDLER_H_

#include <stdint.h>

/**
 * @brief write data to circular buffer when ISR for adc is triggered.
 * Assigned to adc in the HAL.
 * @param adc_value adc data to write to the buffer
 */
void adc_callback(uint32_t adc_value);

/**
 * @brief initialise adc peripheral and assign callback function
 */
void adc_init(void);

/**
 * @brief trigger an adc reading for the correct adc
 * adc will cause an interrupt when data is ready.
 */
void adc_poll(void);

/**
 * @brief read all values from the adc buffer and get an average
 * ignores any invalid values
 * @return average of reading
 */
uint32_t adc_read(void);     // Call to read the averaged readings from the buffer


#endif //ADC_HANDLER_H_
