#include <stdint.h>
#include <stdbool.h>

#include "driverlib/sysctl.h"
#include "clock_hal.h"

void init_clock(void)
{
    // Set the clock rate to 20 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
        SYSCTL_XTAL_16MHZ);
}