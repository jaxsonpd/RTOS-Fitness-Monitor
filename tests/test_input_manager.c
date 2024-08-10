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

#include "input_manager.h"

#include "IO_input_mock.h"
#include "input_comms_mock.h"
#include "freertos_mocks/task_mock.h"

void reset_fff(void) {
    FFF_INPUT_COMMS_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
    FFF_IO_INPUT_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
    FFF_TASK_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

void setUp(void) {
    reset_fff();
}

void tearDown(void) {
}

void test_input_manager_init_calls_comms(void) {
    input_manager_init();

    TEST_ASSERT_EQUAL(1, input_comms_init_fake.call_count);
}

void test_input_manager_init_calls_IO(void) {
    input_manager_init();

    TEST_ASSERT_EQUAL(1, input_init_fake.call_count);
}

void test_input_manager_init_succseful(void) {
    bool return_seq[1] = { true };

    SET_RETURN_SEQ(input_init, return_seq, 1);
    SET_RETURN_SEQ(input_comms_init, return_seq, 1);

    TEST_ASSERT_TRUE(input_manager_init());
}

void test_input_manager_update_calls(void) {
    TEST_IGNORE();
}
