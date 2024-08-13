#include "math.h"
#include "step_counter.h"

static bool stepping = false;

bool detect_step(vector3_t acceleration, uint32_t *stepsAccumulated) 
{
    bool stepRegistered = false;
    uint16_t magnitude = (uint16_t)sqrt(acceleration.x * acceleration.x + acceleration.y * acceleration.y + acceleration.z * acceleration.z);
    
    // Convert delay to ticks
    
    if (stepping == false && magnitude >= STEP_THRESHOLD_HIGH) {
        stepping = true;
        (*stepsAccumulated)++;
        stepRegistered = true;
    } else if (magnitude <= STEP_THRESHOLD_LOW) {
        stepping = false;
    }
    
    return stepRegistered;
}