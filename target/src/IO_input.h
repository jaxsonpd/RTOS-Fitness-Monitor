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


/// The different input button and switches that can be polled.
typedef enum input_mode {
    UP_BUTTON,
    DOWN_BUTTON,
    RIGHT_BUTTON,
    LEFT_BUTTON,
    LEFT_SWITCH,
    RIGHT_SWITCH
} input_mode_t;


/** 
 * @brief Initialise the input module
 * 
 * @return true if successful
 */
bool input_init(void);


/**
 * @brief Get the state of an input
 * 
 * @param input the input to get the state of
 * @return the state of the input
 */
bool input_get(input_mode_t mode);


/**
 * @brief update the inputs
 * 
 */
void input_update(void);


#endif // IO_INPUT_H