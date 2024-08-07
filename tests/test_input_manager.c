/**
 * @file test_button_manager.h
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-08-04
 * @brief Test the button manger module
 */

#include <stdint.h>
#include <stdbool.h>

#include "unity.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "button_manager.h"

#include "IO_input_mock.h"

void reset_fff(void)
{
    FFF_IO_INPUT_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

void setUp(void)
{
    reset_fff();
}

void tearDown(void)
{
}

void test_button_manager_init_calls(void)
{
    btnInit();

    TEST_ASSERT_EQUAL(1, input_init_fake.call_count);
}

void test_button_manager_update_calls(void)
{
    TEST_IGNORE();
}
