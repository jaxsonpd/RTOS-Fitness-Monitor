/** 
 * @file person.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Implementation of person type to hold user info
 */

#include <stdlib.h>

#include "person.h"

bool person_init(person_t* person) {
    if (person == NULL) {
        return false;
    }
    person_reset(person);
    return true;
}

void person_reset(person_t* person) {
    person->userGoal = DEFAULT_GOAL;
    person->userActivityTimeout = DEFAULT_ACTIVITY_TIMEOUT;
    person->userHeight = DEFAULT_HEIGHT;
    person->userWeight = DEFAULT_WEIGHT;
}