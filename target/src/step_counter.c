#include "math.h"

#include "step_counter.h"
#include "accl_manager.h"
#include "step_counter_comms.h"
#include "serial_sender.h"

#include "FreeRTOS.h"
#include "task.h"

#define STEP_THRESHOLD_HIGH 300
#define STEP_THRESHOLD_LOW 250
#define STEP_DELAY_MS 1000       // 1 second in milliseconds

static TickType_t lastStepTime = 0; // Timestamp of the last step registration
static bool stepping = false;

bool detect_step(vector3_t acceleration, uint32_t *stepsAccumulated) 
{
    bool stepRegistered = false;
    TickType_t currentTime = xTaskGetTickCount();
    uint16_t magnitude = (uint16_t)sqrt(acceleration.x * acceleration.x + acceleration.y * acceleration.y + acceleration.z * acceleration.z);
    
    // Convert delay to ticks
    TickType_t stepDelayTicks = pdMS_TO_TICKS(STEP_DELAY_MS);
    
    if (stepping == false && magnitude >= STEP_THRESHOLD_HIGH) {
        if ((currentTime - lastStepTime) >= stepDelayTicks) {
            // It's been more than 1 second since the last step was registered
            stepping = true;
            (*stepsAccumulated)++;
            lastStepTime = currentTime;
            stepRegistered = true;
        }
    } else if (magnitude <= STEP_THRESHOLD_LOW) {
        stepping = false;
    }
    
    return stepRegistered;
}

/** 
 * @brief Initialise the accelerometer manager thread
 * 
 * @return true if successful
 */
bool step_counter_init(void) 
{
    bool result = true;
    result = result && acclInit();
    result = result && step_counter_comms_init();
    return result;
}

void step_counter_thread(void* args) 
{
    if(step_counter_init() == false) {
        while(1);
    }
    vector3_t acceleration = {0};
    uint32_t stepsAccumulated = 0;
    bool stepChange = false;
    
    for (;;) {
        acclPoll();
        acceleration = acclMean();
        stepChange = detect_step(acceleration, &stepsAccumulated);
        if (stepChange == true) {
            step_counter_set(stepsAccumulated);
            stepsAccumulated = 0;
        }
        vTaskDelay(5);
    }
}