#include "fff.h"
#include "step_counter_comms.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef FFF_MOCK_IMPL
#define VOID_FUNC FAKE_VOID_FUNC
#define VALUE_FUNC FAKE_VALUE_FUNC
#else
#define VOID_FUNC DECLARE_FAKE_VOID_FUNC
#define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_STEP_COUNTER_COMMS_FAKES_LIST(FUNC) \
    FUNC(step_counter_comms_init)               \
    FUNC(step_counter_set)                      \
    FUNC(step_counter_get)                      \


VALUE_FUNC(bool, step_counter_comms_init);
VALUE_FUNC(bool, step_counter_set, uint32_t);
VALUE_FUNC(uint32_t, step_counter_get);