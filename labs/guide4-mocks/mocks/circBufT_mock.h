#include "fff.h"
#include "circular_buffer_T.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_circular_buffer_T_FAKES_LIST(FUNC)         \
    FUNC(CircBuf_init)                    \
    FUNC(CircBuf_write)                   \
    FUNC(CircBuf_read)                    \
    FUNC(CircBuf_free)      

VALUE_FUNC(int32_t *, initCircBuf, CircBuf_t *, uint32_t);
VOID_FUNC(CircBuf_write, CircBuf_t *, int32_t);
VALUE_FUNC(int32_t, CircBuf_read, CircBuf_t *);
VOID_FUNC(CircBuf_free, CircBuf_t *);
