#include "fff.h"
#include "utils/ustdlib.h"

#ifdef FFF_MOCK_IMPL
#define VOID_FUNC FAKE_VOID_FUNC
#define VALUE_FUNC FAKE_VALUE_FUNC
#define VALUE_FUNC_VARARG FAKE_VALUE_FUNC_VARARG
#else
#define VOID_FUNC DECLARE_FAKE_VOID_FUNC
#define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#define VALUE_FUNC_VARARG DECLARE_FAKE_VALUE_FUNC_VARARG
#endif

#define FFF_USTDLIB_FAKES_LIST(FUNC) \
    FUNC(usnprintf)

VALUE_FUNC_VARARG(int, usnprintf, char * restrict, size_t, const char * restrict, ...);