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
#include "tiva_mocks/ustdlib_mock.h"

#include "display_hal.h"

void reset_fff(void) {
    FFF_ORBIT_OLED_INPUT_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
    FFF_USTDLIB_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
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

    const char* oled_input = malloc(sizeof(char) * 17);
    memcpy(oled_input, OLEDStringDraw_fake.arg0_val, 17);

    char* expected = "     Tests      ";
    for (uint8_t i = 0; i < 16; i++) {
        TEST_ASSERT_EQUAL(expected[i], oled_input[i]);
    }
}

void test_display_line_aligns_left(void) {
    display_line("Tests", 0, ALIGN_LEFT);

    const char* oled_input = malloc(sizeof(char) * 17);
    memcpy(oled_input, OLEDStringDraw_fake.arg0_val, 17);

    char* expected = "Tests           ";
    for (uint8_t i = 0; i < 16; i++) {
        TEST_ASSERT_EQUAL(expected[i], oled_input[i]);
    }
}

void test_display_line_aligns_right(void) {
    display_line("Tests", 0, ALIGN_RIGHT);

    const char* oled_input = malloc(sizeof(char) * 17);
    memcpy(oled_input, OLEDStringDraw_fake.arg0_val, 17);

    char* expected = "           Tests";
    for (uint8_t i = 0; i < 16; i++) {
        TEST_ASSERT_EQUAL(expected[i], oled_input[i]);
    }
}
