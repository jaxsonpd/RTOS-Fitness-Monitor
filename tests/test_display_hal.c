/**
 * @file test_display.c
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-08-11
 * @brief Test the display module
 */


#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "unity.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "OrbitOLEDInterface_mock.h"

#include "display_hal.h"

void reset_fff(void) {
    FFF_ORBIT_OLED_INPUT_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

void ensure_oled_display_matches(char* expected) {
    char* oled_input = calloc(17, sizeof(char));
    strcpy(oled_input, OLEDStringDraw_fake.arg0_val);

    for (uint8_t i = 0; i < 16; i++) {
        TEST_ASSERT_EQUAL(expected[i], oled_input[i]);
    }

    free(oled_input);
}

void setUp(void) {
    reset_fff();
}

void tearDown(void) {
}


// ========================== Tests ========================
void test_display_init_calls_oled(void) {
    display_init();

    TEST_ASSERT_EQUAL(1, OLEDInitialise_fake.call_count);
}

void test_disply_init_successful(void) {
    TEST_ASSERT(display_init());
}

void test_display_line_calls_OLED_draw(void) {
    display_line("Tests", 0, ALIGN_LEFT);

    TEST_ASSERT_EQUAL(1, OLEDStringDraw_fake.call_count);
}

void test_display_line_correct_row(void) {
    display_line("Tests", 0, ALIGN_LEFT);

    TEST_ASSERT_EQUAL(0, OLEDStringDraw_fake.arg2_val);
}

void test_display_line_aligns_center(void) {
    display_line("Tests", 0, ALIGN_CENTRE);

    char* expected = "     Tests      ";
    char oled_input[17] = { 0 };
    memcpy(oled_input, OLEDStringDraw_fake.arg0_val, 17);

    for (uint8_t i = 0; i < 16; i++) {
        TEST_ASSERT_EQUAL(expected[i], oled_input[i]);
    }
}

void test_display_line_aligns_left(void) {
    display_line("Tests", 0, ALIGN_LEFT);

    char* expected = "Tests           ";
    char oled_input[17] = { 0 };
    memcpy(oled_input, OLEDStringDraw_fake.arg0_val, 17);

    for (uint8_t i = 0; i < 16; i++) {
        TEST_ASSERT_EQUAL(expected[i], oled_input[i]);
    }
}

void test_display_line_aligns_right(void) {
    display_line("Tests", 0, ALIGN_RIGHT);

    char* expected = "           Tests";
    char oled_input[17] = { 0 };
    memcpy(oled_input, OLEDStringDraw_fake.arg0_val, 17);

    for (uint8_t i = 0; i < 16; i++) {
        TEST_ASSERT_EQUAL(expected[i], oled_input[i]);
    }
}


void test_display_time_formats_minutes(void) {
    display_time("Time:", 100, 0, ALIGN_LEFT);
    
    char* expected = "Time: 1:40      ";
    char oled_input[17] = { 0 };
    memcpy(oled_input, OLEDStringDraw_fake.arg0_val, 17);

    for (uint8_t i = 0; i < 16; i++) {
        TEST_ASSERT_EQUAL(expected[i], oled_input[i]);
    }
}

void test_display_time_formats_hours(void) {
    display_time("Time:", 3660, 0, ALIGN_LEFT);
    
    char* expected = "Time: 1:01:00   ";
    char oled_input[17] = { 0 };
    memcpy(oled_input, OLEDStringDraw_fake.arg0_val, 17);

    for (uint8_t i = 0; i < 16; i++) {
        TEST_ASSERT_EQUAL(expected[i], oled_input[i]);
    }
}

void test_display_value_formats_correct(void) {
    display_value("Value:", "m", 2, 0, ALIGN_LEFT, false);

    char* expected = "Value: 2 m      ";
    char oled_input[17] = { 0 };
    memcpy(oled_input, OLEDStringDraw_fake.arg0_val, 17);

    for (uint8_t i = 0; i < 16; i++) {
        TEST_ASSERT_EQUAL(expected[i], oled_input[i]);
    }
}

void test_display_value_formats_correct_thousands(void) {
    display_value("Value:", "m", 2, 0, ALIGN_LEFT, true);

    char* expected = "Value: 0.002 m      ";
    char oled_input[17] = { 0 };
    memcpy(oled_input, OLEDStringDraw_fake.arg0_val, 17);

    for (uint8_t i = 0; i < 16; i++) {
        TEST_ASSERT_EQUAL(expected[i], oled_input[i]);
    }
}