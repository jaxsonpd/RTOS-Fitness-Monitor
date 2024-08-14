#include "math.h"
#include "assert.h"

#include "step_counter_manager.h"

#include "serial_sender.h"
#include "accl_manager.h"
#include "step_counter.h"
#include "step_counter_comms.h"
#include "vector3.h"

#include "FreeRTOS.h"
#include "task.h"

#define SERIAL_PLOTTING_ENABLED

static uint16_t step_counter_period = 0;
/** 
 * @brief Initialise the accelerometer manager thread
 * 
 * @return true if successful
 */
bool step_counter_manager_init(uint16_t* p_step_counter_period) 
{
    #ifdef SERIAL_PLOTTING_ENABLED
    SerialInit ();
    #endif // SERIAL_PLOTTING_ENABLED
    bool result = true;
    result = accl_init() && result;
    result = step_counter_comms_init() && result;
    result = step_counter_init(p_step_counter_period) && result;
    return result;
}

void step_counter_thread(void* args) 
{
    assert(step_counter_manager_init(&step_counter_period));
    assert(step_counter_period > 0);

    vector3_t acceleration;
    uint32_t stepsAccumulated = 0;
    
    TickType_t t_last_step_ticks = xTaskGetTickCount();
    for (;;) {
        accl_poll();
        if(xTaskGetTickCount() - t_last_step_ticks >= step_counter_period) {
            t_last_step_ticks = xTaskGetTickCount();
            acceleration = accl_mean();
            if (detect_step(acceleration, &stepsAccumulated)) {
                step_counter_set(stepsAccumulated);
                stepsAccumulated = 0;
            }
        }
        #ifdef SERIAL_PLOTTING_ENABLED
        SerialPlot(acceleration.x,acceleration.y,acceleration.z,t_last_step_ticks);
        #endif // SERIAL_PLOTTING_ENABLED
        vTaskDelay(5);
    }
}