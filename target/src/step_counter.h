#ifndef STEP_COUNTER_H_
#define STEP_COUNTER_H_

#include <stdint.h>
#include <stdbool.h>

#include "accl_manager.h"

#define STEP_THRESHOLD_HIGH 300
#define STEP_THRESHOLD_LOW 250
#define STEP_DELAY_MS 1000

bool detect_step(vector3_t acceleration, uint32_t *stepsAccumulated);

#endif // STEP_COUNTER_H_