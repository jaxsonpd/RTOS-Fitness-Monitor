#ifndef ADC_READ_H_
#define ADC_READ_H_

#include <stdint.h>

//*****************************************************************************
//
// The handler for the ADC conversion complete interrupt.
// Writes to the circular buffer.
//
//*****************************************************************************
void callbackADC(void);

void pollADC(void);         // Called periodically at high frequency
void initADC (void);
uint32_t readADC(void);     // Call to read the averaged readings from the buffer

bool adc_read_init(void);
void adc_read_thread(void* args);

#endif //ADC_READ_H_
