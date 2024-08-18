/** 
 * @file test_device_manager.c
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-08-13
 * @brief Test the device manager module
 */


#include <stdint.h>
#include <stdbool.h>

#include "unity.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "freertos_mocks/task_mock.h"
#include "comms_mocks/step_counter_comms_mock.h"

#include "device_manager.h"

void reset_fff(void) {
    FFF_TASK_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

void setup(void) {
    reset_fff();
}

void tearDown(void) {

}
