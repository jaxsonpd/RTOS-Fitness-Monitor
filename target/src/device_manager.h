/** 
 * @file device_manager.h
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Declarations for the device manager thread which handles the device state
 */

#ifndef DEVICE_MANAGER_H_
#define DEVICE_MANAGER_H_

/** 
 * @brief Entry point to device manager thread
 * @param rtos_param the rtos parameter
 * 
 */
void device_manager_thread(void *rtos_param);


#endif /* DEVICE_MANAGER_H_ */
