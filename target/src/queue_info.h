/**
 * @file queue_info.h
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-08-07
 * @brief Defines for the values used in the message queues in the
 * application
 */

#ifndef QUEUE_INFO_H
#define QUEUE_INFO_H

#include <stdint.h>
#include <stdbool.h>

typedef enum in_dis_queue_info
{
    MOVE_SCREEN_LEFT,
    MOVE_SCREEN_RIGHT
} in_dis_queue_info_t;

#endif // QUEUE_INFO_H