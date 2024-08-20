#include <stdint.h>
#include <stdbool.h>

#include "unity.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "step_counter_manager.h"

#include "math_mock.h"
#include "step_counter_mock.h"
#include "comms_mocks/step_counter_comms_mock.h"
#include "accl_handler_mock.h"
#include "freertos_mocks/task_mock.h"

void reset_fff(void)
{
    FFF_STEP_COUNTER_COMMS_FAKES_LIST(RESET_FAKE);
    FFF_ACCL_HANDLER_FAKES_LIST(RESET_FAKE);
    FFF_MATH_FAKES_LIST(RESET_FAKE);
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

#define DUMMY_PERIOD 100 // ticks
bool step_counter_init_fake_period(uint16_t* arg0)
{
    *arg0 = DUMMY_PERIOD;
}

void test_step_counter_manager_init_calls_comms_init(void)
{
    uint16_t dummy_period;
    step_counter_manager_init(&dummy_period);

    TEST_ASSERT_EQUAL(1, step_counter_comms_init_fake.call_count);
}

void test_step_counter_manager_init_calls_accl_init(void)
{
    uint16_t dummy_period;
    step_counter_manager_init(&dummy_period);

    TEST_ASSERT_EQUAL(1, accl_init_fake.call_count);
}

void test_step_counter_manager_init_calls_step_counter_init(void)
{
    uint16_t dummy_period;
    step_counter_manager_init(&dummy_period);

    TEST_ASSERT_EQUAL(1, step_counter_init_fake.call_count);
}

void test_step_counter_manager_init_gets_period(void)
{
    step_counter_init_fake.custom_fake = step_counter_init_fake_period;
    uint16_t dummy_period;

    step_counter_manager_init(&dummy_period);
    TEST_ASSERT_EQUAL(DUMMY_PERIOD, dummy_period);
}

void test_step_counter_manager_init_success(void)
{
    uint16_t dummy_period;
    bool return_seq[1] = {true};

    SET_RETURN_SEQ(step_counter_comms_init, return_seq, 1);
    SET_RETURN_SEQ(accl_init, return_seq, 1);
    SET_RETURN_SEQ(step_counter_init, return_seq, 1);

    TEST_ASSERT_TRUE(step_counter_manager_init(&dummy_period));
}
