#ifndef STEP_COUNTER_MANAGER_H_
#define STEP_COUNTER_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef UNIT_TESTING
bool step_counter_manager_init(uint16_t* period);
#endif // UNIT_TESTING

/**
 * @brief The entry to the step counter thread
 * @param rtos_param the rtos parameter
 */
void step_counter_thread(void* rtos_param);


#endif // STEP_COUNTER_H_