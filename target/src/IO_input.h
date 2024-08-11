/**
 * @file IO_input.h
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-07-31
 * @brief This module allows for communication IO on the TIVA board.
 */


#ifndef IO_INPUT_H
#define IO_INPUT_H


#include <stdint.h>
#include <stdbool.h>

#define NUM_INPUT_MODES 5 ///< number of input modes


/// The different input button and switches that can be polled.
typedef enum input_mode_e {
    UP_BUTTON,
    DOWN_BUTTON,
    LEFT_BUTTON,
    RIGHT_BUTTON,
    RIGHT_SWITCH,
    LEFT_SWITCH,
} input_mode_t;

typedef enum input_state_e {
    PUSHED,
    RELEASED,
    NO_CHANGE
} input_state_t;


/**
 * @brief Initialise the input module
 *
 * @param override_portf true if the port f (right button) should work (this
 * will fail on non tiva board systems)
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
bool input_get(input_mode_t input);


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
input_state_t input_check(input_mode_t input);

#endif // IO_INPUT_H