#include "fff.h"
#include <stdbool.h>
#include "driverlib/i2c.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_I2C_FAKES_LIST(FUNC)     \
    FUNC(I2CMasterInitExpClk)           \

VOID_FUNC(I2CMasterInitExpClk, uint32_t, uint32_t, bool);
