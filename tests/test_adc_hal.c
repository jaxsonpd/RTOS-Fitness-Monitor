#include <stdint.h>
#include <stdbool.h>

#include "unity.h"
#include "adc_hal.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "tiva_mocks/adc_mock.h"
#include "tiva_mocks/sysctl_mock.h"

/* Helper functions */
void reset_fff(void)
{
    FFF_ADC_FAKES_LIST(RESET_FAKE);
    FFF_SYSCTL_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

void dummy_callback(uint32_t)
{

}

void periph_ready(void) {
    bool return_seq[1] = {true};
    SET_RETURN_SEQ(SysCtlPeripheralReady, return_seq, 1);
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

void test_adc_doesnt_exceed_max_peripherals(void)
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
