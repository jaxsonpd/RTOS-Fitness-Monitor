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

- Display state machine

- Hals

- Solid principles