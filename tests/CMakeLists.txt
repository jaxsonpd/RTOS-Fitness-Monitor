set(SRC_DIR ../target/src)
set(COMMS_DIR ../target/src/comms)
set(HAL_DIR ../target/src/hal)
set(UTILITY_DIR ../target/src/utility)
set(TIVAWARE_DIR ../target/libs/tivaware)
set(MOCK_DIR mocks)
set(RTOS_DIR_INCLUDE ../target/libs/freertos/include)
set(RTOS_ARM ../target/libs/freertos/portable/GCC/ARM_CM4F)
set(RTOS_CONFIG_DIR ../target)
set(OLED_DIR ../target/libs)

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(FitnessTrackerTests LANGUAGES C)

enable_testing()
add_subdirectory(unity)

add_unity_test(test_circular_buffer test_circular_buffer.c ${UTILITY_DIR}/circular_buffer.c)
add_unity_test(test_adc test_adc.c ${SRC_DIR}/adc_manager.c)
add_unity_test(test_accl_manager test_accl_manager.c ${SRC_DIR}/accl_manager.c)
add_unity_test(test_input_hal test_input_hal.c ${HAL_DIR}/input_hal_tiva.c)
add_unity_test(test_input_manager test_input_manager.c ${SRC_DIR}/input_manager.c)
add_unity_test(test_input_comms test_input_comms.c ${COMMS_DIR}/input_comms.c)
add_unity_test(test_step_counter test_step_counter.c ${SRC_DIR}/step_counter_analog.c)
add_unity_test(test_step_counter_manager test_step_counter_manager.c ${SRC_DIR}/step_counter_manager.c)
add_unity_test(test_step_counter_comms test_step_counter_comms.c ${COMMS_DIR}/step_counter_comms.c)
add_unity_test(test_pot_comms test_pot_comms.c ${COMMS_DIR}/pot_comms.c)
add_unity_test(test_pot_manager test_pot_manager.c ${SRC_DIR}/pot_manager.c)
add_unity_test(test_adc_hal test_adc_hal.c ${HAL_DIR}/adc_hal_tiva.c)
add_unity_test(test_accl_hal test_accl_hal.c ${HAL_DIR}/accl_hal_tiva.c)
add_unity_test(test_display_hal test_display_hal.c ${HAL_DIR}/display_hal_tiva.c ${TIVAWARE_DIR}/utils/ustdlib.c)
add_unity_test(test_device_info test_device_info.c ${SRC_DIR}/device_info.c)
add_unity_test(test_clock_hal test_clock_hal.c ${HAL_DIR}/clock_hal_tiva.c)
add_unity_test(test_i2c_hal test_i2c_hal.c ${HAL_DIR}/i2c_hal_tiva.c)

target_include_directories(test_circular_buffer PRIVATE ${SRC_DIR} ${UNITY_DIR})

target_include_directories(test_input_hal PRIVATE ${SRC_DIR} ${TIVAWARE_DIR} ${MOCK_DIR} ${HAL_DIR})
target_compile_definitions(test_input_hal PRIVATE UNIT_TESTING)

target_include_directories(test_adc PRIVATE ${SRC_DIR} ${RTOS_ARM} ${RTOS_DIR_INCLUDE} ${MOCK_DIR} ${RTOS_CONFIG_DIR})
target_include_directories(test_accl_manager PRIVATE ${SRC_DIR} ${TIVAWARE_DIR} ${MOCK_DIR})
target_compile_definitions(test_accl_manager PRIVATE PART_TM4C123GH6PM)

target_include_directories(test_adc_hal PRIVATE ${SRC_DIR} ${TIVAWARE_DIR} ${MOCK_DIR} ${HAL_DIR})
target_compile_definitions(test_adc_hal PRIVATE PART_TM4C123GH6PM)

target_include_directories(test_accl_hal PRIVATE ${SRC_DIR} ${TIVAWARE_DIR} ${MOCK_DIR} ${HAL_DIR})
target_compile_definitions(test_accl_hal PRIVATE PART_TM4C123GH6PM)

target_include_directories(test_input_manager PRIVATE ${SRC_DIR} ${MOCK_DIR} ${RTOS_ARM} ${RTOS_DIR_INCLUDE} ${RTOS_CONFIG_DIR})
target_compile_definitions(test_input_manager PRIVATE UNIT_TESTING)

target_include_directories(test_clock_hal PRIVATE ${SRC_DIR} ${TIVAWARE_DIR} ${MOCK_DIR} ${HAL_DIR})

target_include_directories(test_input_comms PRIVATE ${SRC_DIR} ${RTOS_ARM} ${RTOS_DIR_INCLUDE} ${MOCK_DIR} ${RTOS_CONFIG_DIR} ${COMMS_DIR})

target_include_directories(test_step_counter PRIVATE ${SRC_DIR} ${TIVAWARE_DIR} ${MOCK_DIR})

target_include_directories(test_step_counter_manager PRIVATE ${SRC_DIR} ${MOCK_DIR} ${RTOS_ARM} ${RTOS_DIR_INCLUDE} ${RTOS_CONFIG_DIR})
target_compile_definitions(test_step_counter_manager PRIVATE UNIT_TESTING)

target_include_directories(test_pot_manager PRIVATE ${SRC_DIR} ${MOCK_DIR} ${RTOS_ARM} ${RTOS_DIR_INCLUDE} ${RTOS_CONFIG_DIR})
target_compile_definitions(test_pot_manager PRIVATE UNIT_TESTING)

target_include_directories(test_step_counter_comms PRIVATE ${SRC_DIR} ${RTOS_ARM} ${RTOS_DIR_INCLUDE} ${MOCK_DIR} ${RTOS_CONFIG_DIR} ${COMMS_DIR})

target_include_directories(test_pot_comms PRIVATE ${SRC_DIR} ${RTOS_ARM} ${RTOS_DIR_INCLUDE} ${MOCK_DIR} ${RTOS_CONFIG_DIR} ${COMMS_DIR})

target_include_directories(test_display_hal PRIVATE ${SRC_DIR} ${RTOS_ARM} ${RTOS_CONFIG_DIR} ${RTOS_DIR_INCLUDE} ${MOCK_DIR} ${OLED_DIR} ${TIVAWARE_DIR} ${HAL_DIR})

target_include_directories(test_device_info PRIVATE ${SRC_DIR} ${RTOS_ARM} ${RTOS_CONFIG_DIR} ${RTOS_DIR_INCLUDE} ${MOCK_DIR})

target_include_directories(test_i2c_hal PRIVATE ${SRC_DIR} ${TIVAWARE_DIR} ${MOCK_DIR} ${HAL_DIR})
target_compile_definitions(test_i2c_hal PRIVATE PART_TM4C123GH6PM)