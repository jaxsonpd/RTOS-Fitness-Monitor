set(SRC_DIR ../target/src)
set(TIVAWARE_DIR ../target/libs/tivaware)
set(MOCK_DIR mocks)
set(RTOS_DIR_INCLUDE ../target/libs/freertos/include)
set(RTOS_ARM ../target/libs/freertos/portable/GCC/ARM_CM4F)
set(RTOS_CONFIG_DIR ../target)

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(FitnessTrackerTests LANGUAGES C)

enable_testing()
add_subdirectory(unity)

add_unity_test(test_circular_buffer test_circular_buffer.c ${SRC_DIR}/circular_buffer_T.c)
add_unity_test(test_adc test_adc.c ${SRC_DIR}/adc_manager.c)
add_unity_test(test_accl_manager test_accl_manager.c ${SRC_DIR}/accl_manager.c)
add_unity_test(test_IO_input test_IO_input.c ${SRC_DIR}/IO_input.c)
add_unity_test(test_input_manager test_input_manager.c ${SRC_DIR}/input_manager.c)
add_unity_test(test_input_comms test_input_comms.c ${SRC_DIR}/input_comms.c)
add_unity_test(test_step_counter_manager test_step_counter_manager.c ${SRC_DIR}/step_counter_manager.c)
add_unity_test(test_step_counter_comms test_step_counter_comms.c ${SRC_DIR}/step_counter_comms.c)
add_unity_test(test_pot_comms test_pot_comms.c ${SRC_DIR}/pot_comms.c)
add_unity_test(test_adc_hal test_adc_hal.c ${SRC_DIR}/adc_hal_tiva.c)

target_include_directories(test_circular_buffer PRIVATE ${SRC_DIR})
target_include_directories(test_IO_input PRIVATE ${SRC_DIR} ${TIVAWARE_DIR} ${MOCK_DIR})
target_compile_definitions(test_IO_input PRIVATE UNIT_TESTING)

target_include_directories(test_adc PRIVATE ${SRC_DIR} ${RTOS_ARM} ${RTOS_DIR_INCLUDE} ${MOCK_DIR} ${RTOS_CONFIG_DIR})
target_include_directories(test_accl_manager PRIVATE ${SRC_DIR} ${TIVAWARE_DIR} ${MOCK_DIR})
target_compile_definitions(test_accl_manager PRIVATE PART_TM4C123GH6PM)

target_include_directories(test_adc_hal PRIVATE ${SRC_DIR} ${TIVAWARE_DIR} ${MOCK_DIR})
target_compile_definitions(test_adc_hal PRIVATE PART_TM4C123GH6PM)

target_include_directories(test_input_manager PRIVATE ${SRC_DIR} ${MOCK_DIR} ${RTOS_ARM} ${RTOS_DIR_INCLUDE} ${RTOS_CONFIG_DIR})
target_compile_definitions(test_input_manager PRIVATE UNIT_TESTING)

target_include_directories(test_input_comms PRIVATE ${SRC_DIR} ${RTOS_ARM} ${RTOS_DIR_INCLUDE} ${MOCK_DIR} ${RTOS_CONFIG_DIR})

target_include_directories(test_step_counter_manager PRIVATE ${SRC_DIR} ${MOCK_DIR} ${RTOS_ARM} ${RTOS_DIR_INCLUDE} ${RTOS_CONFIG_DIR})
target_compile_definitions(test_step_counter_manager PRIVATE UNIT_TESTING)

target_include_directories(test_step_counter_comms PRIVATE ${SRC_DIR} ${RTOS_ARM} ${RTOS_DIR_INCLUDE} ${MOCK_DIR} ${RTOS_CONFIG_DIR})
target_include_directories(test_pot_comms PRIVATE ${SRC_DIR} ${RTOS_ARM} ${RTOS_DIR_INCLUDE} ${MOCK_DIR} ${RTOS_CONFIG_DIR})

