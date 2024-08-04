#include "unity.h"
#include "ADC_read.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "circBufT_mock.h"
#include "tiva_mocks/adc_mock.h"
#include "tiva_mocks/sysctl_mock.h"

#define ADC_BUF_SIZE 10
#define FAKE_ADC_VALUE 0xFACE // Reduced size as buffer write value is int32_t, so uint32_t overflows.

/* Helper functions */
void reset_fff(void)
{
    FFF_ADC_FAKES_LIST(RESET_FAKE);
    FFF_SYSCTL_FAKES_LIST(RESET_FAKE);
    FFF_CIRCBUFT_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

void init_adc_periph_ready(void) {
    bool return_seq[1] = {true};
    SET_RETURN_SEQ(SysCtlPeripheralReady, return_seq, 1);
    initADC();
}

CircBuf_t *get_circBuf_ptr_and_reset_fff(void)
{
    init_adc_periph_ready();
    CircBuf_t *buffer_ptr = CircBuf_init_fake.arg0_val;
    reset_fff();
    return buffer_ptr;
}

void assert_f1_called_before_f2(void *f1, void *f2)
{
    int8_t i_f1 = -1;
    int8_t i_f2 = -1;

    for (uint8_t i = 0; i < FFF_CALL_HISTORY_LEN; i++)
    {
        if (fff.call_history[i] == NULL)
            break;

        if (i_f1 == -1 && fff.call_history[i] == f1)
            i_f1 = i;
        if (i_f2 == -1 && fff.call_history[i] == f2)
            i_f2 = i;
    }

    TEST_ASSERT(i_f1 < i_f2);
}

// Checks that func is called once with args arg0, arg1
#define assert_func_called_with_args(func, arg0, arg1) \
    TEST_ASSERT_EQUAL(1, func.call_count);             \
    TEST_ASSERT_EQUAL(arg0, func.arg0_val);            \
    TEST_ASSERT_EQUAL(arg1, func.arg1_val);


/* Custom fakes */
int32_t ADCSequenceDataGet_fake_adc_value(uint32_t arg0, uint32_t arg1,
                                          uint32_t *arg2)
{
    (void)arg0;
    (void)arg1;
    *arg2 = FAKE_ADC_VALUE;
    return 0;
}

/* Unity setup and teardown */
void setUp(void)
{
    reset_fff();
}

void tearDown(void)
{
}

/* Test cases - initADC */
void test_adc_init_initialises_buffer(void)
{
    // Act
    init_adc_periph_ready();

    // Assert
    TEST_ASSERT_EQUAL(1, CircBuf_init_fake.call_count);
    TEST_ASSERT_EQUAL(ADC_BUF_SIZE, CircBuf_init_fake.arg1_val);
}

void test_adc_init_enables_adc(void)
{
    // Act
    init_adc_periph_ready();

    // Assert
    TEST_ASSERT_EQUAL(1, SysCtlPeripheralEnable_fake.call_count);
    TEST_ASSERT_EQUAL(SYSCTL_PERIPH_ADC0, SysCtlPeripheralEnable_fake.arg0_val);
}

void test_adc_init_initialises_adc_sequence(void)
{
    // Act
    init_adc_periph_ready();

    // Assert
    TEST_ASSERT_EQUAL(1, ADCSequenceConfigure_fake.call_count);
    TEST_ASSERT_EQUAL(ADC0_BASE, ADCSequenceConfigure_fake.arg0_val);
    TEST_ASSERT_EQUAL(3, ADCSequenceConfigure_fake.arg1_val);
    TEST_ASSERT_EQUAL(ADC_TRIGGER_PROCESSOR, ADCSequenceConfigure_fake.arg2_val);
    TEST_ASSERT_EQUAL(0, ADCSequenceConfigure_fake.arg3_val);
}

void test_adc_init_initialises_adc_sequence_step(void)
{
    // Act
    init_adc_periph_ready();

    // Assert
    TEST_ASSERT_EQUAL(1, ADCSequenceStepConfigure_fake.call_count);
    TEST_ASSERT_EQUAL(ADC0_BASE, ADCSequenceStepConfigure_fake.arg0_val);
    TEST_ASSERT_EQUAL(3, ADCSequenceStepConfigure_fake.arg1_val);
    TEST_ASSERT_EQUAL(0, ADCSequenceStepConfigure_fake.arg2_val);
    TEST_ASSERT_EQUAL((ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END), ADCSequenceStepConfigure_fake.arg3_val);
}

void test_adc_init_enables_adc_sequence(void)
{
    // Act
    init_adc_periph_ready();

    // Assert
    TEST_ASSERT_EQUAL(1, ADCSequenceEnable_fake.call_count);
    TEST_ASSERT_EQUAL(ADC0_BASE, ADCSequenceEnable_fake.arg0_val);
    TEST_ASSERT_EQUAL(3, ADCSequenceEnable_fake.arg1_val);
}

void test_adc_init_registers_adc_interrupt(void)
{
    // Act
    init_adc_periph_ready();

    // Assert
    TEST_ASSERT_EQUAL(1, ADCIntRegister_fake.call_count);
    TEST_ASSERT_EQUAL(ADC0_BASE, ADCIntRegister_fake.arg0_val);
    TEST_ASSERT_EQUAL(3, ADCIntRegister_fake.arg1_val);
    TEST_ASSERT_EQUAL(ADCIntHandler, ADCIntRegister_fake.arg2_val);
}

void test_adc_init_enables_adc_before_other_adc_operations(void)
{
    // Act
    init_adc_periph_ready();

    // Assert
    assert_f1_called_before_f2((void *)SysCtlPeripheralEnable, (void *)ADCSequenceConfigure);
    assert_f1_called_before_f2((void *)SysCtlPeripheralEnable, (void *)ADCSequenceStepConfigure);
    assert_f1_called_before_f2((void *)SysCtlPeripheralEnable, (void *)ADCSequenceEnable);
    assert_f1_called_before_f2((void *)SysCtlPeripheralEnable, (void *)ADCIntRegister);
    assert_f1_called_before_f2((void *)SysCtlPeripheralEnable, (void *)ADCIntEnable);
}

void test_adc_init_enables_adc_interrupt(void)
{
    // Act
    init_adc_periph_ready();

    // Assert
    TEST_ASSERT_EQUAL(1, ADCIntEnable_fake.call_count);
    TEST_ASSERT_EQUAL(ADC0_BASE, ADCIntEnable_fake.arg0_val);
    TEST_ASSERT_EQUAL(3, ADCIntEnable_fake.arg1_val);
}

/* Test cases - pollADC */
void test_adc_poll_triggers_adc(void)
{
    // Act
    pollADC();

    // Assert
    assert_func_called_with_args(ADCProcessorTrigger_fake, ADC0_BASE, 3);
}

/* Test cases - ADCIntHandler */
void test_adc_isr_reads_from_correct_adc_channel(void)
{
    // Act
    ADCIntHandler();

    // Assert
    assert_func_called_with_args(ADCSequenceDataGet_fake, ADC0_BASE, 3);
}

void test_adc_isr_writes_to_correct_buffer(void)
{
    // Arrange
    CircBuf_t *correct_buffer = get_circBuf_ptr_and_reset_fff();

    // Act
    ADCIntHandler();

    // Assert
    TEST_ASSERT_EQUAL(1, CircBuf_write_fake.call_count);
    TEST_ASSERT_EQUAL(correct_buffer, CircBuf_write_fake.arg0_val);
}

void test_adc_isr_writes_correct_value(void)
{
    // Act
    ADCSequenceDataGet_fake.custom_fake = ADCSequenceDataGet_fake_adc_value;
    ADCIntHandler();

    // Assert
    TEST_ASSERT_EQUAL(FAKE_ADC_VALUE, *ADCSequenceDataGet_fake.arg2_val);
    TEST_ASSERT_EQUAL(FAKE_ADC_VALUE, CircBuf_write_fake.arg1_val);
}

void test_isr_clears_interrupt_flag(void)
{
    // Act
    ADCIntHandler();

    // Assert
    assert_func_called_with_args(ADCIntClear_fake, ADC0_BASE, 3);
}

/* Test cases - readADC */
void test_adc_read_reads_correct_buffer(void)
{
    // Arrange
    CircBuf_t *correct_buffer = get_circBuf_ptr_and_reset_fff();

    // Act
    readADC();

    // Assert
    TEST_ASSERT_EQUAL(correct_buffer, CircBuf_read_fake.arg0_val);
}

void test_adc_reads_buffer_correct_amount(void)
{
    // Act
    readADC();

    // Assert
    TEST_ASSERT_EQUAL(ADC_BUF_SIZE + 1, CircBuf_read_fake.call_count);
}

void test_adc_averages_correctly_homogenous(void)
{
    // Arrange
    uint32_t fake_avg;
    int32_t fake_buffer_values[ADC_BUF_SIZE] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2};

    SET_RETURN_SEQ(CircBuf_read, fake_buffer_values, ADC_BUF_SIZE);

    // Act
    fake_avg = readADC();

    // Assert
    TEST_ASSERT_EQUAL(2, fake_avg);
}

void test_adc_averages_correctly_nonhomogenous(void)
{
    // Arrange
    uint32_t fake_avg;
    int32_t fake_buffer_values[ADC_BUF_SIZE] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

    SET_RETURN_SEQ(CircBuf_read, fake_buffer_values, ADC_BUF_SIZE);

    // Act
    fake_avg = readADC();

    TEST_ASSERT_EQUAL(55, fake_avg);
}

void test_adc_averages_correctly_zeros(void)
{
    // Arrange
    uint32_t fake_avg;
    int32_t fake_buffer_values[ADC_BUF_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    SET_RETURN_SEQ(CircBuf_read, fake_buffer_values, ADC_BUF_SIZE);

    // Act
    fake_avg = readADC();

    TEST_ASSERT_EQUAL(0, fake_avg);
}