#include "math.h"
#include "potentiometer_reader.h"
#include "ADC_read.h"

#include "FreeRTOS.h"
#include "task.h"

#define POT_SCALE_COEFF 20000 / 4095
#define STEP_GOAL_ROUNDING 100

// void updateNewGoal(void) {
//     deviceStateInfo_t* deviceState = getDeviceState();

//     int32_t adc_reading = readADC();
//     if (adc_reading != 0) {
//         deviceState->newGoal = adc_reading * POT_SCALE_COEFF;
//         deviceState->newGoal = (deviceState->stateGoal / STEP_GOAL_ROUNDING) * STEP_GOAL_ROUNDING;
//     }

//     if (deviceState->newGoal == 0) {
//         deviceState->newGoal = STEP_GOAL_ROUNDING;
//     }
// }

bool potentiometer_init(void) {
    bool result = true;
    result = result && initADC();
    result = result && potentiometer_init();
}

void potentiometer_thread(void *args) 
{
    if (potentiometer_init() == false) {
        while(1);
    }

}