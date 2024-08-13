#include "math.h"
#include "assert.h"

#include "step_counter_manager.h"

#include "accl_manager.h"
#include "step_counter.h"
#include "step_counter_comms.h"
#include "vector3.h"

#include "FreeRTOS.h"
#include "task.h"

static uint16_t step_counter_period = 0;
/** 
 * @brief Initialise the accelerometer manager thread
 * 
 * @return true if successful
 */
bool step_counter_manager_init(uint16_t* p_step_counter_period) 
{
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
    bool stepChange = false;
    
    TickType_t t_last_step_ticks = xTaskGetTickCount();
    for (;;) {
        accl_poll();
        if(xTaskGetTickCount() - t_last_step_ticks >= step_counter_period) {
            t_last_step_ticks = xTaskGetTickCount();
            acceleration = accl_mean();
            stepChange = detect_step(acceleration, &stepsAccumulated);
        }
        if (stepChange == true) {
            step_counter_set(stepsAccumulated);
            stepsAccumulated = 0;
        }
        vTaskDelay(5);
    }
}