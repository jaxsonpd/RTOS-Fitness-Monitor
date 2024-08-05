#include <stdint.h>
#include "accl_manager.h"

#include "unity.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "circular_buffer_T_mock.h"
#include "accl_hal_mock.h"

#define BUF_SIZE 5
#define FAKE_ACCL_X 10
#define FAKE_ACCL_Y 20
#define FAKE_ACCL_Z 30

/* Helper functions */ 
void reset_fff(void)
{
    FFF_ACCLHAL_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

// Checks that func is called once with args arg0, arg1
#define assert_func_called_with_args(func, arg0, arg1) \
    TEST_ASSERT_EQUAL(1, func.call_count);   \
    TEST_ASSERT_EQUAL(arg0, func.arg0_val);  \
    TEST_ASSERT_EQUAL(arg1, func.arg1_val);

/* Custom fakes */
void accl_data_get_fake_acceleration_value(int16_t* arg0)
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
void test_accl_init_initialises_buffers(void)
{
    // Arrange

    // Act
    acclInit();

    // Assert
    TEST_ASSERT_EQUAL(3,CircBuf_init_fake.call_count);
}

void test_accl_init_configures_accl(void)
{
    // Arrange

    // Act
    acclInit();

    // Assert
    TEST_ASSERT_EQUAL(1,accl_chip_init_fake.call_count);
}

void test_accl_process_gets_acceleration(void)
{
    // Arrange
    acclInit();

    accl_data_get_fake.custom_fake = accl_data_get_fake_acceleration_value;

    // Act
    acclProcess();
    
    // Assert
    TEST_ASSERT_EQUAL(3, CircBuf_write_fake.call_count);
    TEST_ASSERT_EQUAL(FAKE_ACCL_X, CircBuf_write_fake.arg1_history[0]);
    TEST_ASSERT_EQUAL(FAKE_ACCL_Y, CircBuf_write_fake.arg1_history[1]);
    TEST_ASSERT_EQUAL(FAKE_ACCL_Z, CircBuf_write_fake.arg1_history[2]);
}

void test_accl_mean_zero(void) 
{
     // Arrange
    int16_t test_value = 0;

    // Set up the fake to return the test value for each call
    CircBuf_read_fake.return_val = test_value;

    // Act
    vector3_t result = acclMean();

    // Assert
    TEST_ASSERT_EQUAL(test_value, result.x);
    TEST_ASSERT_EQUAL(test_value, result.y);
    TEST_ASSERT_EQUAL(test_value, result.z);
}

void test_accl_mean_homogenous(void) 
{
    // Arrange
    int16_t test_value = 10;

    // Set up the fake to return the test value for each call
    CircBuf_read_fake.return_val = test_value;

    // Act
    vector3_t result = acclMean();

    // Assert
    TEST_ASSERT_EQUAL(test_value, result.x);
    TEST_ASSERT_EQUAL(test_value, result.y);
    TEST_ASSERT_EQUAL(test_value, result.z);
}

void test_accl_mean_nonhomogenous(void) {
    // Arrange
    int32_t return_values[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    SET_RETURN_SEQ(CircBuf_read, return_values, 3*BUF_SIZE);

    // Act
    vector3_t result = acclMean();

    // Assert
    TEST_ASSERT_EQUAL(7, result.x); // (1 + 4 + 7 + 10 + 13) / 5 = 7
    TEST_ASSERT_EQUAL(8, result.y); // (2 + 5 + 8 + 11 + 14) / 5 = 8
    TEST_ASSERT_EQUAL(9, result.z); // (3 + 6 + 9 + 12 + 15) / 5 = 9
}