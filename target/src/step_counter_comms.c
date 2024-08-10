/**
 * @file step_counter_comms.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-10-08
 * @brief Implementation of step count value communication
 */


#include <stdint.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "semphr.h"

#include "step_counter_comms.h"

static uint32_t g_step_count;
SemaphoreHandle_t g_step_count_mutex;
static bool mutex_enabled = false;

bool step_counter_comms_init(void) {
    g_step_count_mutex = xSemaphoreCreateMutex();
    mutex_enabled = g_step_count != NULL;
    g_step_count = 0;

    return mutex_enabled;
}

bool step_counter_set(uint32_t stepsAccumulated) {
    if (mutex_enabled == false) {
        return 0;
    }
    xSemaphoreTake(g_step_count_mutex, portMAX_DELAY);
    g_step_count = g_step_count + stepsAccumulated;
    xSemaphoreGive(g_step_count_mutex);
    return true;
}

uint32_t step_counter_get(void) {
    uint32_t step_count;
    if (mutex_enabled == false) {
        return 0;
    }
    xSemaphoreTake(g_step_count_mutex, portMAX_DELAY);
    step_count = g_step_count;
    xSemaphoreGive(g_step_count_mutex);
    return step_count;
}
