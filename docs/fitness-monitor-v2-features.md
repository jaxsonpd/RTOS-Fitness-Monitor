# Fitness Monitor v2 Features

## Step counting

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
