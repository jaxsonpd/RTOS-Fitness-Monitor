# Demo Presentation Text

Good morning we are Group 01. We hae redesigned the fitness tracker application
to be more robust and feature rich. We have ported the firmware over to FreeRTOS,
implemented 5 new features, including an inactivity timer, revamped custom step counting,
energy tracking, stopwatch and height based distance estimation. While completing
this work we implemented test driven design, solid Principle and software design patterns to ensure that
the code is maintainable going forward.

I will discuss the system level acceptance tests and showcase the UI, Jack
will discuss test driven development and the free rtos implementation, and Isaac
will discuss the solid principles and the design patterns in the code.

## System Level Acceptance Testing - Daniel

- Take through system acceptance testing.
- Show case new display features.

## Test driven development and free RTOS- Jack

We used TTD for input_comms, adc_hal, input_hal, i2C_hal (most comms and hal files).
We did not do TTD for the high level functions as the implementation was complex
and new to us making it difficult to test from the beginning. For these modules
we added tests retroactively.

Over all we have --- tests mostly unit due to many modules being very self contained
but several like display have extra files included in testing where it makes sense.

We did not test several files due to there complexity and visual components this includes
the display states, display manager, and the main file. We believe that this is a
sufficient compromise to ensure development time is not overly-long and development code is
not affected by tests.

- Show tests running

We have four RTOS threads seen here. The input handling all gpio which communicates
with the display over a message queue, the potentiometer which handles adc polling
and communicates with display using a mutex, the step counter which handles
the acceleration measurement and communicates with the display using a mutex and finally
the display manager which uses a finite state machine to display the various UI elements.

## SOLID Principle and Design Pattern implementation - Isaac

CUT THIS DOWN into bullets

Throughout our development we have applied the software principles presented in lectures. These have enabled us to work efficiently with an existing codebase, and make meaningful changes to improve it.

The existing first version showed many of the common "smells" of bad code. Its structure was rigid and fragile, with extending functionality being almost impossible without major architecture changes. To resolve this, we decoupled and encapsulated different behaviours. This makes it easier to make changes, but meant a lot of time was spent refactoring.

We have applied the SOLID principles whenever we refactored existing code or designed new code. The behaviour in each thread follows a consistent heirarchy. At the highest level, the thread is defined in a "manager" file. This file then talks to lower level files that define behaviour. These behaviours then talk to hardware abstraction layers, which then interface with the hardware. This consistent abstraction makes it easy to test at different levels, and also isolates single responsibilites.

All interaction with hardware is done via HALs, which would theoretically let the fitness monitor run on different hardware by only rewriting hal.c files. These are examples of implementing the adaptor design pattern.

The most significant refactor is in the implementation of the state handling. This is achieved by using the state design pattern. In this, the thread interacts with the device state. This holds the current state. Each state implements an interface with a function to enter, execute, and exit the state. These keep the specific implementation abstracted from the thread. New device states can then be implemented simply by creating a new source file for that state.

In this implementation, the specific states write to the OLED display through the display hal. This keeps them abstract from the OrbitOLED, but not necessarily from the "4 lines, 16 characters" style. This is the key area where further abstraction could be implemented in a version 3. Due to the timeframe of this project this interface could not be implemented. Despite this, the current architecture allowed for very simple implementations of new states.

- Throughout development we have applied principles from lectures, including SOLID and design patterns.
- Smells of bad code in the first version. Rigid and fragile - hard to develop new features.
- One of the key ideas when refactoring was making abstraction levels consistent across threads.
- Consistent: Manager -> Behaviour -> HAL -> Tivaware -> Hardware
- This helps to give single responsibilities to modules
- Refactored overall using SOLID
- The use of HALs on all hardware applies the substitution principle, and creates dependency inversion
- These HALs were implemented following the adapter design pattern, and in theory means only they would change to run on new hardware.
- The most significant refactor was to the device state handling.
- Here we applied the state design pattern. This allows for an open-closed design.
- The thread handles state transitions, and talks to a device state instance. This holds the current state.
- Each state is an implementation of the base state type, which has functions for entry, execution, exiting, and resetting.
- The manager does not know about any specifics. This is means that adding a new state is as simple as creating the source file for it and registering it to the device.
- Due to time constraints, each state writes to the display through the display HAL
- Hardware independence, but still conforms to "4 lines, 16 chars"
- In future, we would separate the device and display functions into separate threads.
- This would be better from a single responsibility standpoint, and make the project more hardware agnostic.
- Then, the device thread would use a message queue to send the display data to the display thread.