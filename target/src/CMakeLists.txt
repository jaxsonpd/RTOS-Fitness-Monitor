add_tiva_executable(fitness-tracker
    accl_hal_tiva.c
    accl_manager.c
    adc_hal_tiva.c
    ADC_read.c
    input_manager.c
    circular_buffer_T.c
    display_manager.c
    i2c_driver.c
    serial_sender.c
    fitness_monitor_main.c
    IO_input.c
    input_comms.c
    step_counter.c
    step_counter_comms.c
)
target_compile_options(fitness-tracker PRIVATE -Wall)
target_link_libraries(fitness-tracker PRIVATE tivaware OrbitOLED freertos::heap-4 m)
add_program_target(fitness-tracker PROGRAM_TARGET program DEBUG_TARGET debug)
