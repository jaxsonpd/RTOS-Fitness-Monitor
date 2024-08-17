#include <stdint.h>
#include <stdbool.h>

#include "fff.h"

#include "hal/i2c_hal.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif  

#define FFF_I2CHAL_FAKES_LIST(FUNC) \
    FUNC(i2c_hal_init)               \
    FUNC(i2c_hal_write)              \
    FUNC(i2c_hal_read)               \

VOID_FUNC(i2c_hal_init, uint8_t);
VALUE_FUNC(char, i2c_hal_write, uint8_t, char*, int32_t, char);
VALUE_FUNC(char, i2c_hal_read, uint8_t, char*, int32_t, char);
