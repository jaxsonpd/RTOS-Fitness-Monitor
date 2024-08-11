#ifndef STEP_COUNTER_H_
#define STEP_COUNTER_H

#include <stdint.h>
#include <stdbool.h>

void step_counter_thread(void* args);

#ifdef UNIT_TESTING
bool step_counter_init(void);
#endif // UNIT_TESTING

#endif // STEP_COUNTER_H_