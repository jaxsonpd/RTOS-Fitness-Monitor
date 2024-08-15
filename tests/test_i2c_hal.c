#include <stdint.h>
#include <stdbool.h>

#include "unity.h"

#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"

#include "i2c_hal.h"

#include "fff.h"

DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "tiva_mocks/sysctl_mock.h"
#include "tiva_mocks/gpio_mock.h"
#include "tiva_mocks/i2c_mock.h"

void reset_fff(void)
{
    FFF_SYSCTL_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

/* Unity setup and teardown */
void setUp(void)
{
    reset_fff();
}

void tearDown(void)
{
    
}

void test_i2c_hal_init_initialises_a_peripheral(void)
{
    // Act
    i2c_hal_init();

    // Assert
    TEST_ASSERT_EQUAL(3, SysCtlPeripheralEnable_fake.call_count);
    TEST_ASSERT_EQUAL(1, SysCtlPeripheralReset_fake.call_count);
    TEST_ASSERT_EQUAL(1, GPIOPinTypeI2C_fake.call_count);
    TEST_ASSERT_EQUAL(1, GPIOPinTypeI2CSCL_fake.call_count);
    TEST_ASSERT_EQUAL(2, GPIOPinConfigure_fake.call_count);
    TEST_ASSERT_EQUAL(1, I2CMasterInitExpClk_fake.call_count);
}

void test_i2c_hal_init_enables_correct_peripheral(void)
{
    // Act
    i2c_hal_init();

    // Assert
    TEST_ASSERT_EQUAL(SYSCTL_PERIPH_GPIOB, SysCtlPeripheralEnable_fake.arg0_history[0]);
    TEST_ASSERT_EQUAL(SYSCTL_PERIPH_GPIOE, SysCtlPeripheralEnable_fake.arg0_history[1]);
    TEST_ASSERT_EQUAL(SYSCTL_PERIPH_I2C0, SysCtlPeripheralEnable_fake.arg0_history[2]);

    TEST_ASSERT_EQUAL(SYSCTL_PERIPH_I2C0, SysCtlPeripheralReset_fake.arg0_val);

    TEST_ASSERT_EQUAL(GPIO_PORTB_BASE, GPIOPinTypeI2C_fake.arg0_val);
    TEST_ASSERT_EQUAL(GPIO_PIN_3, GPIOPinTypeI2C_fake.arg1_val);

    TEST_ASSERT_EQUAL(GPIO_PORTB_BASE, GPIOPinTypeI2CSCL_fake.arg0_val);
    TEST_ASSERT_EQUAL(GPIO_PIN_2, GPIOPinTypeI2CSCL_fake.arg1_val);

    TEST_ASSERT_EQUAL(GPIO_PB2_I2C0SCL, GPIOPinConfigure_fake.arg0_history[0]);
    TEST_ASSERT_EQUAL(GPIO_PB3_I2C0SDA, GPIOPinConfigure_fake.arg0_history[1]);

    TEST_ASSERT_EQUAL(I2C0_BASE, I2CMasterInitExpClk_fake.arg0_val);
    TEST_ASSERT_EQUAL(SysCtlClockGet(), I2CMasterInitExpClk_fake.arg1_val);
    TEST_ASSERT_EQUAL(true, I2CMasterInitExpClk_fake.arg2_val);
}

void test_i2c_read_calls_correct_peripheral(void)
{
    TEST_IGNORE();
    // Arrange
    i2c_hal_init();

    // Act

}