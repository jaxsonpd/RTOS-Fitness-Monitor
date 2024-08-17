#ifndef PERSON_H_
#define PERSON_H_

#include <stdint.h>
#include <stdbool.h>

#define DEFAULT_GOAL 1000
#define DEFAULT_ACTIVITY_TIMEOUT 10
#define DEFAULT_HEIGHT 170
#define DEFAULT_WEIGHT 70

typedef struct Person {
    uint16_t userGoal;
    uint16_t userActivityTimeout;
    uint16_t userHeight;
    uint16_t userWeight;
} person_t;

bool person_init(person_t* person);

void person_reset(person_t* person);

#endif // PERSON_H_