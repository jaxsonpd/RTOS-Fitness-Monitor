/**
 * @file input_hal.h
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-07-31
 * @brief Abstract IO interface.
 */


#ifndef INPUT_HAL_H_
#define INPUT_HAL_H


#include <stdint.h>
#include <stdbool.h>

#define NUM_INPUT_MODES 6 ///< number of input modes
#define DEBOUNCE_NUMBER 3 ///< number of times before state change

/// The different input button and switches that can be polled.
typedef enum inputMode_e {
    UP_BUTTON,
    DOWN_BUTTON,
    LEFT_BUTTON,
    RIGHT_BUTTON,
    RIGHT_SWITCH,
    LEFT_SWITCH,
} inputMode_t;

/// The possible states of the buttons and switches
typedef enum inputState_e {
    PUSHED,
    RELEASED,
    NO_CHANGE
} inputState_t;


/**
 * @brief Initialise the input module

 * @return true if successful
 */
bool input_init(void);


/**
 * @brief Get the state of an input
 *
 * This is mainly used for switches
 * @param input the input to get the state of
 * @return true if the input is pressed or switched and false otherwise
 */
bool input_get(inputMode_t input);


/**
 * @brief update the inputs states performing debouncing
 *
 */
void input_update(void);


/**
 * @brief check if the the button has been pressed or released
 *
 * This differs from get in that it returns state changes not the actual
 * state of the button. This will not be as helpful for switches.
 * @param input the input to get the flag of
 * @return the state change of the button
 */
inputState_t input_check(inputMode_t input);

#endif // INPUT_HAL_H_