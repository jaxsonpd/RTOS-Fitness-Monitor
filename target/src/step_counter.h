/** 
 * @file step_counter.h
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Abstract interface for step counting algorithm based to determine a step based
 * on data from a vector3_t.
 */

#ifndef STEP_COUNTER_H_
#define STEP_COUNTER_H_

#include <stdint.h>
#include <stdbool.h>

#include "accl_manager.h" // for vector3_t

bool step_counter_init(void);

/**
 * @brief Detects a step based on the provided acceleration data. 
 *
 * @param acceleration A vector3_t structure representing the current acceleration data.
 * @param stepsAccumulated A pointer to the step count accumulator, which will be incremented if a step is detected.
 * @return true if a valid step is detected, otherwise false.
 */
bool detect_step(vector3_t acceleration, uint32_t* stepsAccumulated);

#endif // STEP_COUNTER_H