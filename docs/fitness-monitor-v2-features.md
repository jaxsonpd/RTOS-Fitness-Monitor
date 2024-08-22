# Fitness Monitor v2 Features

## Step counting
The new algorithm implements an algorithm from [Analog Devices](https://www.analog.com/en/resources/analog-dialogue/articles/pedometer-design-3-axis-digital-acceler.html) in [step_counter_analog](../target/src/step_counter_analog.c). The premise of this algorithm is similar to the existing algorithm, but it implements an adaptive threshold and more precise step definition. This algorithm is executed at `50 [Hz]`. It first determines the axis `x/y/z` with the largest change in acceleration between samples. If this value is below the noise threshold it is ignored. If a sample is above the threshold, it is saved for future comparison. The algorithm detects a step when there is a negative change in acceleration at the point the magnitude drops below the dynamic threshold. This dynamic threshold is updated at `1 [Hz]` based on if any acceleration is detected outside the current threshold. The algorithm also implements a simple sttae machine to classify steps as a part of a real sequence or not. This is achieved by requiring three steps in a row before the count is pushed to the global counter value.

## Energy estimation

The fitness monitor v2 allows for cailory tracking based on the distance (and
thus the number of steps) travelled. This energy value is calculated using the
following equation:

$$
c = \frac{125 d w}{100000} [cal]
$$
Where $d$ is the distance travelled and $w$ is the weight of the person.
This calculation depends on the users height (stride length) and weight.
Which are input in the user parameters display. This calories values
is then displayed on the distance screen.


## Inactivity warning

An inactivity warning prompts the user to move after no steps have
been taken in a set amount of time. This delay can be changed
by the user in the timeout selection display screen. This allows for
values form 10 seconds to one hour. This timeout is then checked
on every execution of the display task loop.

## User parameters

As mentioned in the Energy Estimation section there is a requirement
to get the users height and weight for calory estermation. This
functionality is proved through a new display screen which allows the
input of both height and weight through the potentiometer. The height value
is also used for more accurate step to distance conversion.

## Stopwatch

The most significant display screen change is the addtion of a stopwatch
display. This screen allows for the timing of events and contains the
history of the last two runs. These values are reset on device reset 
and the stopwatch moves to the zeroed state.
The stopwatch uses its own internal state machine to allow for background timing.
The state machine which proceeds consecutively can be seen in the following table.

| State | Action | Leave Event |
| - | - | - |
| Zeroed | Move current value to history one and history one value to history two. | Left button down |
| Running | Display current elapsed time | Left button down  |
| Stopped | Hold elapsed value on display | Left button down |
