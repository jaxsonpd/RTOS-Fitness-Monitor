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

#include "IO_input.h"

#define INPUTS_ON_LIST  0x00000000, 0x00000004, 0x00000000, 0x00000000, 0x00000080
#define INPUTS_OFF_LIST 0x00000001, 0x00000000, 0x00000010, 0x00000001, 0x00000000

void reset_fff(void)
{
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
void test_IO_input_pin_type_set_calls(void) {
    // arrange

    // act
    input_init();

    // assert
    TEST_ASSERT_EQUAL(NUM_INPUT_MODES, GPIOPinTypeGPIOInput_fake.call_count);
}

void test_IO_input_pad_config_set_calls(void) {
    input_init();

    TEST_ASSERT_EQUAL(NUM_INPUT_MODES, GPIOPadConfigSet_fake.call_count);
}

void test_IO_input_peripheral_enable_calls(void) {
    input_init();

    TEST_ASSERT_EQUAL(NUM_INPUT_MODES, SysCtlPeripheralEnable_fake.call_count);
}

void test_IO_input_init_successful(void) {
    TEST_ASSERT_TRUE(input_init());
}

void test_IO_input_get_false_on_init(void) {
    input_init();

    TEST_ASSERT_FALSE(input_get(UP_BUTTON));
    TEST_ASSERT_FALSE(input_get(DOWN_BUTTON));
    TEST_ASSERT_FALSE(input_get(LEFT_BUTTON));
    TEST_ASSERT_FALSE(input_get(RIGHT_BUTTON));
    TEST_ASSERT_FALSE(input_get(RIGHT_SWITCH));
}

void test_IO_input_check_no_change_on_init(void) {
    input_init();

    TEST_ASSERT_EQUAL(NO_CHANGE, input_check(UP_BUTTON));
    TEST_ASSERT_EQUAL(NO_CHANGE, input_check(DOWN_BUTTON));
    TEST_ASSERT_EQUAL(NO_CHANGE, input_check(LEFT_BUTTON));
    TEST_ASSERT_EQUAL(NO_CHANGE, input_check(RIGHT_BUTTON));
    TEST_ASSERT_EQUAL(NO_CHANGE, input_check(RIGHT_SWITCH));
}

void test_IO_input_correct_eventually(void) {
    uint32_t return_seq[25] = {INPUTS_ON_LIST, INPUTS_ON_LIST, INPUTS_ON_LIST, INPUTS_ON_LIST,
        INPUTS_ON_LIST}; 
    
    SET_RETURN_SEQ(GPIOPinRead, return_seq, 25);

    input_init();

    for (uint8_t i = 0; i < 5; i ++) {    
        input_update();
    }

    TEST_ASSERT_TRUE(input_get(UP_BUTTON));
    TEST_ASSERT_TRUE(input_get(DOWN_BUTTON));
    TEST_ASSERT_TRUE(input_get(LEFT_BUTTON));
    TEST_ASSERT_TRUE(input_get(RIGHT_BUTTON));
    TEST_ASSERT_TRUE(input_get(RIGHT_SWITCH));
}

void test_IO_input_debounce(void) {
    uint32_t return_seq[10] = {INPUTS_ON_LIST, INPUTS_OFF_LIST}; 

    SET_RETURN_SEQ(GPIOPinRead, return_seq, 10);

    input_init();

    input_update();
    input_update();

    TEST_ASSERT_FALSE(input_get(UP_BUTTON));
    TEST_ASSERT_FALSE(input_get(DOWN_BUTTON));
    TEST_ASSERT_FALSE(input_get(LEFT_BUTTON));
    TEST_ASSERT_FALSE(input_get(RIGHT_BUTTON));
    TEST_ASSERT_FALSE(input_get(RIGHT_SWITCH));
}
