/**
 * @file test_input_comms.c
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-08-08
 * @brief Test the input comms model
 */

#include <stdint.h>
#include <stdbool.h>

#include "unity.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "queue_mock.h"

#include "input_comms.h"

void setUp(void) {
}

void tearDown(void) {
}

// ========================== Tests ========================
void test_input_comms_init_succesful(void) {
    QueueHandle_t return_seq[1] = { (QueueHandle_t)1 };

    SET_RETURN_SEQ(xQueueGenericCreate, return_seq, 1);
    TEST_ASSERT_TRUE(input_comms_init());
}

void test_input_comms_init_creates_queue(void) {
    input_comms_init();

    TEST_ASSERT_EQUAL(1, xQueueGenericCreate_fake.call_count);
}

void test_input_comms_init_correct_length(void) {
    input_comms_init();

    TEST_ASSERT_EQUAL(5, xQueueGenericCreate_fake.arg0_val);
}

void test_input_comms_init_correct_size(void) {
    input_comms_init();

    TEST_ASSERT_EQUAL(sizeof(inputCommMsg_t), xQueueGenericCreate_fake.arg1_val);
}

void test_input_comms_send_calls_queue_send(void) {

    input_comms_send(SCREEN_LEFT);

    TEST_ASSERT_EQUAL(1, xQueueGenericSend_fake.call_count);
}

void test_input_comms_send_sends_correct_value(void) {
    input_comms_send(SCREEN_LEFT);

    TEST_ASSERT_EQUAL(SCREEN_LEFT, xQueueGenericSend_fake.arg1_val);
}

void test_input_comms_send_sends_without_delay(void) {
    input_comms_send(SCREEN_LEFT);

    TEST_ASSERT_EQUAL(0, xQueueGenericSend_fake.arg2_val);
}

void test_input_comms_send_successful(void) {
    BaseType_t return_seq[1] = { pdTRUE };

    SET_RETURN_SEQ(xQueueGenericSend, return_seq, 1);

    TEST_ASSERT_TRUE(input_comms_send(SCREEN_LEFT));
}

void test_input_comms_receive_call_queue_receive(void) {
    input_comms_receive();

    TEST_ASSERT_EQUAL(1, xQueueReceive_fake.call_count);
}

void test_input_comms_receive_type_converts(void) {
    TEST_IGNORE();
}

void test_input_comms_receive_non_blocking(void) {
    input_comms_receive();

    TEST_ASSERT_EQUAL(0, xQueueReceive_fake.arg2_val);
}
