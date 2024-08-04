/** 
 * @file gpio_mock.h
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-07-31
 * @brief Mocks for the TIVA GPIO module 
 */


#include <stdint.h>
#include <stdbool.h>

#include "fff.h"

#include "driverlib/gpio.h"


#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_GPIO_FAKES_LIST(FUNC)           \
    FUNC(GPIOPinTypeGPIOInput)              \
    FUNC(GPIOPadConfigSet)                  \
    FUNC(GPIOPinRead)                       

typedef void (*void_function_ptr_type)(void);

VOID_FUNC(GPIOPinTypeGPIOInput, uint32_t, uint8_t);
VOID_FUNC(GPIOPadConfigSet, uint32_t, uint8_t, uint32_t, uint32_t);
VALUE_FUNC(int32_t, GPIOPinRead, uint32_t, uint8_t);
