/** 
 * @file person.h
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Defines person type for holding user information
 */

#ifndef PERSON_H_
#define PERSON_H_

#include <stdint.h>
#include <stdbool.h>

#define DEFAULT_GOAL 1000
#define DEFAULT_ACTIVITY_TIMEOUT 100
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