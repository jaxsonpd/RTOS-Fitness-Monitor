#include "fff.h"
#include "i2c_driver.h"

#include <stdint.h>
#include <stdbool.h>

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_I2CDRIVER_FAKES_LIST(FUNC) \
    FUNC(I2CGenTransmit)               \
    FUNC(I2CGenIsNotIdle)              \

VALUE_FUNC(char, I2CGenTransmit, char*, int32_t, bool, char);
VALUE_FUNC(bool,I2CGenIsNotIdle);