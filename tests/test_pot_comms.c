#include <stdint.h>
#include <stdbool.h>

#include "unity.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "freertos_mocks/semphr_mock.h"
#include "freertos_mocks/queue_mock.h"

#include "comms/pot_comms.h"

void setUp(void) {
}

void tearDown(void) {
}

void test_pot_comms_init_successful(void) {
    SemaphoreHandle_t return_seq[1] = { (QueueHandle_t)1 };

    SET_RETURN_SEQ(xQueueCreateMutex, return_seq, 1);
    TEST_ASSERT_TRUE(pot_comms_init());
}

void test_pot_comms_init_fail(void) {
    SemaphoreHandle_t return_seq[1] = { NULL };
    SET_RETURN_SEQ(xQueueCreateMutex, return_seq, 1);

    TEST_ASSERT_FALSE(pot_comms_init());
}

void test_pot_comms_init_creates_mutex(void) {
    pot_comms_init();

    TEST_ASSERT_EQUAL(1, xQueueCreateMutex_fake.call_count);
}

void test_pot_set_succeeds(void) {
    uint32_t dummy_pot = 0;
    SemaphoreHandle_t return_seq[1] = { (QueueHandle_t)1 };
    SET_RETURN_SEQ(xQueueCreateMutex, return_seq, 1);
    pot_comms_init();

    TEST_ASSERT_TRUE(pot_set(dummy_pot));
}

void test_pot_set_increments(void) {

    uint32_t dummy_pot = 5;
    SemaphoreHandle_t return_seq[1] = { (QueueHandle_t)1 };
    SET_RETURN_SEQ(xQueueCreateMutex, return_seq, 1);
    pot_comms_init();
    pot_set(dummy_pot);
    
    TEST_ASSERT_EQUAL(dummy_pot, pot_get());
}

void test_pot_get_before_init(void) {
    TEST_ASSERT_EQUAL(0, pot_get());
}

void test_pot_get_resets_count(void) {
    uint32_t dummy_pot = 5;
    SemaphoreHandle_t return_seq[1] = { (QueueHandle_t)1 };
    SET_RETURN_SEQ(xQueueCreateMutex, return_seq, 1);
    pot_comms_init();
    pot_set(dummy_pot);
    
    TEST_ASSERT_EQUAL(dummy_pot, pot_get());
    TEST_ASSERT_EQUAL(0, pot_get());
}

// void test_pot_set_accumulates_multiple(void) {
//     uint32_t dummy_pot = 5;
//     SemaphoreHandle_t return_seq[1] = { (QueueHandle_t)1 };
//     SET_RETURN_SEQ(xQueueCreateMutex, return_seq, 1);
//     pot_comms_init();
//     pot_set(dummy_pot);
//     pot_set(dummy_pot);
    
//     TEST_ASSERT_EQUAL(dummy_pot + dummy_pot, pot_get());
// }
