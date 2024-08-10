#include "fff.h"

#include "adc_hal.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_ADCHAL_FAKES_LIST(FUNC) \
    FUNC(adc_hal_register)          \
    FUNC(adc_hal_isr)               \
    FUNC(adc_hal_conversion)        \

VOID_FUNC(adc_hal_register, uint8_t, callback_t);
VOID_FUNC(adc_hal_isr);
VOID_FUNC(adc_hal_conversion, uint8_t);