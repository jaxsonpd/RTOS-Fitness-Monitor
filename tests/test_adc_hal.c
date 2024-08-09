#include <stdint.h>
#include <stdbool.h>

#include "unity.h"
#include "adc_hal.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "tiva_mocks/adc_mock.h"
#include "tiva_mocks/sysctl_mock.h"

#define FAKE_ADC_VALUE 0xFACE

/* Helper functions */
static uint32_t callback_sum;

void reset_fff(void)
{
    FFF_ADC_FAKES_LIST(RESET_FAKE);
    FFF_SYSCTL_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

void dummy_callback(uint32_t ulValue)
{
    callback_sum += ulValue;
}

void periph_ready(void) {
    bool return_seq[1] = {true};
    SET_RETURN_SEQ(SysCtlPeripheralReady, return_seq, 1);
}

/* Custom fakes */
int32_t ADCSequenceDataGet_fake_adc_value(uint32_t arg0, uint32_t arg1, uint32_t *arg2)
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

void test_adc_hal_registers_an_adc(void)
{
    // Arrange
    periph_ready();

    // Act
    adc_hal_register(ADC_ID_0, dummy_callback);

    // Assert
    TEST_ASSERT_EQUAL(1, SysCtlPeripheralEnable_fake.call_count);
    TEST_ASSERT_EQUAL(1, ADCSequenceConfigure_fake.call_count);
    TEST_ASSERT_EQUAL(1, ADCSequenceStepConfigure_fake.call_count);
    TEST_ASSERT_EQUAL(1, ADCSequenceEnable_fake.call_count);
    TEST_ASSERT_EQUAL(1, ADCIntRegister_fake.call_count);
    TEST_ASSERT_EQUAL(1, ADCIntEnable_fake.call_count);
}

void test_adc_hal_registers_correct_adc(void)
{
    // Arrange
    periph_ready();

    // Act
    adc_hal_register(ADC_ID_0, dummy_callback);

    // Assert
    TEST_ASSERT_EQUAL(1, SysCtlPeripheralEnable_fake.call_count);
    TEST_ASSERT_EQUAL(SYSCTL_PERIPH_ADC0, SysCtlPeripheralEnable_fake.arg0_val);

    TEST_ASSERT_EQUAL(1, ADCSequenceConfigure_fake.call_count);
    TEST_ASSERT_EQUAL(ADC0_BASE, ADCSequenceConfigure_fake.arg0_val);
    TEST_ASSERT_EQUAL(3, ADCSequenceConfigure_fake.arg1_val);
    TEST_ASSERT_EQUAL(0, ADCSequenceConfigure_fake.arg3_val);

    TEST_ASSERT_EQUAL(1, ADCSequenceStepConfigure_fake.call_count);
    TEST_ASSERT_EQUAL(ADC0_BASE, ADCSequenceStepConfigure_fake.arg0_val);
    TEST_ASSERT_EQUAL(3, ADCSequenceStepConfigure_fake.arg1_val);
    TEST_ASSERT_EQUAL(0, ADCSequenceStepConfigure_fake.arg2_val);
    TEST_ASSERT_EQUAL(ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END, ADCSequenceStepConfigure_fake.arg3_val);
    
    TEST_ASSERT_EQUAL(1, ADCSequenceEnable_fake.call_count);
    TEST_ASSERT_EQUAL(ADC0_BASE, ADCSequenceEnable_fake.arg0_val);
    TEST_ASSERT_EQUAL(3, ADCSequenceEnable_fake.arg1_val);
}

void test_adc_hal_register_doesnt_exceed_max_peripherals(void)
{
    // Arrange
    periph_ready();
    uint8_t invalid_id = 99;

    // Act
    adc_hal_register(invalid_id, dummy_callback);

    // Assert
    TEST_ASSERT_EQUAL(0, SysCtlPeripheralEnable_fake.call_count);
    TEST_ASSERT_EQUAL(0, ADCSequenceConfigure_fake.call_count);
    TEST_ASSERT_EQUAL(0, ADCSequenceStepConfigure_fake.call_count);
    TEST_ASSERT_EQUAL(0, ADCSequenceEnable_fake.call_count);
    TEST_ASSERT_EQUAL(0, ADCIntRegister_fake.call_count);
    TEST_ASSERT_EQUAL(0, ADCIntEnable_fake.call_count);
}

void test_adc_hal_register_doesnt_accept_null_callback(void)
{
    // Arrange
    periph_ready();
    uint8_t valid_id = ADC_ID_0;
    callback_t invalid_callback = NULL;

    // Act
    adc_hal_register(valid_id, invalid_callback);

    // Assert
    TEST_ASSERT_EQUAL(0, SysCtlPeripheralEnable_fake.call_count);
    TEST_ASSERT_EQUAL(0, ADCSequenceConfigure_fake.call_count);
    TEST_ASSERT_EQUAL(0, ADCSequenceStepConfigure_fake.call_count);
    TEST_ASSERT_EQUAL(0, ADCSequenceEnable_fake.call_count);
    TEST_ASSERT_EQUAL(0, ADCIntRegister_fake.call_count);
    TEST_ASSERT_EQUAL(0, ADCIntEnable_fake.call_count);
}

void test_adc_hal_conversion_triggers_adc(void)
{
    // Arrange
    periph_ready();
    adc_hal_register(ADC_ID_0, dummy_callback);

    // Act
    adc_hal_conversion(ADC_ID_0);

    // Assert
    TEST_ASSERT_EQUAL(1, ADCProcessorTrigger_fake.call_count);
}

void test_adc_hal_conversion_triggers_correct_adc(void)
{
    // Arrange
    periph_ready();
    adc_hal_register(ADC_ID_0, dummy_callback);

    // Act
    adc_hal_conversion(ADC_ID_0);

    // Assert
    TEST_ASSERT_EQUAL(1, ADCProcessorTrigger_fake.call_count);
    TEST_ASSERT_EQUAL(ADC0_BASE, ADCProcessorTrigger_fake.arg0_val);
    TEST_ASSERT_EQUAL(3, ADCProcessorTrigger_fake.arg1_val);
}

void test_adc_hal_conversion_doesnt_accept_unregistered_adc(void)
{
    // Act
    adc_hal_conversion(ADC_ID_0);

    // Assert
    TEST_ASSERT_EQUAL(0, ADCProcessorTrigger_fake.call_count);
}

void test_adc_hal_isr_reads_correct_adc_channel(void)
{
    // Arrange
    periph_ready();
    adc_hal_register(ADC_ID_0, dummy_callback);
    ADCIntStatus_fake.return_val = 1;

    // Act
    adc_hal_isr();

    // Assert
    TEST_ASSERT_EQUAL(1, ADCSequenceDataGet_fake.call_count);
    TEST_ASSERT_EQUAL(ADC0_BASE, ADCSequenceDataGet_fake.arg0_val);
    TEST_ASSERT_EQUAL(3, ADCSequenceDataGet_fake.arg1_val);
}

void test_adc_hal_isr_clears_interrupt_flag(void)
{
    // Arrange
    periph_ready();
    adc_hal_register(ADC_ID_0, dummy_callback);
    ADCIntStatus_fake.return_val = 1;

    // Act
    adc_hal_isr();

    // Assert
    TEST_ASSERT_EQUAL(1, ADCIntClear_fake.call_count);
    TEST_ASSERT_EQUAL(ADC0_BASE, ADCIntClear_fake.arg0_val);
    TEST_ASSERT_EQUAL(3, ADCIntClear_fake.arg1_val);
}

void test_adc_hal_isr_calls_correct_callback(void)
{
    // Arrange
    callback_sum = 0;
    periph_ready();
    adc_hal_register(ADC_ID_0, dummy_callback);
    ADCIntStatus_fake.return_val = 1;
    ADCSequenceDataGet_fake.custom_fake = ADCSequenceDataGet_fake_adc_value;

    // Act
    adc_hal_isr();

    // Assert
    TEST_ASSERT_NOT_EQUAL(0, callback_sum);
}

void test_adc_hal_isr_calls_correct_callback_with_correct_value(void)
{
    // Arrange
    callback_sum = 0;
    periph_ready();
    adc_hal_register(ADC_ID_0, dummy_callback);
    ADCIntStatus_fake.return_val = 1;
    ADCSequenceDataGet_fake.custom_fake = ADCSequenceDataGet_fake_adc_value;

    // Act
    adc_hal_isr();

    // Assert
    TEST_ASSERT_EQUAL(FAKE_ADC_VALUE, callback_sum);
}