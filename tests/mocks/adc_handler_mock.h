#include "fff.h"
#include "adc_handler.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef FFF_MOCK_IMPL
#define VOID_FUNC FAKE_VOID_FUNC
#define VALUE_FUNC FAKE_VALUE_FUNC
#else
#define VOID_FUNC DECLARE_FAKE_VOID_FUNC
#define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_ADC_HANDLER_FAKES_LIST(FUNC) \
    FUNC(adc_poll)                        \
    FUNC(adc_init)                        \
    FUNC(adc_read)                        \


VOID_FUNC(adc_poll);
VOID_FUNC(adc_init);
VALUE_FUNC(uint32_t, adc_read);
