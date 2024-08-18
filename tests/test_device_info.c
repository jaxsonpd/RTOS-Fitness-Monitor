/** 
 * @file test_device_info.c
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-08-13
 * @brief Tests for the device_info module
 */


#include <stdint.h>
#include <stdbool.h>

#include "unity.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "freertos_mocks/task_mock.h"

#include "device_info.h"

void reset_fff(void) {
    FFF_TASK_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

void setup(void) {
    reset_fff();
}

void tearDown(void) {

}

// ========================== Tests ========================
void test_device_info_init_debug(void) {
    TEST_ASSERT_FALSE(device_info_get_debug());
}

void test_device_info_init_units(void) {
    TEST_ASSERT_EQUAL(UNITS_SI, device_info_get_units());
}

void test_device_info_init_workout_start_time(void) {
    TEST_ASSERT_EQUAL(0, device_info_get_workout_start_time());
}

void test_device_info_init_input_flags(void) {
    for (uint8_t i = 0; i < NUM_MSGS; i++) {
        TEST_ASSERT_EQUAL(0, device_info_get_input_flag(i));
    }
}

void test_device_info_init_alternate(void) {
    TEST_ASSERT_FALSE(device_info_get_alternate());
}

void test_device_info_init_last_step_time(void) {
    TEST_ASSERT_EQUAL(0, device_info_get_last_step_time());
}

void test_device_info_get_set_debug(void) {
    device_info_set_debug(true);

    TEST_ASSERT_TRUE(device_info_get_debug());
}

void test_device_info_get_set_units(void) {
    device_info_set_units(UNITS_ALTERNATE);

    TEST_ASSERT_EQUAL(UNITS_ALTERNATE, device_info_get_units());
}

void test_device_info_get_set_workout_start(void) {
    device_info_set_workout_start_time(1000);

    TEST_ASSERT_EQUAL(1000, device_info_get_workout_start_time());
}

void test_device_info_get_set_input_flags(void) {
    for (uint8_t i = 0; i < NUM_MSGS; i++) {
        device_info_set_input_flag(i, 1);
    }

    for (uint8_t i = 0; i < NUM_MSGS; i++) {
        TEST_ASSERT_EQUAL(1, device_info_get_input_flag(i));
    }
}

void test_device_info_get_set_alternate(void) {
    device_info_set_alternate(true);

    TEST_ASSERT_TRUE(device_info_get_alternate());
}

void test_device_info_get_set_last_step_time(void) {
    device_info_set_last_step_time(1000);

    TEST_ASSERT_EQUAL(1000, device_info_get_last_step_time());
}

void test_device_info_test_reset(void) {
    for (uint8_t i = 0; i < NUM_MSGS; i++) {
        device_info_set_input_flag(i, 1);
    }

    device_info_set_alternate(true);
    device_info_set_debug(true);
    device_info_set_last_step_time(1000);
    device_info_set_units(UNITS_ALTERNATE);
    device_info_set_workout_start_time(1000);

    device_info_reset();

    for (uint8_t i = 0; i < NUM_MSGS; i++) {
        TEST_ASSERT_EQUAL(0, device_info_get_input_flag(i));
    }

    TEST_ASSERT_FALSE(device_info_get_debug());
    TEST_ASSERT_EQUAL(UNITS_SI, device_info_get_units());
    TEST_ASSERT_EQUAL(0, device_info_get_workout_start_time());
    TEST_ASSERT_FALSE(device_info_get_alternate());
    TEST_ASSERT_EQUAL(0, device_info_get_last_step_time());
    TEST_ASSERT_FALSE(device_info_get_alternate());
}

void test_device_info_get_time(void) {
    uint32_t return_seq[1] = {1000};

    SET_RETURN_SEQ(xTaskGetTickCount, return_seq, 1);

    TEST_ASSERT_EQUAL(10, device_info_get_ds());
}