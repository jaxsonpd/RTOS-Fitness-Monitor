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
    g_pot_mutex = 0;

    return mutex_enabled;
}

bool pot_set(uint32_t pot_value) {
    if (mutex_enabled == false) {
        return 0;
    }
    xSemaphoreTake(g_pot_mutex, portMAX_DELAY);
    g_pot_value = pot_value;
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
    return g_pot_value;
}
