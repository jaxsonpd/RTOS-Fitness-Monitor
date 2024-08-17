#include "fff.h"
#include "adc_manager.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef FFF_MOCK_IMPL
#define VOID_FUNC FAKE_VOID_FUNC
#define VALUE_FUNC FAKE_VALUE_FUNC
#else
#define VOID_FUNC DECLARE_FAKE_VOID_FUNC
#define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_ADC_MANAGER_FAKES_LIST(FUNC) \
    FUNC(callbackADC)                    \
    FUNC(pollADC)                        \
    FUNC(initADC)                        \
    FUNC(readADC)                        \

VOID_FUNC(callbackADC);
VOID_FUNC(pollADC);
VOID_FUNC(initADC);
VALUE_FUNC(uint32_t, readADC);
