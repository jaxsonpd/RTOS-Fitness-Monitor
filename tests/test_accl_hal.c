#include <stdint.h>
#include <stdbool.h>

#include "unity.h"
#include "accl_hal.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "tiva_mocks/i2c_mock.h"
#include "hal_mocks/i2c_hal_mock.h"

/* Helper functions */
void reset_fff(void)
{
    FFF_I2C_FAKES_LIST(RESET_FAKE);
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

void test_accl_hal_initialises_i2c_runs_i2c_hal_int(void)
{
    // Arrange
    

    // Act
    accl_chip_init();

    // Assert
    TEST_ASSERT_EQUAL(1, i2c_hal_init_fake.call_count);
}
