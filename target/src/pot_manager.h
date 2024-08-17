/**
 * @file pot_manager.h
 * @author Daniel Hawes (dha144@uclive.ac.nz)
 * @date 2024-08-12
 * @brief Declarations for the potentiometer manager
 * 
 */

#ifndef POT_MANAGER_H_
#define POT_MANAGER_H

#include <stdint.h>
#include <stdbool.h>


/**
 * @brief Initialise the potentiometer manager
 * 
 * @return true if successful
 */
#ifdef UNIT_TESTING
bool pot_read_init(void);
#endif // UNIT_TESTING

/**
 * @brief The entry to the potentiometer manager thread
 * 
 */
void pot_read_thread(void* args);

#endif // POT_MANGER_H_