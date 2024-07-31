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
    TEST_ASSERT_EQUAL(5, GPIOPinTypeGPIOInput_fake.call_count);
}

void test_IO_input_pad_config_set_calls(void) {
    input_init();

    TEST_ASSERT_EQUAL(5, GPIOPadConfigSet_fake.call_count);
}

void test_IO_input_peripheral_enable_calls(void) {
    input_init();

    TEST_ASSERT_EQUAL(5, SysCtlPeripheralEnable_fake.call_count);
}

void test_IO_input_init_successful(void) {
    TEST_ASSERT_TRUE(input_init());
}

