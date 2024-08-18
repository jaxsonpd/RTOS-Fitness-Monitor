# System Level Acceptance Tests

<style>
table th:first-of-type {
    width: 2%;
}
table th:nth-of-type(2) {
    width: 10%;
}
table th:nth-of-type(3) {
    width: 10%;
}
table th:nth-of-type(4) {
    width: 10%;
}
table th:nth-of-type(5) {
    width: 65%;
}
table th:nth-of-type(6) {
    width: 3%;
}
</style>

This file contains the system level acceptance tests to manually test that
the functionality of the device follows the correct specification.

## Fitness Manager v1 Functionality Tests

| # | Name | Screen | Input |  Expected Result | Pass Fail |
| - | - | - | - | - | - |
| 1 | Startup Screen | Step Count | - | On step counting screen with timer stopped and steps at 0 | |
| 2 | Default Values | Step Goal | - | Step goal 1000 | |
| 3 | Screen Switching | All | Left Button, Right Button | Pushing left button rotates screens in full circle, right button acts in opposite direction with same functionality | |
| 4 | Units Toggle | All | Up Button | Pushing the up button will change the units from SI to imperial and raw step count to percentage of goal. | |
| 5 | Set Goal | Step Goal | Down Button |Pushing down button (short) will set the selected goal as the new goal | |
| 6 | Reset | Most (!Step Goal) | Long Down Button |A long push on the down button will reset all values back to default (except if on step goal setting screen) this should be signalled to the user | |
| 7 | Select Goal | Step Goal | Pot Rotate | Rotating the poteniometer anti clockwise should increase the new goal amount allowing the user to set a new on in steps of 100. | |
| 8 | Debug Mode | All | SW1 Up | When the SW1 is up device is in debug mode up button increments step count by 100 steps and down button decrements by 100 steps all other up/down button behaviour disabled. | |
| 9 | Goal Reached | All | - | When the step goal is reached this should be displayed by a flashed message. This message will not appear again until reset or new goal set. | |
| 10 | Distance Display | Distance | - | The distance screen should display the users distance, time, speed, and energy used. | |
| 11 | Workout Start | All | - | The workout timer should not start until the first step is registered. | |
| 12 | Step Count | All | - | Moving the device in a cyclical motion causes the step counter to increase. | |
| 13 | Average Speed | All | - | Average speed updates with distance and time travelled | |

## Fitness Manager v2 Functionality Tests

| # | Name | Screen | Input |  Expected Result | Pass Fail |
| - | - | - | - | - | - |
| 1 | Stopwatch Start | Stopwatch | Down Button | On short down button press stopwatch timer starts and increments while away from screen. | |
| 2 | Stopwatch Stop | Stopwatch | Down Button | On short down button press stopwatch timer stops the current timer and continuos to display value. | | 
| 3 | Stopwatch Reset | Stopwatch | Down Button | On short down button press stopwatch timer resets counter to 0 and saves the value to the stop-1 row. Moving the stop-1 value to stop-2. | |
| 4 | Change Weight | Parameter Selection | Pot Rotate, SW2 Up | When SW2 is up the potentiometer value should control the weight selection value in increments of 1. | |
| 5 | Set Weight | Parameter Selection | SW2 Up, Down Button | When SW2 is up and the down button is pressed the currently selected new weight should be saved. | |
| 6 | Change Height | Parameter Selection | SW2 Down, Pot Rotate | Rotating the potentiometer while SW2 is down should change the new height value in increments of 1. | |
| 7 | Set Height | Parameter Selection | SW2 Down, Down Button | Pressing the down button wile SW2 is down should save the new height value. | |
| 8 | Change Inactivity Delay | Inactivity Delay Select | Pot Rotate | Rotating the potentiometer value should change the new inactivity timer value in increments of 10. | |
| 9 | Set Inactivity Delay | Inactivity Delay Select | Down Button | Pressing the down button should save the new inactivity delay. | |
| 10 | Inactivity prompt | All | - | When no new steps have been registered or longer than the inactivity delay a message should be displayed to prompt the user to move this should not stop displaying until a new stop is registered. | |
| 11 | Caloric Tracking | Distance, Parameter Selection | - | The caloric expenditure count increases on step count. This value changes based on the weight selected | |
