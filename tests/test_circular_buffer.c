
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "unity.h"

#include "utility/circular_buffer.h"

const uint8_t STANDARD_TEST_CAPACITY = 5;
static circBuf_t buff;

void setUp(void)
{
    circ_buf_init(&buff, STANDARD_TEST_CAPACITY);
}

void tearDown(void)
{
    circ_buf_free(&buff);
}

/* Helper functions */
void writeConsecutiveSequenceToBuffer(uint16_t start, uint16_t size)
{
    for (uint16_t i = 0; i < size; i++) {
      circ_buf_write(&buff, start + i);
    }
} 

void assertReadingSequence(uint16_t start, uint16_t size)
{
    for (uint16_t i = 0; i < size; i++) {
      TEST_ASSERT_EQUAL(start + i, circ_buf_read(&buff));
    }
} 

int32_t * reconstructBufferWithSize(uint16_t size)
{
  circ_buf_free(&buff);
  return circ_buf_init(&buff, size);
}

/* Test cases */

void test_new_buffer_is_empty(void)
{
    // Arrange: given buffer is empty

    // Act: when buffer is read
    int32_t value = circ_buf_read(&buff);

    // Assert: then 0 is returned
    TEST_ASSERT_EQUAL(BUF_EMPTY_VAL, value);
}

void test_single_element_in_single_element_out(void)
{
    // Arrange: given buffer has a single element
    circ_buf_write(&buff, 11);

    // Act: when buffer is read
    int32_t value = circ_buf_read(&buff);

    // Assert: then the same value is returned
    TEST_ASSERT_EQUAL(11, value);
}

void test_first_element_in_first_element_out(void)
{
    // Arrange
    writeConsecutiveSequenceToBuffer(20, STANDARD_TEST_CAPACITY);

    // Act/Assert
    assertReadingSequence(20, STANDARD_TEST_CAPACITY);
}

void test_write_and_read_indices_are_independent(void)
{
    for (uint8_t i = 0; i < STANDARD_TEST_CAPACITY; i++)
    {
      // Arrange: given one element is written
      circ_buf_write(&buff, 20 + i);

      // Act: when buffer is read
      int32_t value = circ_buf_read(&buff);

      // Assert: the last written element is returned
      TEST_ASSERT_EQUAL(20 + i, value);
    }
}

void test_buffer_is_clean_after_full_buffer_cycle_completed(void)
{
    // Check that read index doesn't overtake the write index 
    // Arange: given buffer is fully written to and and then fully read from
    writeConsecutiveSequenceToBuffer(20, STANDARD_TEST_CAPACITY);
    for (uint32_t i = 0; i < STANDARD_TEST_CAPACITY; i++) {
        circ_buf_read(&buff);
    }

    // Assert: notify that buffer is empty
    TEST_ASSERT_EQUAL(BUF_EMPTY_VAL, circ_buf_read(&buff));
}

void test_buffer_is_circular(void)
{
    // Arange: given buffer is fully written to and then fully read from
    writeConsecutiveSequenceToBuffer(20, STANDARD_TEST_CAPACITY);
    for (uint32_t i = 0; i < STANDARD_TEST_CAPACITY; i++) {
        circ_buf_read(&buff);
    }

    // Arrange: given a new value is written
    circ_buf_write(&buff, 2);

    // Act: when buffer is read
    int32_t data = circ_buf_read(&buff); 

    // Assert: the last written element is returned
    TEST_ASSERT_EQUAL(2, data);
}

void test_no_values_overwritten_after_full(void)
{
    // Arrange: given buffer is filled to capacity

    // Given: when one more element is written to buffer
    writeConsecutiveSequenceToBuffer(20, STANDARD_TEST_CAPACITY);
    bool result = circ_buf_write(&buff, 10);

    // Assert: first element in, first element out, no overflow
    TEST_ASSERT_EQUAL(false, result); // Enure notified of data loss
    TEST_ASSERT_EQUAL(20, circ_buf_read(&buff));
}

void test_min_capacity_when_buffer_is_created_then_buffer_empty(void)
{
    // Arrange
    reconstructBufferWithSize(1);

    // Act/Assert
    TEST_ASSERT_EQUAL(BUF_EMPTY_VAL, circ_buf_read(&buff));
}

void test_min_capacity_when_single_element_written_to_buffer_then_same_value_is_read(void)
{
    // Arrange
    reconstructBufferWithSize(1);

    // Act
    circ_buf_write(&buff, 87);

    // Act/Assert
    TEST_ASSERT_EQUAL(87, circ_buf_read(&buff));
}

void test_capacity_0_invalid(void)
{
    // Arrange/Act

    // Assert: the return value of initCircBuf is NULL
    TEST_ASSERT_EQUAL(NULL, circ_buf_init(&buff, 0));
}

void test_capacity_higher_than_max_invalid(void) {
    // Arrange/Act
    
    // Assert: the return value of initCircBuf is NULL
    TEST_ASSERT_EQUAL(NULL, circ_buf_init(&buff, 2001));

    // Allow for overflow in size around a uint32_t
    TEST_ASSERT_NOT_EQUAL(NULL, circ_buf_init(&buff, UINT32_MAX+4));
}
