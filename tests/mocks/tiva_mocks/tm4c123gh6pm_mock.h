// This file is very hacky but is required as otherwise modules that import
// this header will write directly to a raw register which will cause a seg
// fault during testing.

#include <stdint.h>

uint32_t gpio_portf_lock_r;
uint32_t gpio_portf_cr_r;

#define GPIO_PORTF_LOCK_R gpio_portf_lock_r
#define GPIO_PORTF_CR_R gpio_portf_cr_r

#define GPIO_LOCK_KEY 0x4C4F434B
#define GPIO_LOCK_M 0xFFFFFFFF