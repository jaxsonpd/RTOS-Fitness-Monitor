# Fitness Monitor v2 Features

## Step counting
The new fitness monitor implements an algorithm from [Analog Devices](https://www.analog.com/en/resources/analog-dialogue/articles/pedometer-design-3-axis-digital-acceler.html) in [step_counter_analog.c](../target/src/step_counter_analog.c). The premise of this algorithm is similar to the existing algorithm, but it implements an adaptive threshold and more precise step definition. The algorithm is executed at `50 [Hz]`. It first determines the axis with the largest change in acceleration between samples. If this value is below the noise threshold it is ignored. If a sample is above the threshold, it is saved for future comparison. The algorithm detects a step when there is a negative change in acceleration at the point the magnitude drops below the dynamic threshold. This dynamic threshold is updated at `1 [Hz]` based on if any acceleration is detected outside the current threshold. The algorithm also implements a simple sttae machine to classify steps as a part of a real sequence or not. This is achieved by requiring three steps in a row before the count saved.

## Energy estimation

The fitness monitor v2 allows for calorie tracking based on the distance travelled (and
thus the number of steps). This energy value is calculated using the
following equation:

$$
c = \frac{125 \times distance \times weight}{100000} [cal]
$$

The calorie calculation depends directly on the user's weight, and indirectly on the user's height via their stride length for distance estimation. These are set by the user on the [user_parameters_state.c](../target/src/display_states/user_parameters_state.c) display. The energy burned estimation is displayed along with the steps, distance, and speed on the [distance_state.c](../target/src/device_states/distance_state.c) screen.

## User parameters

As mentioned in the Energy Estimation section there is a requirement
to get the users height and weight for calorie estimation. This
functionality is provided through the [user_parameters_state.c](../target/src/display_states/user_parameters_state.c) display, which allows the
input of both height and weight using the potentiometer.

## Inactivity warning

An inactivity warning prompts the user to move after no steps have
been taken in a set amount of time. This delay can be changed
by the user in the [move_prompt_state.c](../target/src/device_states/move_prompt_state.c) 
display. This allows for values from ten seconds to one hour. This timeout is then checked
on every execution of the display task loop to trigger the [move_prompt_alert_state.c](../target/src/device_states/move_prompt_alert_state.c) display.

## Stopwatch

A significant feature that has been implemented is a custom stopwatch in 
[stopwatch_state.c](../target/src/display_states/stopwatch_state.c).
display. This allows the user to time events independently of the main workout 
timer and contains the history of the last two runs. The stopwatch uses its 
own internal state machine to allow for background timing. The stopwatch state 
machine is summarised in the following table.

| State | Action | Leave Event |
| - | - | - |
| Zeroed | Move current value to history one and history one value to history two. | Down button pressed |
| Running | Display current elapsed time | Down button pressed  |
| Stopped | Hold elapsed value on display | Down button pressed |