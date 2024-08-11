/**
 * @file input_comms.h
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-08-08
 * @brief Declirations for the module to handle communication from the input
 * manager to display manager tasks.
 */


#ifndef INPUT_COMMS_H_
#define INPUT_COMMS_H_


#include <stdint.h>
#include <stdbool.h>

enum inputCommMsg_e {
    NO_MESSAGES = 0,
    MSG_SCREEN_LEFT,
    MSG_SCREEN_RIGHT,
    MSG_UP_BUTTON_P,
    MSG_UP_BUTTON_R,
    MSG_DOWN_BUTTON_P,
    MSG_DOWN_BUTTON_R,
    MSG_RIGHT_SWITCH_ON,
    MSG_RIGHT_SWITCH_OFF,
    MSG_LEFT_SWITCH_ON,
    MSG_LEFT_SWITCH_OFF
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

/**
 * @brief Return the number of messages waiting
 *
 * @return the number of messages waiting in the comms channel
 */
uint8_t input_comms_num_msgs(void);



#endif // INPUT_COMMS_H_