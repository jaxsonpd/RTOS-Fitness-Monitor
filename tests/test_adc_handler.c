#include "unity.h"
#include "adc_handler.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "utility_mocks/circular_buffer_mock.h"
#include "hal_mocks/adc_hal_mock.h"

#define ADC_BUF_SIZE 10
#define FAKE_ADC_VALUE 0xFACE // Reduced size as buffer write value is int32_t, so uint32_t overflows.

/* Helper functions */
void reset_fff(void)
{
    FFF_CIRCULAR_BUFFER_FAKES_LIST(RESET_FAKE);
    FFF_ADCHAL_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

circBuf_t *get_circBuf_ptr_and_reset_fff(void)
{
    adc_init();
    circBuf_t *buffer_ptr = circ_buf_init_fake.arg0_val;
    reset_fff();
    return buffer_ptr;
}

/* Unity setup and teardown */
void setUp(void)
{
    reset_fff();
}

void tearDown(void)
{
}

/* Test cases - adc_init */
void test_adc_init_initialises_buffer(void)
{
    // Act
    adc_init();

    // Assert
    TEST_ASSERT_EQUAL(1, circ_buf_init_fake.call_count);
    TEST_ASSERT_EQUAL(ADC_BUF_SIZE, circ_buf_init_fake.arg1_val);
}

void test_adc_init_enables_adc(void)
{
    // Act
    adc_init();

    // Assert
    TEST_ASSERT_EQUAL(1, adc_hal_register_fake.call_count);
}

/* Test cases - adc_poll */
void test_adc_poll_triggers_adc(void)
{
    // Arrange
    adc_init();
    
    // Act
    adc_poll();

    // Assert
    TEST_ASSERT_EQUAL(1, adc_hal_conversion_fake.call_count);
    TEST_ASSERT_EQUAL(ADC_ID_0, adc_hal_conversion_fake.arg0_val);}

/* Test cases - adc_read */
void test_adc_read_reads_correct_buffer(void)
{
    // Arrange
    circBuf_t *correct_buffer = get_circBuf_ptr_and_reset_fff();

    // Act
    adc_read();

    // Assert
    TEST_ASSERT_EQUAL(correct_buffer, circ_buf_read_fake.arg0_val);
}

void test_adc_reads_buffer_correct_amount(void)
{
    // Act
    adc_read();

    // Assert
    TEST_ASSERT_EQUAL(ADC_BUF_SIZE + 1, circ_buf_read_fake.call_count);
}

void test_adc_averages_correctly_homogenous(void)
{
    // Arrange
    uint32_t fake_avg;
    int32_t fake_buffer_values[ADC_BUF_SIZE] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2};

    SET_RETURN_SEQ(circ_buf_read, fake_buffer_values, ADC_BUF_SIZE);

    // Act
    fake_avg = adc_read();

    // Assert
    TEST_ASSERT_EQUAL(2, fake_avg);
}

void test_adc_averages_correctly_nonhomogenous(void)
{
    // Arrange
    uint32_t fake_avg;
    int32_t fake_buffer_values[ADC_BUF_SIZE] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

    SET_RETURN_SEQ(circ_buf_read, fake_buffer_values, ADC_BUF_SIZE);

    // Act
    fake_avg = adc_read();

    TEST_ASSERT_EQUAL(55, fake_avg);
}

void test_adc_averages_correctly_zeros(void)
{
    // Arrange
    uint32_t fake_avg;
    int32_t fake_buffer_values[ADC_BUF_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    SET_RETURN_SEQ(circ_buf_read, fake_buffer_values, ADC_BUF_SIZE);

    // Act
    fake_avg = adc_read();

    TEST_ASSERT_EQUAL(0, fake_avg);
}