/**
 * @file input_hal_mock.h
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-08-07
 * @brief Mock for the IO input module to allow for unit testing
 */

#include <stdint.h>
#include <stdbool.h>

#include "fff.h"

#include "hal/input_hal.h"

#ifdef FFF_MOCK_IMPL
#define VOID_FUNC FAKE_VOID_FUNC
#define VALUE_FUNC FAKE_VALUE_FUNC
#else
#define VOID_FUNC DECLARE_FAKE_VOID_FUNC
#define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_INPUT_HAL_FAKES_LIST(FUNC) \
    FUNC(input_init)                   \
    FUNC(input_get)                    \
    FUNC(input_check)                  \
    FUNC(input_update)

typedef void (*void_function_ptr_type)(void);

VALUE_FUNC(bool, input_init);
VALUE_FUNC(bool, input_get, input_mode_t);
VALUE_FUNC(input_state_t, input_check, input_mode_t);
VOID_FUNC(input_update);