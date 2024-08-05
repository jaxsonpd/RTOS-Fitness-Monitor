#include "fff.h"

#include "accl_manager.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_ACCLMANAGER_FAKES_LIST(FUNC) \
    FUNC(initAcclChip)              \
    FUNC(getAcclData)               \

VOID_FUNC(initAcclChip);
VOID_FUNC(getAcclData, int16_t*);