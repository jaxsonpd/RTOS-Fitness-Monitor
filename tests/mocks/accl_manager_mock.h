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
    FUNC(acclInit)                              \
    FUNC(acclPoll)                              \
    FUNC(acclMean)                              \

VALUE_FUNC(bool, acclInit);
VOID_FUNC(acclPoll);
VALUE_FUNC(vector3_t, acclMean);