#include <stdint.h>
#include "accl_manager.h"

#include "unity.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "circular_buffer_mock.h"
#include "accl_hal_mock.h"
#include "vector3_mock.h"

#define BUF_SIZE 20
#define FAKE_ACCL_X 10
#define FAKE_ACCL_Y 20
#define FAKE_ACCL_Z 30

/* Helper functions */ 
void reset_fff(void)
{
    FFF_ACCLHAL_FAKES_LIST(RESET_FAKE);
    FFF_VECTOR3_FAKES_LIST(RESET_FAKE);
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

vector3_t v3_constructor_fake_construction(int16_t arg0, int16_t arg1, int16_t arg2) 
{
    vector3_t return_val = {arg0,arg1,arg2};
    return return_val;
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
    accl_init();

    // Assert
    TEST_ASSERT_EQUAL(3,CircBuf_init_fake.call_count);
}

void test_accl_init_configures_accl(void)
{
    // Arrange

    // Act
    accl_init();

    // Assert
    TEST_ASSERT_EQUAL(1,accl_chip_init_fake.call_count);
}

void test_accl_process_gets_acceleration(void)
{
    // Arrange
    accl_init();

    accl_data_get_fake.custom_fake = accl_data_get_fake_acceleration_value;

    // Act
    accl_poll();
    
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
    vector3_t result = accl_mean();

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
    v3_constructor_fake.custom_fake = v3_constructor_fake_construction;

    // Act
    vector3_t result = accl_mean();

    // Assert
    TEST_ASSERT_EQUAL(test_value, result.x);
    TEST_ASSERT_EQUAL(test_value, result.y);
    TEST_ASSERT_EQUAL(test_value, result.z);
}

void test_accl_mean_nonhomogenous(void) {
    // Arrange
    int32_t return_values[BUF_SIZE * 3];
    for (size_t i = 0; i < BUF_SIZE*3; i++) {
        return_values[i] = i+1;
    }

    SET_RETURN_SEQ(CircBuf_read, return_values, 3*BUF_SIZE);
    v3_constructor_fake.custom_fake = v3_constructor_fake_construction;

    // Act
    vector3_t result = accl_mean();

    // Assert
    TEST_ASSERT_EQUAL(29, result.x);
    TEST_ASSERT_EQUAL(30, result.y);
    TEST_ASSERT_EQUAL(31, result.z);
}