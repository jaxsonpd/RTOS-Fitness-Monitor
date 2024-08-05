#include <stdint.h>
#include <stdbool.h>
#include "circular_buffer_T.h"
#include "acc.h"
#include "accl_manager.h"

#include "unity.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "tiva_mocks/sysctl_mock.h"
#include "tiva_mocks/gpio_mock.h"
#include "tiva_mocks/i2c_mock.h"
#include "i2c_driver_mock.h"
#include "circular_buffer_T_mock.h"
#include "accl_hal_mock.h"

#define ACCL_BUF_SIZE 5
#define FAKE_ACCL_X 1
#define FAKE_ACCL_Y 2
#define FAKE_ACCL_Z 3

/* Helper functions */ 
void reset_fff(void)
{
    FFF_I2CDRIVER_FAKES_LIST(RESET_FAKE);
    FFF_SYSCTL_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

// Checks that func is called once with args arg0, arg1
#define assert_func_called_with_args(func, arg0, arg1) \
    TEST_ASSERT_EQUAL(1, func.call_count);   \
    TEST_ASSERT_EQUAL(arg0, func.arg0_val);  \
    TEST_ASSERT_EQUAL(arg1, func.arg1_val);

/* Custom fakes */
void getAcclData_fake_acceleration_value(int16_t* arg0)
{
    arg0[0] = FAKE_ACCL_X;
    arg0[1] = FAKE_ACCL_Y;
    arg0[2] = FAKE_ACCL_Z;
}

/* Unity setup and teardown */
void setUp(void)
{
    reset_fff();
}

void tearDown(void)
{
    
}

/* Test cases */
void test_accl_init_initialises_peripherals(void)
{
    // Arrange

    // Act
    acclInit();

    // Assert
    TEST_ASSERT_EQUAL(3,SysCtlPeripheralEnable_fake.call_count);
    TEST_ASSERT_EQUAL(1,SysCtlPeripheralReset_fake.call_count);
}

void test_accl_init_configures_i2c(void)
{
    // Arrange

    // Act
    acclInit();

    // Assert
    assert_func_called_with_args(GPIOPinTypeI2C_fake,I2CSDAPort,I2CSDA_PIN);
}

void test_accl_init_creates_buffer_space(void)
{
    // Arrange

    // Act
    acclInit();
    
    // Assert
    TEST_ASSERT_GREATER_OR_EQUAL(1, CircBuf_init_fake.call_count);
}

void test_accl_process_polls_accelerometer(void)
{
    // Arrange

    // Act
    acclProcess();

    // Assert
    TEST_ASSERT_EQUAL(1, I2CGenTransmit_fake.call_count);
    TEST_ASSERT_EQUAL(ACCL_ADDR, I2CGenTransmit_fake.arg3_val);
}

void test_accl_process_gets_acceleration(void)
{
    // Arrange
    acclInit();

    getAcclData_fake.custom_fake = getAcclData_fake_acceleration_value;

    // Act
    acclProcess();
    
    // Assert
    TEST_ASSERT_EQUAL(3, CircBuf_write_fake.call_count);
    TEST_ASSERT_EQUAL(FAKE_ACCL_X, CircBuf_write_fake.arg1_history[0]);
    TEST_ASSERT_EQUAL(FAKE_ACCL_Y, CircBuf_write_fake.arg1_history[1]);
    TEST_ASSERT_EQUAL(FAKE_ACCL_Z, CircBuf_write_fake.arg1_history[2]);
}