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
    FUNC(circ_buf_init)                    \
    FUNC(circ_buf_write)                   \
    FUNC(circ_buf_read)                    \
    FUNC(circ_buf_free)      

VALUE_FUNC(int32_t *, initCircBuf, circBuf_t *, uint32_t);
VOID_FUNC(circ_buf_write, circBuf_t *, int32_t);
VALUE_FUNC(int32_t, circ_buf_read, circBuf_t *);
VOID_FUNC(circ_buf_free, circBuf_t *);
