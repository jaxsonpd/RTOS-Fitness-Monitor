# RTOS Implementation

This file covers the implementation of the real time operating system in the
firmware. A high level overview can be found in the following table.

| Task Name | Priority | Task Size | Internal Delay (ms) |
| - | - | - | - |
| Input Manager | 3 | 64 | 10 |
| Step Counter | 4 | 128 | 5 |
| Device Manager | 2 | 512 | 50 |
| Potentiometer Manager | 3 | 64 | 20 |

The philosophy for this is step counting is the highest priority
while display manager is least important. The stack sizes are
selected based on the complexity of the threads the display
manager having the most depth to function calls etc. has the
largest stack size. The step counter contains an complex algorithm
hence the need for a larger stack size than expected. 

## Task Breakdown

### Input Manager Task

The input manager task handles the debouncing of all GPIO and then
sends this information to the input queue. The input manager is
purely a producer it doesn't receive information from any other
tasks.

This task is located in [input_manager.c](../target/src/input_manager.c).

### Potentiometer Manager Task

The potentiometer task handles the adc polling for the potentiometer
on the orbit board. This task does not affect the value simply saves
it to the mutex locked variable. This task does not receive information
from any other tasks.

This task is located in [pot_manager.c](../target/src/pot_manager.c).

### Device/Display Manager Task

The device manager tasks handles the storage of device information and displaying the various screens that make up the GUI of the device. This is the largest task in the firmware and contains the state machine that controls all of the devices features. This task get data from the three other tasks through their respective communications modules.

This task is located in [device_manager.c](../target/src/device_manager.c)

### Step Counter Manager Task

The step counter task handles the counting of steps using a custom algorithm. This thread saves the step count value to the mutex locked communication module.

This task is located in [step_counter_manager.c](../target/src/step_counter_manager.c)

## Inter Task Communication

The tasks communicate through a mixture of queues and mutexes to
allow for the asynchronous nature of the firmware. Each of these
methods is located in its own module with a single getter and
setter to ensure that deadlock doesn't occur.

### Input Queue

The input messages which include button presses and switch inputs
are added to a queue located in the [./comms/input_comms.c](../target/src/comms/input_comms.c)
module. This queue is then picked up by the device manager.  A
queue is used over a mutex to ensure polling isn't nessasary and all
inputs are preserved and handled.

### Potentiometer Mutex 

The potentiometer value is written to a mutex locked variable located in [./comms/pot_comms.c](../target/src/comms/input_comms.c).
This module uses a mutex to ensure that the value isn't read halfway
through writing.

### Step Counter Mutex

The potentiometer value is written to a mutex locked variable located in [./comms/step_counter_comms.c](../target/src/comms/step_counter_comms.c).
This module uses a mutex to ensure that the value isn't read halfway
through writing.
