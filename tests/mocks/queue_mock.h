#include "fff.h"
#include "queue.h"

#ifdef FFF_MOCK_IMPL
#define VOID_FUNC FAKE_VOID_FUNC
#define VALUE_FUNC FAKE_VALUE_FUNC
#else
#define VOID_FUNC DECLARE_FAKE_VOID_FUNC
#define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_CIRCBUFT_FAKES_LIST(FUNC) \
    FUNC(xQueueCreate)                \
    FUNC(xQueueSendToBack)

VALUE_FUNC(QueueHandle_t, xQueueCreate, BaseType_t, BaseType_t);
VALUE_FUNC(BaseType_t, xQueueSendToBack, QueueHandle_t, const void *, TickType_t);