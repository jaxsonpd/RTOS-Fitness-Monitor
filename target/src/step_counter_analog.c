/** 
 * @file step_counter_analog.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Implementation of step counter to fit the defined interface. Algorithm based on 
 * Analog Devices case study on different 3-axis accelerometer chip.
 * @cite https://www.analog.com/en/resources/analog-dialogue/articles/pedometer-design-3-axis-digital-acceler.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "step_counter.h"

#define ALGORITHM_PERIOD_MS 20
#define TIME_WINDOW_LOWER 10        ///< Lower period limit for high frequency noise rejection
#define TIME_WINDOW_UPPER 100       ///< Upper period limit for low frequency rejection
#define DYNAMIC_PRECISION 100       ///< Tunable parameter, depends on accelerometer data magnitudes
#define RHYTHMIC_SEQUENCE_START 2   ///< Steps in a row required to begin tracking rhythmic sequence 

/// typedefs to store algorithm information
typedef enum {
    STATE_SEARCHING,    ///< Searching for rhytmic steps
    STATE_RHYTHMIC,     ///< In rhythmic step pattern
} StepState;

/**
 * @struct structure to hold the context of the step counter
 *
 */
uint32_t step_counter_get(void);
typedef struct {
    int16_t min_threshold;          ///< The minimum threshold for a step, updates dynamically
    int16_t max_threshold;          ///< The maximum threshold for a step, updates dynamically
    int16_t sample_old;             ///< The previous x/y/z sample for acceleration slope estimation
    int16_t sample_new;             ///< The current x/y/z sample for acceleration slope estimation
    vector3_t acceleration_old;     ///< The previous acceleration vector
    uint16_t samples_counter;       ///< Counter to determine the timeout for high and low on step period
    uint8_t rhythmic_step_counter;  ///< Count steps to enter a rhythmic step counting pattern
    StepState state;                ///< The step state (SEARCHING or RHYTHMIC)
} stepContext_t;

// Initialise step counter context
static stepContext_t context;

/**
 * @brief Initialise the step counter context
 *
 * @param context a pointer to the context to be initialised
 */
void init_step_context(stepContext_t* context) {
    context->min_threshold = INT16_MAX;
    context->max_threshold = INT16_MIN;
    context->sample_old = 0;
    context->sample_new = 0;
    context->acceleration_old = v3_constructor(0,0,0);
    context->samples_counter = 0;
    context->rhythmic_step_counter = 0;
    context->state = STATE_SEARCHING;
}

/**
 * @brief find the acceleration axis with the largest change in magnitude since previous sample
 *
 * @return acceleration value from axis with largest change since previous sample
 */
int16_t calculate_sample(stepContext_t* context, vector3_t acceleration) {
    int32_t dx = acceleration.x - context->acceleration_old.x;
    int32_t dy = acceleration.y - context->acceleration_old.y;
    int32_t dz = acceleration.z - context->acceleration_old.z;

    context->acceleration_old = acceleration;

    if (abs(dx) < DYNAMIC_PRECISION && abs(dy) < DYNAMIC_PRECISION && abs(dz) < DYNAMIC_PRECISION) {
        return context->sample_new;
    }

    if (abs(dx) > abs(dy) && abs(dx) > abs(dz)) {
        return acceleration.x;
    } else if (abs(dy) > abs(dz)) {
        return acceleration.y;
    } else {
        return acceleration.z;
    }
}

/**
 * @brief update context with new thresholds if sample is outside current range
 *
 * @param context step counter context to be updated
 * @param sample sample value to be set if outside current threshold
 */
void update_dynamic_threshold(stepContext_t* context, int16_t sample) {
    context->max_threshold = (context->max_threshold < sample) ? sample : context->max_threshold;
    context->min_threshold = (context->min_threshold > sample) ? sample : context->min_threshold;
}

/**
 * @brief determine if a step is detected
 * 
 * Step is detected based on a negative acceleration gradient when the magnitude crosses the threshold
 *
 * @param sample_new new sample for comparison
 * @param sample_old old sample for comparison
 * @param dynamic_threshold average of context max and min threshold
 * 
 * @return true if step detected
 */
bool is_step_detected(int16_t sample_new, int16_t sample_old, int dynamic_threshold) {
    return (sample_new < sample_old && sample_new < dynamic_threshold);
}

/**
 * @brief executed when step counter is searching for a rhythmic step pattern
 * 
 * Checks for rhythmic conditions to change state
 *
 * @param context the step counter context
 * @param stepsAccumulated pointer to the running total of steps counted
 * @param acceleration the latest acceleration vector for decision-making
 * 
 * @return true if step detected (always false for searching)
 */
 bool handle_searching_state(stepContext_t* context, uint32_t* stepsAccumulated, vector3_t acceleration) {
    if (is_step_detected(context->sample_new, context->sample_old, (context->max_threshold + context->min_threshold) / 2)) {
        context->rhythmic_step_counter++;
        if (context->rhythmic_step_counter >= RHYTHMIC_SEQUENCE_START) {
            context->samples_counter = 0;
            context->state = STATE_RHYTHMIC;
        }
        return false;
    } 

    context->samples_counter++;
    return false;
}

/**
 * @brief executed when step counter has found a rhythmic step pattern
 * 
 * Checks for non-rhythmic conditions to change state to searching
 *
 * @param context the step counter context
 * @param stepsAccumulated pointer to the running total of steps counted
 * @param acceleration the latest acceleration vector for decision-making
 * 
 * @return true if step detected
 */
bool handle_rhythmic_state(stepContext_t* context, uint32_t* stepsAccumulated, vector3_t acceleration) {
    if (is_step_detected(context->sample_new, context->sample_old, (context->max_threshold + context->min_threshold) / 2)) {
        if (context->samples_counter >= TIME_WINDOW_LOWER && context->samples_counter <= TIME_WINDOW_UPPER) {
            if (context->rhythmic_step_counter != 0) {
                *stepsAccumulated += context->rhythmic_step_counter;
                context->rhythmic_step_counter = 0;
            } else {
                (*stepsAccumulated)++;
                context->samples_counter = 0;
                return true;
            }
        }
    }

    context->samples_counter++;

    if (context->samples_counter > TIME_WINDOW_UPPER) {
        context->state = STATE_SEARCHING;
    }

    return false;
}

bool step_counter_init(uint16_t* period) {
    if (period == NULL) {
        return false;
    }
    init_step_context(&context);
    *period = ALGORITHM_PERIOD_MS;
    return true;
}

bool detect_step(vector3_t acceleration, uint32_t* stepsAccumulated) {
    context.sample_old = context.sample_new;
    context.sample_new = calculate_sample(&context, acceleration);

    if (context.samples_counter % 50 == 0) {
        update_dynamic_threshold(&context, context.sample_new);
    }

    switch (context.state) {
        case STATE_SEARCHING:
            return handle_searching_state(&context, stepsAccumulated, acceleration);
        case STATE_RHYTHMIC:
            return handle_rhythmic_state(&context, stepsAccumulated, acceleration);
        default:
            return false;
    }
}