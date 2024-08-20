/**
 * @file distance_state.c
 * @author Isaac Cone (ico29@uclive.ac.nz), Jack Duignan (jdu80@uclive.ac.nz)
 * @date 2024-08
 * @brief Display the distance travelled by the user
 */

#include <stdint.h>

#include "../hal/display_hal.h"
#include "../device_info.h"
#include "../comms/step_counter_comms.h"
#include "../utility/person.h"

#include "distance_state.h"

#define KM_TO_MILES 62/100  ///< Approximation of 0.6215
#define MS_TO_KMH 36/10

#define CAL_TO_KJ(x) (4184*x)/1000
#define M_PER_STEP(x) (x*415)/100000
#define M_KG_TO_CAL(x,y) (125*x*y)/100000 // https://www.analog.com/en/resources/analog-dialogue/articles/pedometer-design-3-axis-digital-acceler.html

void distance_state_enter(void) {

}

stateStatus_t distance_state_execute(void* args) {
    person_t *person = (person_t *)args;

    // Calc workout time
    uint32_t workout_time = 0;
    uint32_t workout_start_time = device_info_get_workout_start_time();
    if (workout_start_time != 0) {
        workout_time = device_info_get_ds() - workout_start_time;
    }

    // Calc Distance travelled
    uint32_t mTravelled = step_counter_get() * M_PER_STEP(person->userHeight);
    uint32_t calories = M_KG_TO_CAL(mTravelled, person->userWeight);

    uint16_t speed;
    if (workout_time != 0) { // Protect against division by zero
        speed = (mTravelled / (workout_time * DS_TO_S)) * MS_TO_KMH; // in km/h
    } else {
        speed = mTravelled * MS_TO_KMH; // if zero seconds elapsed, act as if it's at least one
    }

    display_time("Time:", workout_time * DS_TO_S, 1, ALIGN_CENTRE);

    if (device_info_get_units() == UNITS_SI) {
        display_value("Dist:", "km", mTravelled, 0, ALIGN_CENTRE, true);
        display_value("Speed", "kph", speed, 2, ALIGN_CENTRE, false);
        display_value("Energy:","kJ", CAL_TO_KJ(calories), 3, ALIGN_CENTRE, false);

    } else {
        display_value("Dist:", "mi", mTravelled * KM_TO_MILES, 0, ALIGN_CENTRE, true);
        display_value("Speed:", "mph", speed * KM_TO_MILES, 2, ALIGN_CENTRE, false);
        display_value("Energy:","Cal", calories, 3, ALIGN_CENTRE, false);
    }

    return STATE_SUCCESS;
}

void distance_state_exit(void) {
    display_clear();
}

void distance_state_reset(void) {
    
}

state_t distanceState = { distance_state_enter,distance_state_execute,distance_state_exit,distance_state_reset };