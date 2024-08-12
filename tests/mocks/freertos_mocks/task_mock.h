#include "fff.h"
#include "FreeRTOS.h"
#include "task.h"

#ifdef FFF_MOCK_IMPL
#define VOID_FUNC FAKE_VOID_FUNC
#define VALUE_FUNC FAKE_VALUE_FUNC
#else
#define VOID_FUNC DECLARE_FAKE_VOID_FUNC
#define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_TASK_FAKES_LIST(FUNC) \
    FUNC(vTaskDelay)              \


VOID_FUNC(vTaskDelay, const TickType_t)
VALUE_FUNC(TickType_t, xTaskGetTickCount);