#include "math.h"

#include "step_counter_manager.h"

#include "accl_manager.h"
#include "step_counter.h"
#include "step_counter_comms.h"
#include "serial_sender.h"

#include "FreeRTOS.h"
#include "task.h"

/** 
 * @brief Initialise the accelerometer manager thread
 * 
 * @return true if successful
 */
bool step_counter_manager_init(void) 
{
    bool result = true;
    result = acclInit() && result;
    result = step_counter_comms_init() && result;
    result = step_counter_init() && result;
    return result;
}

void step_counter_thread(void* args) 
{
    if(step_counter_manager_init() == false) {
        while(1);
    }
    vector3_t acceleration = {0};
    uint32_t stepsAccumulated = 0;
    bool stepChange = false;
    
    TickType_t t_stepTicks = xTaskGetTickCount();
    for (;;) {
        acclPoll();
        if(xTaskGetTickCount() - t_stepTicks >= 50) {
            acceleration = acclMean();
            stepChange = detect_step(acceleration, &stepsAccumulated);
            t_stepTicks = xTaskGetTickCount();
            #ifdef SERIAL_PLOTTING_ENABLED
            SerialPlot(acceleration.x,acceleration.y,acceleration.z,stepsAccumulated);
            #endif // SERIAL_PLOTTING_ENABLED
        }
        if (stepChange == true) {
            step_counter_set(stepsAccumulated);
            stepsAccumulated = 0;
        }
        vTaskDelay(5);
    }
}