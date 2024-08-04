#include <stdint.h>
#include "circBufV.h"
#include "accl_manager.h"

#include "unity.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "tiva_mocks/sysctl_mock.h"
#include "i2c_driver_mock.h"

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
    TEST_ASSERT_EQUAL(arg1, func.arg1_val);  \


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
    // acclInit();

    // Assert
    // TEST_ASSERT_EQUAL(1,SysCtlPeripheralEnable_fake.call_count);
}