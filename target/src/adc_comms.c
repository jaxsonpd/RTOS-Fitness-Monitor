#include "adc_comms.h"
#include "FreeRTOS.h"
#include "semphr.h"

#include <stdbool.h>

static uint32_t g_adc_value;
SemaphoreHandle_t g_adc_mutex;
static bool mutex_enabled = false;

bool adc_comms_init(void) {
    g_adc_mutex = xSemaphoreCreateMutex();
    mutex_enabled = g_adc_mutex != NULL;
    g_adc_value = 0;

    return mutex_enabled;
}

bool adc_set(uint32_t adc_value) {
    if (mutex_enabled == false) {
        return 0;
    }
    xSemaphoreTake(g_adc_mutex, portMAX_DELAY);
    g_adc_value = adc_value;
    xSemaphoreGive(g_adc_mutex);
    return true;
}

uint32_t adc_get(void) {
    uint32_t adc_value;
    if (mutex_enabled == false) {
        return 0;
    }
    xSemaphoreTake(g_adc_mutex, portMAX_DELAY);
    adc_value = g_adc_value;
    g_adc_value = 0;
    xSemaphoreGive(g_adc_mutex);
    return adc_value;
}
