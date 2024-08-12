#ifndef POTENTIOMETER_READ_H
#define POTENTIOMETER_READ_H

#include <stdint.h>
#include <stdbool.h>

void potentiometerIntHandler(void);

void pollPotentiometer(void);
void initPotentiometer(void);
uint32_t readADC(void);

void potentiometer_thread(void* args);

#ifdef UNIT_TESTING
bool potentiometer_init(void);
#endif //UNIT_TESTING

#endif // POTENTIOMETER_READ_H