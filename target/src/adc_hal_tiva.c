/** 
 * @file adc_hal_tiva.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Hardware Abstraction Layer implementation for the ADC on the TIVA.
 */

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "driverlib/adc.h"
#include "driverlib/sysctl.h"

#include "adc_hal.h"

typedef struct {
    uint32_t sysctlPeriph;
    uint32_t baseAddress;
    uint32_t sequenceNumber;
    uint32_t step;
    uint32_t configuration;
    uint32_t priority;
    callback_t callback;
} adc_config_t;

static adc_config_t adc_configurations[MAX_ADC_IDS] = {
    {SYSCTL_PERIPH_ADC0, ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END, 0,},
};

void adc_hal_isr(void)
{

}

void adc_hal_register(uint8_t id, callback_t callback)
{
    // Catch invalid id
    if (id >= MAX_ADC_IDS) {
        return;
    }

    adc_config_t *config = &adc_configurations[id];

    SysCtlPeripheralEnable(config->sysctlPeriph);
    while (!SysCtlPeripheralReady(config->sysctlPeriph));

    ADCSequenceConfigure(config->baseAddress, config->sequenceNumber, ADC_TRIGGER_PROCESSOR, config->priority);
    ADCSequenceStepConfigure(config->baseAddress, config->sequenceNumber, config->step, config->configuration);
    ADCSequenceEnable(config->baseAddress, config->sequenceNumber);

    ADCIntRegister(config->baseAddress, config->sequenceNumber, adc_hal_isr);
    ADCIntEnable(config->baseAddress, config->sequenceNumber);

    config->callback = callback;
}