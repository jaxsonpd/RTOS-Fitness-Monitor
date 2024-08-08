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
void test_input_comms_init_succesful(void)
{
    TEST_ASSERT_TRUE(input_comms_init());
}

void test_input_comms_init_creates_queue(void)
{
    input_comms_init();

    TEST_ASSERT_EQUAL(1, xQueueCreate_fake.call_count);
}

void test_input_comms_init_correct_length(void)
{
    input_comms_init();

    TEST_ASSERT_EQUAL(5, xQueueCreate_fake.arg0_val);
}

void test_input_comms_init_correct_size(void)
{
    input_comms_init();

    TEST_ASSERT_EQUAL(sizeof(inputCommMsg_t), xQueueCreate_fake.arg1_val);
}