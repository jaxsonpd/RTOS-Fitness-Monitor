#include <stdint.h>
#include <stdbool.h>

#include "unity.h"
#include "clock_hal.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "tiva_mocks/sysctl_mock.h"


/* Helper functions */
static uint32_t callback_sum;

void reset_fff(void)
{
    FFF_SYSCTL_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

/* Unity setup and teardown */
void setUp(void)
{
    reset_fff();
}

void tearDown(void)
{
    
}

void test_init_clock_initialises_clock(void)
{
    init_clock();

    TEST_ASSERT_EQUAL(1, SysCtlClockSet_fake.call_count);
    TEST_ASSERT_EQUAL(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ, SysCtlClockSet_fake.arg0_val);
}