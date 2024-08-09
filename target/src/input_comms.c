/**
 * @file input_comms.c
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-08-08
 * @brief Definitions for the input comms module to handle sending information
 * from the input manager to the display manager
 */

#include <stdint.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "queue.h"

#include "input_comms.h"

QueueHandle_t g_input_display_manager_queue;

bool input_comms_init(void) {
    g_input_display_manager_queue = xQueueCreate(5, sizeof(inputCommMsg_t));

    return g_input_display_manager_queue != NULL;
}


bool input_comms_send(inputCommMsg_t msg) {
    BaseType_t xStatus = xQueueSendToBack(g_input_display_manager_queue,
        (const void* const)msg, 0);

    return xStatus == pdTRUE;
}

inputCommMsg_t input_comms_receive(void) {
    inputCommMsg_t value;
    BaseType_t xStatus = xQueueReceive(g_input_display_manager_queue,
        &value, 0);

    if (xStatus != pdTRUE) {
        return NO_MESSAGES;
    }

    return value;
}