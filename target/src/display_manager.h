/** 
 * @file display_manager.h
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-08-11
 * @brief Declarations for the display manager thread which writes information
 * to the OLED display
 * 
 * @cite Matthew Suter
 */

#ifndef DISPLAY_MANAGER_H_
#define DISPLAY_MANAGER_H_

/** 
 * @brief Entry point to the display manager thread which updates the display 
 * based on information from the various other threads
 * @param rtos_param the rtos parameter
 * 
 */
void display_manager_thread(void *rtos_param);


#endif /* DISPLAY_MANAGER_H_ */
