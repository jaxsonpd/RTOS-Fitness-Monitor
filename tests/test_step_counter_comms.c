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

void test_step_counter_comms_init_fail(void) {
    SemaphoreHandle_t return_seq[1] = { NULL };
    SET_RETURN_SEQ(xQueueCreateMutex, return_seq, 1);

    TEST_ASSERT_FALSE(step_counter_comms_init());
}

void test_step_counter_comms_init_creates_mutex(void) {
    step_counter_comms_init();

    TEST_ASSERT_EQUAL(1, xQueueCreateMutex_fake.call_count);
}

void test_step_counter_set_succeeds(void) {
    uint32_t dummy_steps = 0;
    SemaphoreHandle_t return_seq[1] = { (QueueHandle_t)1 };
    SET_RETURN_SEQ(xQueueCreateMutex, return_seq, 1);
    step_counter_comms_init();

    TEST_ASSERT_TRUE(step_counter_set(dummy_steps, true));
}

void test_step_counter_set_increments(void) {

    uint32_t dummy_steps = 5;
    SemaphoreHandle_t return_seq[1] = { (QueueHandle_t)1 };
    SET_RETURN_SEQ(xQueueCreateMutex, return_seq, 1);
    step_counter_comms_init();
    step_counter_set(dummy_steps, true);
    
    TEST_ASSERT_EQUAL(dummy_steps, step_counter_get());
}

void test_step_counter_get_before_init(void) {
    TEST_ASSERT_EQUAL(0, step_counter_get());
}

void test_step_counter_get_resets_count(void) {
    uint32_t dummy_steps = 5;
    SemaphoreHandle_t return_seq[1] = { (QueueHandle_t)1 };
    SET_RETURN_SEQ(xQueueCreateMutex, return_seq, 1);
    step_counter_comms_init();
    step_counter_set(dummy_steps, true);
    
    TEST_ASSERT_EQUAL(dummy_steps, step_counter_get());
    TEST_ASSERT_EQUAL(0, step_counter_get());
}

void test_step_counter_set_accumulates_multiple(void) {
    uint32_t dummy_steps = 5;
    SemaphoreHandle_t return_seq[1] = { (QueueHandle_t)1 };
    SET_RETURN_SEQ(xQueueCreateMutex, return_seq, 1);
    step_counter_comms_init();
    step_counter_set(dummy_steps, true);
    step_counter_set(dummy_steps, true);
    
    TEST_ASSERT_EQUAL(dummy_steps + dummy_steps, step_counter_get());
}