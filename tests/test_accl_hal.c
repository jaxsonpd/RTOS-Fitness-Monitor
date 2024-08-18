#include <stdint.h>
#include <stdbool.h>

#include "unity.h"
#include "accl_hal.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "tiva_mocks/i2c_mock.h"
#include "hal_mocks/i2c_hal_mock.h"

/*
 * Accl Interrupt Pins
 */
#define ACCL_INT1Port       GPIO_PORTB_BASE
#define ACCL_INT2Port       GPIO_PORTE_BASE
#define ACCL_INT1           GPIO_PIN_4
#define ACCL_INT2           GPIO_PIN_4

#define ACCL                2
#define ACCL_ADDR           0x1D

#define ACCL_INT            0x2E
#define ACCL_OFFSET_X       0x1E
#define ACCL_OFFSET_Y       0x1F
#define ACCL_OFFSET_Z       0x20
#define ACCL_DATA_X0        0x32
#define ACCL_DATA_X1        0x33

#define ACCL_PWR_CTL        0x2D
// Parameters for ACCL_PWR_CTL:
#define ACCL_MEASURE        0x08

#define ACCL_DATA_FORMAT    0x31
// Parameters for ACCL_DATA_FORMAT:
#define ACCL_RANGE_2G       0x00
#define ACCL_RANGE_4G       0x01
#define ACCL_RANGE_8G       0x02
#define ACCL_RANGE_16G      0x03
#define ACCL_FULL_RES       0x08
#define ACCL_JUSTIFY        0x04

#define ACCL_BW_RATE        0x2C
// Parameters for ACCL_BW_RATE:
#define ACCL_RATE_3200HZ    0x0F
#define ACCL_RATE_1600HZ    0x0E
#define ACCL_RATE_800HZ     0x0D
#define ACCL_RATE_400HZ     0x0C
#define ACCL_RATE_200HZ     0x0B
#define ACCL_RATE_100HZ     0x0A
#define ACCL_RATE_50HZ      0x09
#define ACCL_RATE_25HZ      0x08
#define ACCL_RATE_12_5HZ    0x07
#define ACCL_RATE_6_25HZ    0x06
#define ACCL_RATE_3_13HZ    0x05
#define ACCL_RATE_1_56HZ    0x04
#define ACCL_RATE_0_78HZ    0x03
#define ACCL_RATE_0_39HZ    0x02
#define ACCL_RATE_0_20HZ    0x01
#define ACCL_RATE_0_10HZ    0x00

char mock_data[] = {ACCL_DATA_X0, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC};
uint8_t mock_data_size = 6;

/* Helper functions */
void reset_fff(void)
{
    FFF_I2C_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

// Helper Functions

char i2c_hal_read_fake_char(uint8_t arg0, char *arg1, int32_t arg2, char arg3)
{
    char simulatedData[7] = {ACCL_DATA_X0, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
    
    // Copy the simulated data into the provided data buffer
    memcpy(arg1, simulatedData, arg2 + 1);
    
    return 0x00; // Return success
}

/* Unity setup and teardown */
void setUp(void)
{
    reset_fff();
}

void tearDown(void)
{
    
}

void test_accl_chip_init_initialises_an_accelerometer(void)
{
    // Act
    accl_chip_init();

    // Assert
    TEST_ASSERT_EQUAL(1, i2c_hal_init_fake.call_count);
    TEST_ASSERT_EQUAL(7, i2c_hal_write_fake.call_count);
}

void test_accl_chip_init(void) {
    accl_chip_init();

    TEST_ASSERT_EQUAL(I2C_ID_0, i2c_hal_init_fake.arg0_val);

    TEST_ASSERT_EQUAL(7, i2c_hal_write_fake.call_count);

}


void test_accl_data_get_reads_acceleration_data(void)
{
    // Arrange
    int16_t acceleration[3] = {0};

    // Act
    accl_data_get(acceleration);

    // Assert
    TEST_ASSERT_EQUAL(1, i2c_hal_read_fake.call_count);
}

void test_accl_chip_init_random(void)
{
    // Arrange
    uint8_t id = I2C_ID_0;
    char data[10];
    char addr = 0x1D;
    
    // Set up mock return values
    i2c_hal_write_fake.return_val = 0;
    i2c_hal_read_fake.return_val = 0;

    // Act
    i2c_hal_init(id);

    // Assert
    TEST_ASSERT_EQUAL(0, i2c_hal_write(id, data, sizeof(data), addr));
    TEST_ASSERT_EQUAL(0, i2c_hal_read(id, data, sizeof(data), addr));
}

void test_accl_data_get_new()
{
    // Arrange
    int16_t acceleration[3] = {0};
    i2c_hal_read_fake.custom_fake = i2c_hal_read_fake_char;
    
    // Act
    accl_data_get(acceleration);

    // Assert
    TEST_ASSERT_EQUAL(0x0201, acceleration[0]); // X-axis
    TEST_ASSERT_EQUAL(0x0403, acceleration[1]); // Y-axis
    TEST_ASSERT_EQUAL(0x0605, acceleration[2]); // Z-axis
}