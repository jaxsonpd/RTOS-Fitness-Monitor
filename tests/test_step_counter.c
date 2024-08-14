#include "unity.h"
#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "step_counter.h"

#include "vector3_mock.h"

void setUp(void) {
    FFF_RESET_HISTORY();
}

void tearDown(void) {
    
}

void test_step_counter_init_valid(void) {
    uint16_t period = 100;
    bool result = step_counter_init(&period);
    TEST_ASSERT_TRUE(result);
}

void test_step_counter_init_null_pointer(void) {
    bool result = step_counter_init(NULL);
    TEST_ASSERT_FALSE(result);
}

// TODO test step detect???? Could require extensive arrange