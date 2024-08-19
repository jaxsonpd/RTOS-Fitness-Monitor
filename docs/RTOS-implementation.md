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

### Potentiometer Manager Task

The potentiometer task handles the adc polling for the potentiometer
on the orbit board. This task does not affect the value simply saves
it to the mutex locked variable. This task does not receive information
from any other tasks.

### Device/Display Manager Task



### Step Counter Manager Task


## Inter Task Communication

### Input Queue


### Potentiometer Mutex 


### Step Counter Mutex
