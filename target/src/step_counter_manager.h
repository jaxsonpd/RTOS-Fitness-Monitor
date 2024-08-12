#ifndef STEP_COUNTER_MANAGER_H_
#define STEP_COUNTER_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef UNIT_TESTING
bool step_counter_manager_init(void);
#endif // UNIT_TESTING

void step_counter_thread(void* args);


#endif // STEP_COUNTER_H_