#include <stdint.h>
#include <stdbool.h>

#include "unity.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "pot_manager.h"

#include "adc_manager_mock.h"
#include "comms_mocks/pot_comms_mock.h"

#include "freertos_mocks/task_mock.h"

void reset_fff(void)
{
    FFF_POT_COMMS_FAKES_LIST(RESET_FAKE);
    FFF_ADC_MANAGER_FAKES_LIST(RESET_FAKE);
    FFF_TASK_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

