/**
 * @file potentiometer_comms.c
 * @author Daniel Hawes (dha144@uclive.ac.nz)
 * @date 2024-08-12
 * @brief Interface to handle communication of the step
 */

#include <stdint.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "semphr.h"

#include "potentiometer_comms.h"

static uint32_t g_potentiometer_value;
SemaphoreHandle_t g_potentiometer_mutex;
static bool mutex_enabled = false;

bool potentiometer_comms_init(void) {
    g_potentiometer_mutex = xSemaphoreCreateMutex();
    mutex_enabled = g_potentiometer_mutex != NULL;
    g_potentiometer_value = 0;

    return mutex_enabled;
}

bool potentiometer_set(uint32_t potentiometerRead) {
    if (mutex_enabled == false) {
        return 0;
    }

    xSemaphoreTake(g_potentiometer_mutex, portMAX_DELAY);
    g_potentiometer_value += potentiometerRead;
    xSemaphoreGive(g_potentiometer_mutex);
}


uint32_t potentiometer_get(void) {
    uint32_t potentiometer_value;
    if (mutex_enabled == false) {
        return 0;
    }
    xSemaphoreTake(g_potentiometer_mutex, portMAX_DELAY);
    potentiometer_value = g_potentiometer_value;
    g_potentiometer_value = 0;
    xSemaphoreGive(g_potentiometer_mutex);
    return potentiometer_value;
}