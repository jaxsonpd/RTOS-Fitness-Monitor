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
#include "inc/tm4c123gh6pm.h"  // Board specific defines (for PF0)

#include "IO_input.h"

#define NUM_MODES 5 ///< number of input modes


typedef struct input_mode_info {
    uint32_t periph;
    uint32_t port_base;
    uint8_t pin;
    uint32_t type;
    bool normal;
    input_mode_t name;
    bool state;
    uint8_t count;
    bool flag;
} input_mode_info_t;

/// UP button on orbit board
input_mode_info_t g_up_button = {
    .periph = SYSCTL_PERIPH_GPIOE,
    .port_base = GPIO_PORTE_BASE,
    .pin = GPIO_PIN_0,
    .type = GPIO_PIN_TYPE_STD_WPD,
    .normal = true,
    .name = UP_BUTTON
};

/// DOWN button on orbit board
input_mode_info_t g_down_button = {
    .periph = SYSCTL_PERIPH_GPIOD,
    .port_base = GPIO_PORTD_BASE,
    .pin = GPIO_PIN_2,
    .type = GPIO_PIN_TYPE_STD_WPD,
    .normal = false,
    .name = DOWN_BUTTON
};

/// LEFT button on Tiva board
input_mode_info_t g_left_button = {
    .periph = SYSCTL_PERIPH_GPIOF,
    .port_base = GPIO_PORTF_BASE,
    .pin = GPIO_PIN_4,
    .type = GPIO_PIN_TYPE_STD_WPU,
    .normal = true,
    .name = LEFT_BUTTON
};

/// RIGHT button on Tiva board
input_mode_info_t g_right_button = {
    .periph = SYSCTL_PERIPH_GPIOF,
    .port_base = GPIO_PORTF_BASE,
    .pin = GPIO_PIN_0,
    .type = GPIO_PIN_TYPE_STD_WPU,
    .normal = true,
    .name = RIGHT_BUTTON
};

/// SW1 Right switch on orbit board
input_mode_info_t g_right_switch = {
    .periph = SYSCTL_PERIPH_GPIOA,
    .port_base = GPIO_PORTA_BASE,
    .pin = GPIO_PIN_7,
    .type = GPIO_PIN_TYPE_STD_WPD,
    .normal = false,
    .name = RIGHT_SWITCH
};


input_mode_info_t g_mode_arr[5] = {0};


bool input_init(void) {
    g_mode_arr[0] = g_up_button;
    g_mode_arr[1] = g_down_button;
    g_mode_arr[2] = g_left_button;
    g_mode_arr[3] = g_right_button;
    g_mode_arr[4] = g_right_switch;

    // GPIO enable
    for (uint8_t i = 0; i < NUM_MODES; i++) {
        SysCtlPeripheralEnable(g_mode_arr[i].periph);
        GPIOPinTypeGPIOInput(g_mode_arr[i].port_base, g_mode_arr[i].pin);
        GPIOPadConfigSet(g_mode_arr[i].port_base, g_mode_arr[i].pin, 
            GPIO_STRENGTH_2MA, g_mode_arr[i].type);
    }

    // Clear states
    for (uint8_t i = 0; i < NUM_MODES; i++) {
        g_mode_arr[i].count = 0;
        g_mode_arr[i].flag = false;
        g_mode_arr[i].state = g_mode_arr[i].normal;
    }

    return true;
}

bool input_get(input_mode_t mode) {

    return false;
}

void input_update(void) {

}
