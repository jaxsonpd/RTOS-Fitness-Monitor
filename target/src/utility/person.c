#include "person.h"

bool person_init(person_t* person) {
    person_reset(person);
    return true;
}

void person_reset(person_t* person) {
    person->userGoal = DEFAULT_GOAL;
    person->userActivityTimeout = DEFAULT_ACTIVITY_TIMEOUT;
    person->userHeight = DEFAULT_HEIGHT;
    person->userWeight = DEFAULT_WEIGHT;
}