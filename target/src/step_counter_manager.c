#include "math.h"
#include "assert.h"

#include "step_counter_manager.h"

#include "serial_sender.h"
#include "accl_handler.h"
#include "step_counter.h"
#include "comms/step_counter_comms.h"
#include "utility/vector3.h"

#include "FreeRTOS.h"
#include "task.h"

// #define STEP_SERIAL_ENABLED

static uint16_t step_counter_period = 0;

/** 
 * @brief Initialise the accelerometer manager thread
 * @param p_step_counter_period pointer to step counter period from algorithm
 * @return true if successful
 */
bool step_counter_manager_init(uint16_t* p_step_counter_period) {
    #ifdef STEP_SERIAL_ENABLED
    SerialInit ();
    #endif // STEP_SERIAL_ENABLED
    bool result = true;
    result = accl_init() && result;
    result = step_counter_comms_init() && result;
    result = step_counter_init(p_step_counter_period) && result;
    return result;
}

void step_counter_thread(void* args) {
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
                step_counter_set(stepsAccumulated, true);
                stepsAccumulated = 0;
            }
        }
        #ifdef STEP_SERIAL_ENABLED
        SerialPlot(acceleration.x,acceleration.y,acceleration.z,t_last_step_ticks);
        #endif // STEP_SERIAL_ENABLED
        vTaskDelay(5);
    }
}