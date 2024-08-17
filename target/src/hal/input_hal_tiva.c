/**
 * @file input_hal_tiva.c
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-07-31
 * @brief This module allows for interaction with the IO on the orbit board.
 */

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

#ifndef UNIT_TESTING
#include "inc/tm4c123gh6pm.h" // Board specific defines (for PF0)
#else
#include "tiva_mocks/tm4c123gh6pm_mock.h"
#endif // UNIT_TESTING

#include "input_hal.h"


/**
 * @struct input_mode_info_t
 *  A structure to store information about a input mode (button or switch)
 * 
 */
typedef struct input_mode_info {
    uint32_t periph;            ///< Gpio peripheral for the device
    uint32_t port_base;         ///< Gpio base port
    uint8_t pin;                ///< Gpio pin
    uint32_t type;              ///< Gpio type pull down etc.
    bool normal;                ///< Normal state of the device (off state)
    input_mode_t name;          ///< Corrsponding name of the device
    bool state;                 ///< Current state after debouncing
    bool value;                 ///< Raw value of the device
    uint8_t count;              ///< Number of times at the given value
    bool flag;                  ///< Wether the input has changed
    uint8_t deb_limit;          ///< Number of counts before state change
} input_mode_info_t;

/// UP button on orbit board
input_mode_info_t g_up_button = {
    .periph = SYSCTL_PERIPH_GPIOE,
    .port_base = GPIO_PORTE_BASE,
    .pin = GPIO_PIN_0,
    .type = GPIO_PIN_TYPE_STD_WPD,
    .normal = true,
    .name = UP_BUTTON };

/// DOWN button on orbit board
input_mode_info_t g_down_button = {
    .periph = SYSCTL_PERIPH_GPIOD,
    .port_base = GPIO_PORTD_BASE,
    .pin = GPIO_PIN_2,
    .type = GPIO_PIN_TYPE_STD_WPD,
    .normal = false,
    .name = DOWN_BUTTON };

/// LEFT button on Tiva board
input_mode_info_t g_left_button = {
    .periph = SYSCTL_PERIPH_GPIOF,
    .port_base = GPIO_PORTF_BASE,
    .pin = GPIO_PIN_4,
    .type = GPIO_PIN_TYPE_STD_WPU,
    .normal = true,
    .name = LEFT_BUTTON };

/// RIGHT button on Tiva board
input_mode_info_t g_right_button = {
    .periph = SYSCTL_PERIPH_GPIOF,
    .port_base = GPIO_PORTF_BASE,
    .pin = GPIO_PIN_0,
    .type = GPIO_PIN_TYPE_STD_WPU,
    .normal = true,
    .name = RIGHT_BUTTON };

/// SW1 Right switch on orbit board
input_mode_info_t g_right_switch = {
    .periph = SYSCTL_PERIPH_GPIOA,
    .port_base = GPIO_PORTA_BASE,
    .pin = GPIO_PIN_7,
    .type = GPIO_PIN_TYPE_STD_WPD,
    .normal = false,
    .name = RIGHT_SWITCH };

input_mode_info_t g_left_switch = {
    .periph = SYSCTL_PERIPH_GPIOA,
    .port_base = GPIO_PORTA_BASE,
    .pin = GPIO_PIN_6,
    .type = GPIO_PIN_TYPE_STD_WPD,
    .normal = false,
    .name = LEFT_SWITCH
};

input_mode_info_t g_mode_arr[6] = { 0 };

bool input_init(void) {
    g_mode_arr[0] = g_up_button;
    g_mode_arr[1] = g_down_button;
    g_mode_arr[2] = g_left_button;
    g_mode_arr[3] = g_right_button;
    g_mode_arr[4] = g_right_switch;
    g_mode_arr[5] = g_left_switch;

    // GPIO enable
    for (uint8_t i = 0; i < NUM_INPUT_MODES; i++) {
        SysCtlPeripheralEnable(g_mode_arr[i].periph);\
            while (!SysCtlPeripheralReady(g_mode_arr[i].periph));
        if (i == RIGHT_BUTTON) {
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
    for (uint8_t i = 0; i < NUM_INPUT_MODES; i++) {
        g_mode_arr[i].count = 0;
        g_mode_arr[i].flag = false;
        g_mode_arr[i].value = g_mode_arr[i].normal;
        g_mode_arr[i].state = g_mode_arr[i].normal;
        g_mode_arr[i].deb_limit = DEBOUNCE_NUMBER;
    }

    return true;
}

bool input_get(input_mode_t input) {
    return g_mode_arr[input].state != g_mode_arr[input].normal;
}

input_state_t input_check(input_mode_t input) {
    if (g_mode_arr[input].flag) {
        g_mode_arr[input].flag = false;

        if (g_mode_arr[input].state == g_mode_arr[input].normal) {
            return RELEASED;
        } else {
            return PUSHED;
        }
    }
    return NO_CHANGE;
}

void input_update(void) {
    int32_t value = 0;
    for (uint8_t i = 0; i < NUM_INPUT_MODES; i++) {
        value = GPIOPinRead(g_mode_arr[i].port_base, g_mode_arr[i].pin);
        g_mode_arr[i].value = (value == g_mode_arr[i].pin);

        if (g_mode_arr[i].value != g_mode_arr[i].state) {
            g_mode_arr[i].count++;
            if (g_mode_arr[i].count >= g_mode_arr[i].deb_limit) {
                g_mode_arr[i].state = g_mode_arr[i].value;
                g_mode_arr[i].flag = true;
                g_mode_arr[i].count = 0;
            }
        } else {
            g_mode_arr[i].count = 0;
        }
    }
}
