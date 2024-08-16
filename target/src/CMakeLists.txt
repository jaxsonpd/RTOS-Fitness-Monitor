add_tiva_executable(fitness-tracker
    accl_hal_tiva.c
    accl_manager.c
    adc_hal_tiva.c
    adc_manager.c
    input_manager.c
    circular_buffer_T.c
    display_manager.c
    i2c_driver.c
    serial_sender.c
    fitness_monitor_main.c
    input_hal_tiva.c
    clock_hal_tiva.c
    input_comms.c
    step_counter_analog.c
    step_counter_manager.c
    step_counter_comms.c
    display_hal_tiva.c
    display_info.c
    pot_comms.c
    pot_manager.c
    vector3.c
    stopwatch.c
)
target_compile_options(fitness-tracker PRIVATE -Wall)
target_link_libraries(fitness-tracker PRIVATE tivaware OrbitOLED freertos::heap-4 m)
add_program_target(fitness-tracker PROGRAM_TARGET program DEBUG_TARGET debug)
