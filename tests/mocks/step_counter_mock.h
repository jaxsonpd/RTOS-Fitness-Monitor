#include "fff.h"
#include "step_counter.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef FFF_MOCK_IMPL
#define VOID_FUNC FAKE_VOID_FUNC
#define VALUE_FUNC FAKE_VALUE_FUNC
#else
#define VOID_FUNC DECLARE_FAKE_VOID_FUNC
#define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_STEP_COUNTER_FAKES_LIST(FUNC) \
    FUNC(step_counter_init)         \
    FUNC(detect_step)               \

VALUE_FUNC(bool, step_counter_init, uint16_t*);
VALUE_FUNC(bool, detect_step, vector3_t, uint32_t*);