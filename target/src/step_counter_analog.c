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

// Defined parameters
#define ALGORITHM_PERIOD_MS 20
#define TIME_WINDOW_LOWER 10    // H.f. noise rejection
#define TIME_WINDOW_UPPER 100   // Rhythmic timeout
#define DYNAMIC_PRECISION 100   // Magnitude threshold
#define RHYTHMIC_SEQUENCE_START 2 // Steps to begin tracking rhythmic sequence

// typedefs to store algorithm information
typedef enum {
    STATE_SEARCHING,
    STATE_RHYTHMIC,
} StepState;

typedef struct {
    int16_t min_threshold;
    int16_t max_threshold;
    int16_t sample_old;
    int16_t sample_new;
    vector3_t acceleration_old;
    uint16_t samples_counter;
    uint8_t rhythmic_step_counter;
    StepState state;
} step_context_t;

// Initialise step counter context
static step_context_t context;

// Local helper functions
static void init_step_context(step_context_t* context) {
    context->min_threshold = INT16_MAX;
    context->max_threshold = INT16_MIN;
    context->sample_old = 0;
    context->sample_new = 0;
    context->acceleration_old = v3_constructor(0,0,0);
    context->samples_counter = 0;
    context->rhythmic_step_counter = 0;
    context->state = STATE_SEARCHING;
}

static int16_t calculate_sample(step_context_t* context, vector3_t acceleration) {
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

static void update_dynamic_threshold(step_context_t* context, int16_t sample) {
    context->max_threshold = (context->max_threshold < sample) ? sample : context->max_threshold;
    context->min_threshold = (context->min_threshold > sample) ? sample : context->min_threshold;
}

static bool is_step_detected(int16_t sample_new, int16_t sample_old, int dynamic_threshold) {
    return (sample_new < sample_old && sample_new < dynamic_threshold);
}

static bool handle_searching_state(step_context_t* context, uint32_t* stepsAccumulated, vector3_t acceleration) {
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

static bool handle_rhythmic_state(step_context_t* context, uint32_t* stepsAccumulated, vector3_t acceleration) {
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

// Implementations of interface functions
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