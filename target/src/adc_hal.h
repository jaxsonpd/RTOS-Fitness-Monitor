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

void adc_hal_isr(void);

void adc_hal_register(uint8_t, callback_t);

void adc_hal_conversion(uint8_t);

#endif // ADC_HAL_