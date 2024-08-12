/**
 * @file adc_comms.c
 * @author Daniel Hawes (dha144@uclive.ac.nz)
 * @date 2024-08-12
 * @brief Interface to handle communication of the step
 */

#ifdef ADC_COMMS_H
#define ADC_COMMS_H

#include <stdint.h>
#include <stdbool.h>

bool adc_comms_init(void);
bool adc_set(uint32_t adcRead);
uint32_t adc_get(void);


#endif // ADC_COMMS_H