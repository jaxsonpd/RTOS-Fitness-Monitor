#ifndef POTENTIOMETER_READER_H
#define POTENTIOMETER_READER_H

#include <stdint.h>
#include <stdbool.h>

void potentiometer_thread(void* args);

#ifdef UNIT_TESTING
bool potentiometer_init(void);
#endif //UNIT_TESTING

#endif // POTENTIOMETER_READER_H