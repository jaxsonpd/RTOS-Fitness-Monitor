/**
 * @file input_manager.h
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-08-07
 * @brief Declarations for the input managment thread to handle IO
 * input.
 *
 * @cite Daniel Rabbidge
 */

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief The entry to the input manager thread
 * @param input_msg_queue a pointer to the input_msg_queue for communicating
 * with display manager
 *
 */
void input_manager_thread(void *input_msg_queue);


#endif // INPUT_MANAGER_H
