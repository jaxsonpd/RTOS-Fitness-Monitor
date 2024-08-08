/**
 * @file test_input_comms.c
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-08-08
 * @brief Test the input comms model
 */

#include <stdint.h>
#include <stdbool.h>

#include "unity.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "queue_mock.h"

#include "input_comms.h"

void setUp(void)
{
}

void tearDown(void)
{
}

// ========================== Tests ========================
void test_input_comms_init_creates_queue(void)
{
}