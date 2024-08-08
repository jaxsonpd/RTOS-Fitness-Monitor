/**
 * @file input_comms.h
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-08-08
 * @brief Declirations for the module to handle communication from the input
 * manager to display manager tasks.
 */


#ifndef INPUT_COMMS_H
#define INPUT_COMMS_H


#include <stdint.h>
#include <stdbool.h>

enum inputCommMsg_e {
    NO_MESSAGES = 0,
    SCREEN_LEFT,
    SCREEN_RIGHT
};

typedef enum inputCommMsg_e inputCommMsg_t;

/**
 * @brief Initialise the comms module
 *
 * @return true if successful
 */
bool input_comms_init(void);

/**
 * @brief Send a message using the comm channel
 * @param msg the message to send
 *
 * @return true if successfully sent
 */
bool input_comms_send(inputCommMsg_t msg);

/**
 * @brief Read a message from the comm channel if one is ready
 *
 * @return the message received
 */
inputCommMsg_t input_comms_receive(void);



#endif // INPUT_COMMS_H