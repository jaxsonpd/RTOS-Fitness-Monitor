#include "fff.h"
#include "accl_manager.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef FFF_MOCK_IMPL
#define VOID_FUNC FAKE_VOID_FUNC
#define VALUE_FUNC FAKE_VALUE_FUNC
#else
#define VOID_FUNC DECLARE_FAKE_VOID_FUNC
#define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_ACCL_MANAGER_FAKES_LIST(FUNC) \
    FUNC(accl_init)                              \
    FUNC(accl_poll)                              \
    FUNC(accl_mean)                              \

VALUE_FUNC(bool, accl_init);
VOID_FUNC(accl_poll);
VALUE_FUNC(vector3_t, accl_mean);