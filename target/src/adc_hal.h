/** 
 * @file adc_hal.h
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Hardware Abstraction Layer header for an ADC
 */

#ifndef ADC_HAL_
#define ADC_HAL_

#include <stdint.h>

typedef enum {
    ADC_ID_0 = 0,
    MAX_ADC_IDS = 10
} adc_id_t;

typedef void (*callback_t)(uint32_t);

/**
 * @brief check interrupt flags, handle raised interrupts from callback, 
 * clear flags. 
 */
void adc_hal_isr(void);

/**
 * @brief enable adc peripheral and register callback function to instance
 * @param id adc peripheral id
 * @param callback callback function pointer to handle uint32_t reading
 */
void adc_hal_register(uint8_t id, callback_t callback);

/**
 * @brief trigger adc peripheral reading to get data
 * @param id adc peripheral id
 */
void adc_hal_conversion(uint8_t id);

#endif // ADC_HAL_