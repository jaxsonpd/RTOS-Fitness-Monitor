add_tiva_executable(fitness-tracker
    comms/input_comms.c
    comms/pot_comms.c
    comms/step_counter_comms.c
    device_state_handler.c
    device_states/steps_state.c
    device_states/distance_state.c
    device_states/goal_state.c
    device_states/stopwatch_state.c
    device_states/move_prompt_state.c
    device_states/user_parameters_state.c
    device_states/goal_reached_state.c
    device_states/move_prompt_alert_state.c
    device_states/reset_state.c
    hal/accl_hal_tiva.c
    hal/adc_hal_tiva.c
    hal/clock_hal_tiva.c
    hal/display_hal_tiva.c
    hal/i2c_hal_tiva.c
    hal/input_hal_tiva.c
    utility/circular_buffer.c
    utility/vector3.c
    utility/person.c
    accl_handler.c
    adc_handler.c
    device_info.c
    device_manager.c
    input_manager.c
    fitness_monitor_main.c
    step_counter_analog.c
    step_counter_manager.c
    pot_manager.c
    serial_sender.c
)
target_compile_options(fitness-tracker PRIVATE -Wall)
target_link_libraries(fitness-tracker PRIVATE tivaware OrbitOLED freertos::heap-4 m)
add_program_target(fitness-tracker PROGRAM_TARGET program DEBUG_TARGET debug)
