#include "fff.h"
#include <stdbool.h>
#include "driverlib/sysctl.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_SYSCTL_FAKES_LIST(FUNC)     \
    FUNC(SysCtlClockSet)                \
    FUNC(SysCtlPeripheralEnable)        \
    FUNC(SysCtlClockGet)                \
    FUNC(SysCtlPeripheralReset)         \
    FUNC(SysCtlPeripheralReady)         \
    FUNC(SysCtlDelay)                   \

VOID_FUNC(SysCtlClockSet, uint32_t);
VOID_FUNC(SysCtlPeripheralEnable, uint32_t);
VALUE_FUNC(uint32_t, SysCtlClockGet);
VOID_FUNC(SysCtlPeripheralReset, uint32_t);
VALUE_FUNC(bool, SysCtlPeripheralReady, uint32_t);
VOID_FUNC(SysCtlDelay, uint32_t);