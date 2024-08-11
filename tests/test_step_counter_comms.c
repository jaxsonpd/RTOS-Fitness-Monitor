#include <stdint.h>
#include <stdbool.h>

#include "unity.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "freertos_mocks/semphr_mock.h"
#include "freertos_mocks/queue_mock.h"

#include "step_counter_comms.h"

void setUp(void) {
}

void tearDown(void) {
}

void test_step_counter_comms_init_succesful(void) {
    SemaphoreHandle_t return_seq[1] = { (QueueHandle_t)1 };

    SET_RETURN_SEQ(xQueueCreateMutex, return_seq, 1);
    TEST_ASSERT_TRUE(step_counter_comms_init());
}

void test_step_counter_comms_init_creates_mutex(void) {
    step_counter_comms_init();

    TEST_ASSERT_EQUAL(1, xQueueCreateMutex_fake.call_count);
}

// void test_input_comms_init_correct_size(void) {
//     input_comms_init();

//     TEST_ASSERT_EQUAL(sizeof(inputCommMsg_t), xQueueGenericCreate_fake.arg1_val);
// }

// void test_input_comms_send_calls_queue_send(void) {

//     input_comms_send(MSG_SCREEN_LEFT);

//     TEST_ASSERT_EQUAL(1, xQueueGenericSend_fake.call_count);
// }

// void test_input_comms_send_sends_correct_value(void) {
//     TEST_IGNORE(); // Passes pointer so doesn't work anymore
//     input_comms_send(MSG_SCREEN_LEFT);

//     TEST_ASSERT_EQUAL(MSG_SCREEN_LEFT, xQueueGenericSend_fake.arg1_val);
// }

// void test_input_comms_send_sends_without_delay(void) {
//     input_comms_send(MSG_SCREEN_LEFT);

//     TEST_ASSERT_EQUAL(0, xQueueGenericSend_fake.arg2_val);
// }

// void test_input_comms_send_successful(void) {
//     BaseType_t return_seq[1] = { pdTRUE };

//     SET_RETURN_SEQ(xQueueGenericSend, return_seq, 1);

//     TEST_ASSERT_TRUE(input_comms_send(MSG_SCREEN_LEFT));
// }

// void test_input_comms_receive_call_queue_receive(void) {
//     input_comms_receive();

//     TEST_ASSERT_EQUAL(1, xQueueReceive_fake.call_count);
// }

// void test_input_comms_receive_type_converts(void) {
//     TEST_IGNORE();
// }

// void test_input_comms_receive_non_blocking(void) {
//     input_comms_receive();

//     TEST_ASSERT_EQUAL(0, xQueueReceive_fake.arg2_val);
// }

// void test_input_comms_receive_returns_no_messages(void) {
//     BaseType_t return_seq[1] = { pdFALSE };

//     SET_RETURN_SEQ(xQueueReceive, return_seq, 1);

//     TEST_ASSERT_EQUAL(NO_MESSAGES, input_comms_receive());
// }

// void test_input_comms_num_msg_calls_queue_length(void) {
//     input_comms_num_msgs();

//     TEST_ASSERT_EQUAL(1, uxQueueMessagesWaiting_fake.call_count);
// }