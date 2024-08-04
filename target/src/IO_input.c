/**
 * @file IO_input.c
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-07-31
 * @brief This module allows for interaction with the IO on the orbit board.
 */

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/tm4c123gh6pm.h" // Board specific defines (for PF0)

#include "IO_input.h"

#define DEBOUNCE_NUMBER 3 ///< number of times before state change

typedef struct input_mode_info
{
    uint32_t periph;
    uint32_t port_base;
    uint8_t pin;
    uint32_t type;
    bool normal;
    input_mode_t name;
    bool state;
    bool value;
    uint8_t count;
    bool flag;
    uint8_t deb_limit;
} input_mode_info_t;

/// UP button on orbit board
input_mode_info_t g_up_button = {
    .periph = SYSCTL_PERIPH_GPIOE,
    .port_base = GPIO_PORTE_BASE,
    .pin = GPIO_PIN_0,
    .type = GPIO_PIN_TYPE_STD_WPD,
    .normal = true,
    .name = UP_BUTTON};

/// DOWN button on orbit board
input_mode_info_t g_down_button = {
    .periph = SYSCTL_PERIPH_GPIOD,
    .port_base = GPIO_PORTD_BASE,
    .pin = GPIO_PIN_2,
    .type = GPIO_PIN_TYPE_STD_WPD,
    .normal = false,
    .name = DOWN_BUTTON};

/// LEFT button on Tiva board
input_mode_info_t g_left_button = {
    .periph = SYSCTL_PERIPH_GPIOF,
    .port_base = GPIO_PORTF_BASE,
    .pin = GPIO_PIN_4,
    .type = GPIO_PIN_TYPE_STD_WPU,
    .normal = true,
    .name = LEFT_BUTTON};

/// RIGHT button on Tiva board
input_mode_info_t g_right_button = {
    .periph = SYSCTL_PERIPH_GPIOF,
    .port_base = GPIO_PORTF_BASE,
    .pin = GPIO_PIN_0,
    .type = GPIO_PIN_TYPE_STD_WPU,
    .normal = true,
    .name = RIGHT_BUTTON};

/// SW1 Right switch on orbit board
input_mode_info_t g_right_switch = {
    .periph = SYSCTL_PERIPH_GPIOA,
    .port_base = GPIO_PORTA_BASE,
    .pin = GPIO_PIN_7,
    .type = GPIO_PIN_TYPE_STD_WPD,
    .normal = false,
    .name = RIGHT_SWITCH};

input_mode_info_t g_mode_arr[5] = {0};

bool input_init(bool override_portf)
{
    g_mode_arr[0] = g_up_button;
    g_mode_arr[1] = g_down_button;
    g_mode_arr[2] = g_left_button;
    g_mode_arr[3] = g_right_button;
    g_mode_arr[4] = g_right_switch;

    // GPIO enable
    for (uint8_t i = 0; i < NUM_INPUT_MODES; i++)
    {
        SysCtlPeripheralEnable(g_mode_arr[i].periph);
        if (i == RIGHT_BUTTON && override_portf)
        {
            // Unlock right button
            GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
            GPIO_PORTF_CR_R |= GPIO_PIN_0; // PF0 unlocked
            GPIO_PORTF_LOCK_R = GPIO_LOCK_M;
        }
        GPIOPinTypeGPIOInput(g_mode_arr[i].port_base, g_mode_arr[i].pin);
        GPIOPadConfigSet(g_mode_arr[i].port_base, g_mode_arr[i].pin,
                         GPIO_STRENGTH_2MA, g_mode_arr[i].type);
    }

    // Clear states
    for (uint8_t i = 0; i < NUM_INPUT_MODES; i++)
    {
        g_mode_arr[i].count = 0;
        g_mode_arr[i].flag = false;
        g_mode_arr[i].value = g_mode_arr[i].normal;
        g_mode_arr[i].state = g_mode_arr[i].normal;
        g_mode_arr[i].deb_limit = DEBOUNCE_NUMBER;
    }

    return true;
}

bool input_get(input_mode_t input)
{
    return g_mode_arr[input].state != g_mode_arr[input].normal;
}

input_state_t input_check(input_mode_t input)
{
    if (g_mode_arr[input].flag)
    {
        g_mode_arr[input].flag = false;

        if (g_mode_arr[input].state == g_mode_arr[input].normal)
        {
            return RELEASED;
        }
        else
        {
            return PUSHED;
        }
    }
    return NO_CHANGE;
}

void input_update(void)
{
    int32_t value = 0;
    for (uint8_t i = 0; i < NUM_INPUT_MODES; i++)
    {
        value = GPIOPinRead(g_mode_arr[i].port_base, g_mode_arr[i].pin);
        g_mode_arr[i].value = (value == g_mode_arr[i].pin);

        if (g_mode_arr[i].value != g_mode_arr[i].state)
        {
            g_mode_arr[i].count++;
            if (g_mode_arr[i].count >= g_mode_arr[i].deb_limit)
            {
                g_mode_arr[i].state = g_mode_arr[i].value;
                g_mode_arr[i].flag = true;
                g_mode_arr[i].count = 0;
            }
        }
        else
        {
            g_mode_arr[i].count = 0;
        }
    }
}
