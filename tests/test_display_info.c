/** 
 * @file test_display_info.c
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-08-13
 * @brief Tests for the display_info module
 */


#include <stdint.h>
#include <stdbool.h>

#include "unity.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "freertos_mocks/task_mock.h"

#include "display_info.h"

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
void test_display_info_init_steps(void) {
    TEST_ASSERT_EQUAL(0, display_info_get_steps());
}

void test_display_info_init_debug(void) {
    TEST_ASSERT_FALSE(display_info_get_debug());
}

void test_display_info_init_units(void) {
    TEST_ASSERT_EQUAL(UNITS_SI, display_info_get_units());
}

void test_display_info_init_goal(void) {
    TEST_ASSERT_EQUAL(1000, display_info_get_goal());
}

void test_display_info_init_workout_start_time(void) {
    TEST_ASSERT_EQUAL(0, display_info_get_start());
}

void test_display_info_init_input_flags(void) {
    for (uint8_t i = 0; i < NUM_MSGS; i++) {
        TEST_ASSERT_EQUAL(0, display_info_get_input_flag(i));
    }
}

void test_display_info_get_set_debug(void) {
    display_info_set_debug(true);

    TEST_ASSERT_TRUE(display_info_get_debug());
}

void test_display_info_get_set_units(void) {
    display_info_set_units(UNITS_ALTERNATE);

    TEST_ASSERT_EQUAL(UNITS_ALTERNATE, display_info_get_units());
}

void test_display_info_get_set_steps(void) {
    display_info_set_steps(1000);

    TEST_ASSERT_EQUAL(1000, display_info_get_steps());
}

void test_display_info_get_set_goal(void) {
    display_info_set_goal(5000);

    TEST_ASSERT_EQUAL(5000, display_info_get_goal());
}

void test_display_info_get_set_start(void) {
    display_info_set_start(1000);

    TEST_ASSERT_EQUAL(1000, display_info_get_start());
}

void test_display_info_get_set_input_flags(void) {
    for (uint8_t i = 0; i < NUM_MSGS; i++) {
        display_info_set_input_flag(i, 1);
    }

    for (uint8_t i = 0; i < NUM_MSGS; i++) {
        TEST_ASSERT_EQUAL(1, display_info_get_input_flag(i));
    }
}