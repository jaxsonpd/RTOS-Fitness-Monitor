#include <stdint.h>
#include <stdbool.h>

#include "unity.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "pot_manager.h"

#include "comms_mocks/pot_comms_mock.h"
#include "adc_handler_mock.h"

#include "freertos_mocks/task_mock.h"

void reset_fff(void)
{
    FFF_POT_COMMS_FAKES_LIST(RESET_FAKE);
    FFF_ADC_HANDLER_FAKES_LIST(RESET_FAKE);
    FFF_TASK_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

void setUp(void)
{
    reset_fff();
}

void tearDown(void)
{
    
}

void test_pot_manager_init_calls_comms_init(void)
{
    pot_read_init();

    TEST_ASSERT_EQUAL(1, pot_comms_init_fake.call_count);
}