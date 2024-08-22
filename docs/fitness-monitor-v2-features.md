# Fitness Monitor v2 Features

## Step counting
The new algorithm implements an algorithm from [Analog Devices](https://www.analog.com/en/resources/analog-dialogue/articles/pedometer-design-3-axis-digital-acceler.html) in [step_counter_analog](../target/src/step_counter_analog.c). The premise of this algorithm is similar to the existing algorithm, but it implements an adaptive threshold and more precise step definition. This algorithm is executed at `50 [Hz]`. It first determines the axis `x/y/z` with the largest change in acceleration between samples. If this value is below the noise threshold it is ignored. If a sample is above the threshold, it is saved for future comparison. The algorithm detects a step when there is a negative change in acceleration at the point the magnitude drops below the dynamic threshold. This dynamic threshold is updated at `1 [Hz]` based on if any acceleration is detected outside the current threshold. The algorithm also implements a simple sttae machine to classify steps as a part of a real sequence or not. This is achieved by requiring three steps in a row before the count is pushed to the global counter value.

## Energy estimation

## Inactivity warning

## User parameters

## Stopwatch