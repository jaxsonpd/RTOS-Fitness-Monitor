#include "fff.h"

#include "hal/accl_hal.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_ACCLHAL_FAKES_LIST(FUNC) \
    FUNC(accl_chip_init)              \
    FUNC(accl_data_get)               \

VOID_FUNC(accl_chip_init);
VOID_FUNC(accl_data_get, int16_t*);