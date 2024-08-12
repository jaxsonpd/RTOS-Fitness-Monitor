#ifndef ADC_MANAGER_H_
#define ADC_MANAGER_H_

#include <stdint.h>

//*****************************************************************************
//
// The handler for the ADC conversion complete interrupt.
// Writes to the circular buffer.
//
//*****************************************************************************
void callbackADC(uint32_t adc_value);

void pollADC(void);         // Called periodically at high frequency
void initADC (void);
uint32_t readADC(void);     // Call to read the averaged readings from the buffer

#ifdef UNIT_TESTING
bool adc_read_init(void);
#endif // UNIT_TESTING


#endif //ADC_MANAGER_H_
