set(SRC_DIR ../target/src)
set(TIVAWARE_DIR ../target/libs/tivaware)
set(MOCK_DIR mocks)

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(FitnessTrackerTests LANGUAGES C)

enable_testing()
add_subdirectory(unity)

add_unity_test(test_circular_buffer test_circular_buffer.c ${SRC_DIR}/circular_buffer_T.c)
add_unity_test(test_adc test_adc.c ${SRC_DIR}/ADC_read.c)
add_unity_test(test_accl_manager test_accl_manager.c ${SRC_DIR}/accl_manager.c)
add_unity_test(test_IO_input test_IO_input.c ${SRC_DIR}/IO_input.c)
add_unity_test(test_button_manager test_button_manager.c ${SRC_DIR}/button_manager.c)

target_include_directories(test_circular_buffer PRIVATE ${SRC_DIR})
target_include_directories(test_IO_input PRIVATE ${SRC_DIR} ${TIVAWARE_DIR} ${MOCK_DIR})
target_include_directories(test_adc PRIVATE ${SRC_DIR} ${TIVAWARE_DIR} ${MOCK_DIR})
target_include_directories(test_accl_manager PRIVATE ${SRC_DIR} ${TIVAWARE_DIR} ${MOCK_DIR})
target_include_directories(test_button_manager PRIVATE ${SRC_DIR} ${MOCK_DIR})

target_compile_definitions(test_accl_manager PRIVATE PART_TM4C123GH6PM)
