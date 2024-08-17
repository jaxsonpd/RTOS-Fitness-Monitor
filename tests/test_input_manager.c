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

#include "hal_mocks/input_hal_mock.h"
#include "comms_mocks/input_comms_mock.h"
#include "freertos_mocks/task_mock.h"

void reset_fff(void) {
    FFF_INPUT_COMMS_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
    FFF_INPUT_HAL_FAKES_LIST(RESET_FAKE);
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

void test_input_manager_init_successful(void) {
    bool return_seq[1] = { true };

    SET_RETURN_SEQ(input_init, return_seq, 1);
    SET_RETURN_SEQ(input_comms_init, return_seq, 1);

    TEST_ASSERT_TRUE(input_manager_init());
}

void test_input_manager_init_calls_update(void) {
    input_manager_init();

    TEST_ASSERT_EQUAL(DEBOUNCE_NUMBER, input_update_fake.call_count);
}

void test_input_manager_init_sends_switches(void) {
    bool return_seq[1] = { true };

    SET_RETURN_SEQ(input_get, return_seq, 1);

    input_manager_init();

    TEST_ASSERT_EQUAL(MSG_RIGHT_SWITCH_ON, input_comms_send_fake.arg0_history[0]);
    TEST_ASSERT_EQUAL(MSG_LEFT_SWITCH_ON, input_comms_send_fake.arg0_history[1]);
}

void test_input_manager_send_updates_sends_pushed(void) {
    inputState_t return_seq[1] = { PUSHED };

    SET_RETURN_SEQ(input_check, return_seq, 1);

    input_manager_send_updates();

    TEST_ASSERT_EQUAL(MSG_SCREEN_LEFT, input_comms_send_fake.arg0_history[0]);
    TEST_ASSERT_EQUAL(MSG_SCREEN_RIGHT, input_comms_send_fake.arg0_history[1]);
    TEST_ASSERT_EQUAL(MSG_UP_BUTTON_P, input_comms_send_fake.arg0_history[2]);
    TEST_ASSERT_EQUAL(MSG_DOWN_BUTTON_P, input_comms_send_fake.arg0_history[3]);
    TEST_ASSERT_EQUAL(MSG_RIGHT_SWITCH_ON, input_comms_send_fake.arg0_history[4]);
    TEST_ASSERT_EQUAL(MSG_LEFT_SWITCH_ON, input_comms_send_fake.arg0_history[5]);
}

void test_input_manager_send_updates_sends_released(void) {
    inputState_t return_seq[1] = { RELEASED };

    SET_RETURN_SEQ(input_check, return_seq, 1);

    input_manager_send_updates();

    TEST_ASSERT_EQUAL(MSG_UP_BUTTON_R, input_comms_send_fake.arg0_history[0]);
    TEST_ASSERT_EQUAL(MSG_DOWN_BUTTON_R, input_comms_send_fake.arg0_history[1]);
    TEST_ASSERT_EQUAL(MSG_RIGHT_SWITCH_OFF, input_comms_send_fake.arg0_history[2]);
    TEST_ASSERT_EQUAL(MSG_LEFT_SWITCH_OFF, input_comms_send_fake.arg0_history[3]);
}