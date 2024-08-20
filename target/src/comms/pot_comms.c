/**
 * @file pot_comms.c
 * @author Daniel Hawes (dha144@uclive.ac.nz)
 * @date 2024-08-12
 * @brief Definitions for the pot comms interface
 * Handles mutex for communication with potentiometer read thread.
 */


#include "pot_comms.h"
#include "FreeRTOS.h"
#include "semphr.h"

#include <stdbool.h>
#include <stdint.h>

static uint32_t g_pot_value;
SemaphoreHandle_t g_pot_mutex;
static bool mutex_enabled = false;

bool pot_comms_init(void) {
    g_pot_mutex = xSemaphoreCreateMutex();
    mutex_enabled = g_pot_mutex != NULL;
    g_pot_value = 0;

    return mutex_enabled;
}

bool pot_set(uint32_t pot_value_passed) {
    if (mutex_enabled == false) {
        return 0;
    }
    xSemaphoreTake(g_pot_mutex, portMAX_DELAY);
    g_pot_value = pot_value_passed;
    xSemaphoreGive(g_pot_mutex);
    return true;
}

uint32_t pot_get(void) {
    uint32_t pot_value;
    if (mutex_enabled == false) {
        return 0;
    }
    xSemaphoreTake(g_pot_mutex, portMAX_DELAY);
    pot_value = g_pot_value;
    g_pot_value = 0;
    xSemaphoreGive(g_pot_mutex);
    return pot_value;
}
