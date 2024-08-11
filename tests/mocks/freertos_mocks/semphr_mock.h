#include "fff.h"
#include "FreeRTOS.h"
#include "semphr.h"

#ifdef FFF_MOCK_IMPL
#define VOID_FUNC FAKE_VOID_FUNC
#define VALUE_FUNC FAKE_VALUE_FUNC
#else
#define VOID_FUNC DECLARE_FAKE_VOID_FUNC
#define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_SEMPHR_FAKES_LIST(FUNC) \
    FUNC(xQueueCreateMutex)         \
    FUNC(xSemaphoreTake)            \
    FUNC(xSemaphoreGive)            \

VALUE_FUNC(QueueHandle_t, xQueueCreateMutex, const uint8_t);
VALUE_FUNC(BaseType_t, xQueueSemaphoreTake, QueueHandle_t, TickType_t);
VALUE_FUNC(BaseType_t, xQueueSemaphoreGive, QueueHandle_t, TickType_t);
