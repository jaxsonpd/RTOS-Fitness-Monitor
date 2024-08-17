#include "fff.h"
#include "comms/pot_comms.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef FFF_MOCK_IMPL
#define VOID_FUNC FAKE_VOID_FUNC
#define VALUE_FUNC FAKE_VALUE_FUNC
#else
#define VOID_FUNC DECLARE_FAKE_VOID_FUNC
#define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_POT_COMMS_FAKES_LIST(FUNC) \
    FUNC(pot_comms_init)               \
    FUNC(pot_set)                      \
    FUNC(pot_get)                      \

VALUE_FUNC(bool, pot_comms_init);
VALUE_FUNC(bool, pot_set, uint32_t);
VALUE_FUNC(uint32_t, pot_get);