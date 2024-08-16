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

bool i2c_master_bus_busy_fake_return(uint32_t arg0)
{
    return 1;
}

bool i2c_master_busy_fake_return(uint32_t arg0)
{
    return 0;
}

void test_i2c_hal_init_initialises_a_peripheral(void)
{
    // Act
    i2c_hal_init(0);

    // Assert
    TEST_ASSERT_EQUAL(2, SysCtlPeripheralEnable_fake.call_count);
    TEST_ASSERT_EQUAL(1, SysCtlPeripheralReset_fake.call_count);
    TEST_ASSERT_EQUAL(1, GPIOPinTypeI2C_fake.call_count);
    TEST_ASSERT_EQUAL(1, GPIOPinTypeI2CSCL_fake.call_count);
    TEST_ASSERT_EQUAL(2, GPIOPinConfigure_fake.call_count);
    TEST_ASSERT_EQUAL(1, I2CMasterInitExpClk_fake.call_count);
}

void test_i2c_hal_init_enables_correct_peripheral(void)
{
    // Act
    i2c_hal_init(0);

    // Assert
    TEST_ASSERT_EQUAL(SYSCTL_PERIPH_GPIOB, SysCtlPeripheralEnable_fake.arg0_history[0]);
    TEST_ASSERT_EQUAL(SYSCTL_PERIPH_I2C0, SysCtlPeripheralEnable_fake.arg0_history[1]);

    TEST_ASSERT_EQUAL(SYSCTL_PERIPH_I2C0, SysCtlPeripheralReset_fake.arg0_val);

    TEST_ASSERT_EQUAL(GPIO_PORTB_BASE, GPIOPinTypeI2C_fake.arg0_val);
    TEST_ASSERT_EQUAL(GPIO_PIN_3, GPIOPinTypeI2C_fake.arg1_val);

    TEST_ASSERT_EQUAL(GPIO_PORTB_BASE, GPIOPinTypeI2CSCL_fake.arg0_val);
    TEST_ASSERT_EQUAL(GPIO_PIN_2, GPIOPinTypeI2CSCL_fake.arg1_val);

    TEST_ASSERT_EQUAL(GPIO_PB2_I2C0SCL, GPIOPinConfigure_fake.arg0_history[1]);
    TEST_ASSERT_EQUAL(GPIO_PB3_I2C0SDA, GPIOPinConfigure_fake.arg0_history[0]);

    TEST_ASSERT_EQUAL(I2C0_BASE, I2CMasterInitExpClk_fake.arg0_val);
    TEST_ASSERT_EQUAL(SysCtlClockGet(), I2CMasterInitExpClk_fake.arg1_val);
    TEST_ASSERT_EQUAL(true, I2CMasterInitExpClk_fake.arg2_val);
}

void test_i2c_hal_write_sends_data_correctly(void)
{
    // Arrange
    char data[] = {0x01, 0x02, 0x03};
    int32_t size = 3;
    char addr = 0x50;
    I2CMasterBusBusy_fake.custom_fake = i2c_master_bus_busy_fake_return;
    I2CMasterBusy_fake.custom_fake = i2c_master_busy_fake_return;
    // Act
    i2c_hal_write(0, data, size, addr);

    // Assert
    TEST_ASSERT_EQUAL(addr, I2CMasterSlaveAddrSet_fake.arg1_val);
    TEST_ASSERT_EQUAL(0, I2CMasterSlaveAddrSet_fake.arg2_val);
    TEST_ASSERT_EQUAL(data[0], I2CMasterDataPut_fake.arg1_history[0]);
    TEST_ASSERT_EQUAL(size, I2CMasterControl_fake.call_count - 1);
}

void test_i2c_hal_read_receives_data_correctly(void)
{
    // Arrange
    char data[3];
    int32_t size = 3;
    char addr = 0x50;
    I2CMasterBusBusy_fake.custom_fake = i2c_master_bus_busy_fake_return;
    I2CMasterBusy_fake.custom_fake = i2c_master_busy_fake_return;

    // Act
    i2c_hal_read(0, data, size, addr);

    // Assert
    TEST_ASSERT_EQUAL(addr, I2CMasterSlaveAddrSet_fake.arg1_val);
    TEST_ASSERT_EQUAL(1, I2CMasterSlaveAddrSet_fake.arg2_val);
    TEST_ASSERT_EQUAL(size, I2CMasterControl_fake.call_count - 1);
    TEST_ASSERT_EQUAL(size, I2CMasterDataGet_fake.call_count);
}

void test_i2c_hal_write_handles_large_data(void)
{
    // Arrange
    char data[100];
    int32_t size = 100;
    char addr = 0x50;
    I2CMasterBusBusy_fake.custom_fake = i2c_master_bus_busy_fake_return;
    I2CMasterBusy_fake.custom_fake = i2c_master_busy_fake_return;

    // Act
    i2c_hal_write(0, data, size, addr);

    // Assert
    TEST_ASSERT_EQUAL(size, I2CMasterControl_fake.call_count - 1);
    TEST_ASSERT_EQUAL(addr, I2CMasterSlaveAddrSet_fake.arg1_val);
}

void test_i2c_hal_read_handles_large_data(void)
{
    // Arrange
    char data[100];
    int32_t size = 100;
    char addr = 0x50;
    I2CMasterBusBusy_fake.custom_fake = i2c_master_bus_busy_fake_return;
    I2CMasterBusy_fake.custom_fake = i2c_master_busy_fake_return;

    // Act
    i2c_hal_read(0, data, size, addr);

    // Assert
    TEST_ASSERT_EQUAL(size, I2CMasterControl_fake.call_count - 1);
    TEST_ASSERT_EQUAL(size, I2CMasterDataGet_fake.call_count);
}

void test_i2c_hal_init_catches_invalid_id(void)
{
    // Arrange
    uint8_t invalid_id = 5;
    I2CMasterBusBusy_fake.custom_fake = i2c_master_bus_busy_fake_return;
    I2CMasterBusy_fake.custom_fake = i2c_master_busy_fake_return;

    // Act
    i2c_hal_init(invalid_id);

    // Assert
    TEST_ASSERT_EQUAL(0, SysCtlPeripheralEnable_fake.call_count);
    TEST_ASSERT_EQUAL(0, GPIOPinTypeI2C_fake.call_count);
}