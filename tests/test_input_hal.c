/**
 * @file test_IO_input.c
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-07-31
 * @brief Tests the IO input button module which gets data from the GPIO
 */

#include <stdint.h>
#include <stdbool.h>

#include "unity.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "tiva_mocks/gpio_mock.h"
#include "tiva_mocks/sysctl_mock.h"

#include "inc/tm4c123gh6pm.h" // Board specific defines (for PF0)

#include "input_hal.h"

#define INPUTS_ON_LIST 0x00000001, 0x00000004, 0x00000000, 0x00000000, 0x00000080, 0x00000040
#define INPUTS_OFF_LIST 0x00000000, 0x00000000, 0x00000010, 0x00000001, 0x00000000, 0x00000000

bool auto_input_init(void) {
    bool return_seq[1] = {true};

    SET_RETURN_SEQ(SysCtlPeripheralReady, return_seq, 1);

    return input_init();
}

void reset_fff(void) {
    FFF_GPIO_FAKES_LIST(RESET_FAKE);
    FFF_SYSCTL_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

void setUp(void) {
    reset_fff();
}

void tearDown(void) {
}

// ================ Tests ================
void test_input_hal_pin_type_set_calls(void) {
    // arrange

    // act
    auto_input_init();

    // assert
    TEST_ASSERT_EQUAL(NUM_INPUT_MODES, GPIOPinTypeGPIOInput_fake.call_count);
}

void test_input_hal_pad_config_set_calls(void) {
    auto_input_init();

    TEST_ASSERT_EQUAL(NUM_INPUT_MODES, GPIOPadConfigSet_fake.call_count);
}

void test_input_hal_peripheral_enable_calls(void) {
    auto_input_init();

    TEST_ASSERT_EQUAL(NUM_INPUT_MODES, SysCtlPeripheralEnable_fake.call_count);
}

void test_input_hal_init_successful(void) {
    TEST_ASSERT_TRUE(auto_input_init());
}

void test_input_hal_get_false_on_init(void) {
    auto_input_init();

    for (uint32_t i = 0; i < NUM_INPUT_MODES; i++) {
        TEST_ASSERT_FALSE(input_get(i));
    }
}

void test_input_hal_check_no_change_on_init(void) {
    auto_input_init();

    for (uint32_t i = 0; i < NUM_INPUT_MODES; i++) {
        TEST_ASSERT_EQUAL(NO_CHANGE, input_check(i));
    }
}

void test_input_hal_correct_eventually(void) {
    uint32_t return_seq[NUM_INPUT_MODES*5] = { INPUTS_ON_LIST, INPUTS_ON_LIST, INPUTS_ON_LIST, INPUTS_ON_LIST,
                               INPUTS_ON_LIST };

    SET_RETURN_SEQ(GPIOPinRead, return_seq, NUM_INPUT_MODES*5);

    auto_input_init();

    for (uint8_t i = 0; i < 5; i++) {
        input_update();
    }

    for (uint32_t i = 0; i < NUM_INPUT_MODES; i++) {
        TEST_ASSERT_TRUE(input_get(i));
    }
}

void test_input_hal_debounce(void) {
    uint32_t return_seq[NUM_INPUT_MODES*3] = { INPUTS_ON_LIST, INPUTS_ON_LIST, INPUTS_OFF_LIST };

    SET_RETURN_SEQ(GPIOPinRead, return_seq, NUM_INPUT_MODES*3);

    auto_input_init();

    for (uint8_t i = 0; i < 3; i++) {
        input_update();
    }

    for (uint32_t i = 0; i < NUM_INPUT_MODES; i++) {
        TEST_ASSERT_FALSE(input_get(i));
    }
}

void test_input_hal_pushed_indicated(void) {
    uint32_t return_seq[NUM_INPUT_MODES*3] = { INPUTS_ON_LIST, INPUTS_ON_LIST, INPUTS_ON_LIST };

    SET_RETURN_SEQ(GPIOPinRead, return_seq, NUM_INPUT_MODES*3);

    auto_input_init();

    for (uint8_t i = 0; i < 3; i++) {
        input_update();
    }

    for (uint32_t i = 0; i < NUM_INPUT_MODES; i++) {
        TEST_ASSERT_EQUAL(PUSHED, input_check(i));
    }
}

void test_input_hal_released_indicated(void) {
    uint32_t return_seq[NUM_INPUT_MODES*6] = { INPUTS_ON_LIST, INPUTS_ON_LIST, INPUTS_ON_LIST,
                               INPUTS_OFF_LIST, INPUTS_OFF_LIST, INPUTS_OFF_LIST };

    SET_RETURN_SEQ(GPIOPinRead, return_seq, NUM_INPUT_MODES*6);

    auto_input_init();

    for (uint8_t i = 0; i < 6; i++) {
        input_update();
    }

    for (uint32_t i = 0; i < NUM_INPUT_MODES; i++) {
        TEST_ASSERT_EQUAL(RELEASED, input_check(i));
    }
}