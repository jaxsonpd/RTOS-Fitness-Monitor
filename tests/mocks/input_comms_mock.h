#include "fff.h"
#include "input_comms.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef FFF_MOCK_IMPL
#define VOID_FUNC FAKE_VOID_FUNC
#define VALUE_FUNC FAKE_VALUE_FUNC
#else
#define VOID_FUNC DECLARE_FAKE_VOID_FUNC
#define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_INPUT_COMMS_FAKES_LIST(FUNC) \
    FUNC(input_comms_init)          \
    FUNC(input_comms_send)          \
    FUNC(input_comms_receive)


VALUE_FUNC(bool, input_comms_init);
VALUE_FUNC(bool, input_comms_send, inputCommMsg_t);
VALUE_FUNC(inputCommMsg_t, input_comms_receive);