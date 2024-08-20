/** 
 * @file person.h
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Defines person type for holding user information and functions to interact with it
 */

#ifndef PERSON_H_
#define PERSON_H_

#include <stdint.h>
#include <stdbool.h>

#define DEFAULT_GOAL 1000
#define DEFAULT_ACTIVITY_TIMEOUT 100
#define DEFAULT_HEIGHT 170
#define DEFAULT_WEIGHT 70

/**
 * @struct person_t
 *  A structure to store information about a fitness monitor user
 *  
 */
typedef struct Person {
    uint16_t userGoal;              ///< user step count goal
    uint16_t userActivityTimeout;   ///< user target for max inactive time
    uint16_t userHeight;            ///< user height (cm)
    uint16_t userWeight;            ///< user weight (kg)
} person_t;

/** 
 * @brief initialises an instance of person
 * 
 * @param person a pointer to the person instance to be initialised
 * 
 * @return true if successful
 */
bool person_init(person_t* person);

/** 
 * @brief resets user information
 * 
 * @param person a pointer to the person instance be reset
 */
void person_reset(person_t* person);

#endif // PERSON_H_