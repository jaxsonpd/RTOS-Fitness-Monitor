#include "fff.h"

#include "vector3.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_VECTOR3_FAKES_LIST(FUNC) \
    FUNC(v3_constructor)             \
    FUNC(v3_magnitude)               \

VALUE_FUNC(vector3_t, v3_constructor, int16_t, int16_t, int16_t);
VALUE_FUNC(uint32_t, v3_magnitude, vector3_t);