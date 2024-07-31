set(SRC_DIR ../target/src)
set(TIVAWARE_DIR ../target/libs/tivaware)
set(MOCK_DIR mocks)

set(SRC_DIR ../target/src)
set(TIVAWARE_DIR ../target/libs/tivaware)
set(MOCK_DIR mocks)

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(FitnessTrackerTests LANGUAGES C)

set(SRC_DIR ../target/src)

enable_testing()
add_subdirectory(unity)

add_unity_test(test_example test_example.c)
add_unity_test(test_fff test_fff.c)
add_unity_test(test_circular_buffer test_circular_buffer.c ${SRC_DIR}/circular_buffer_T.c)
add_unity_test(test_adc test_adc.c ${SRC_DIR}/ADC_read.c)
add_unity_test(test_IO_input test_IO_input.c ${SRC_DIR}/IO_input.c)

target_include_directories(test_circular_buffer PRIVATE ${SRC_DIR})
target_include_directories(test_IO_input PRIVATE ${SRC_DIR} ${TIVAWARE_DIR} ${MOCK_DIR})
target_include_directories(test_adc PRIVATE ${SRC_DIR} ${TIVAWARE_DIR} ${MOCK_DIR})