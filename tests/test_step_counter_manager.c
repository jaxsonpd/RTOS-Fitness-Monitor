#include <stdint.h>
#include <stdbool.h>

#include "unity.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "step_counter_manager.h"

#include "math_mock.h"
#include "step_counter_mock.h"
#include "step_counter_comms_mock.h"
#include "accl_manager_mock.h"
#include "freertos_mocks/task_mock.h"

void reset_fff(void)
{
    FFF_STEP_COUNTER_COMMS_FAKES_LIST(RESET_FAKE);
    FFF_ACCL_MANAGER_FAKES_LIST(RESET_FAKE);
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

void test_step_counter_manager_init_calls_comms_init(void)
{
    step_counter_manager_init();

    TEST_ASSERT_EQUAL(1, step_counter_comms_init_fake.call_count);
}

void test_step_counter_manager_init_calls_accl_init(void)
{
    step_counter_manager_init();

    TEST_ASSERT_EQUAL(1, acclInit_fake.call_count);
}

void test_step_counter_manager_init_calls_step_counter_init(void)
{
    step_counter_manager_init();

    TEST_ASSERT_EQUAL(1, step_counter_init_fake.call_count);
}

void test_step_counter_manager_init_success(void)
{
    bool return_seq[1] = {true};

    SET_RETURN_SEQ(step_counter_comms_init, return_seq, 1);
    SET_RETURN_SEQ(acclInit, return_seq, 1);
    SET_RETURN_SEQ(step_counter_init, return_seq, 1);

    TEST_ASSERT_TRUE(step_counter_manager_init());
}
