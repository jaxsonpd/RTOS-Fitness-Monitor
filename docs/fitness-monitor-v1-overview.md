# Fitness Monitor v1 Overview

This file documents the structure and implementation of the fitness monitor v1.
To allow for a comparison with the new implementation for version 2.

## Structure

The version 1 firmware has a modularized structure with several layers of
abstraction. An overview of the modules can be seen in Figure 1. The key
take aways from this diagram are several modules with similar names and
the circular dependency created by the use of deviceStateInfo_t.

![Fitness Monitor v1 source structure](./images/version1-structurev2.png)

Figure 1: Fitness Monitor v1 source structure.

### Modules

A more in depth description of the version 1's modules and there functions can be found below:

- `button_manager` - Updates the device state based on user inputs to buttons and switches.
- `display_manager` - Updates the devices screen based on the current state of the device.
- `adc_read` - Perform reads to the adc to measure the potentiometer.
- `accl_manger` - Handle communications to the accelerometer including initialisation, signal processing and reads.
- `circBufV/T` - Provides functionality for a int32_t and vector3_t circular buffer respectivly.
- `serial_sender` - Provides functionality to write to the serial port.
- `button_4` - Handles polling and debouncing of the 4 push buttons on the Tiva board.
- `switches` - Handles reading the Tiva orbit boards switches.
- `I2C_driver` - Provides the ability to send I2C messages to the accelerometer (or other I2C devices).
- `step_counter_main` - Handles scheduling for all the tasks as well as state updates and step counting.

## Architecture

The fitness monitor version 1 uses a timer based scheduler, with each task getting run at its defined time. There is no checking of task overrun and tasks cannot run concurrently. The ticks are incremented by the systick interrupt which runs at a rate of `10000 [Hz]`. The frequencies for each task can be seen in Table 1.

Table 1: Frequency for each tasks execution in fitness monitor version 1 project.

| Task | Frequency [Hz] |
| - | - |
| GPIO and adc | 75 |
| Accelerometer | 200 |
| Display | 5 |
| Serial | 100 |

## Step Counting

The step counting algorithm used by fitness monitor version 1 is detailed
as follows. The accelerometer is polled at a rate of `200 [Hz]`. At every excution the magnitude ($m$) of the three accelerometer directions is found:

$$
m = \sqrt{a^2_x+a^2_y+a^2_z}
$$

If this value is greater than $270~[m/s^2]$ and the user has not stepped then a step is recorded. Once the magnitude falls below $235~[m/s^2]$ the step flag is reset.

## Evaluation

### Module Evaluation

The version 1 firmware does make use of modules improving its modifiability and
readability; However the modules are convoluted with several performing similar
functions (circBuf T vs V and button4 vs switches). There also exists a circular
dependency with deviceState_t being provided by the main header file and the
other modules depending on this function. This is not necessary and just causes
increased global variable scope to functions that don't need it.

The modules could be improved by reducing duplicate functionality, improving
naming and reducing global variable scope. These changes would improve the
firmwares modifiability and reduce the chance of un-noticed faults or bugs.

### Architecture Evaluation

The version 1's architecture allows tasks to run at different rates this
means that slow low priority tasks can not interfere with the high speed
calculation required to track the step counting. On top of this there is
overflow checking of the tick counters which ensures the code is robust.

The main issue with this timer based scheduler is that it does not check for
task execution time overrun. This means that high priority tasks can miss their
deadlines due to a lower priority task taking longer than its frequency to
run. Secondly much of the tasks execution is spread out across both the main file
and the module. This means that it is difficult to split tasks into smaller
pieces if this is required to ensure deadlines are met.
