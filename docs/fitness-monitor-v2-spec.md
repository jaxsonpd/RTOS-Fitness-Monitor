# Fitness Monitor v2.0 User Interface Specification

_Note: The Fitness Monitor v2.0 is an adaption of the Fitness Monitor v1.0 which was originally developed as an ENCE361 project. This is an edited and restructured version of the "User Interface Specification" that was provided to the class._

1. The program should respond reliably and in real time to button pushes, swithces and a scroll wheel.

   - The scroll wheel input is provided by the potentiometer on the Orbit board, which produces an analogue signal on pin AIN0.

2. Pushing the RESET button on the TIVA board should restart the program and return any stored step, distance, timer and goal values to their default values.

   2.1. The default step count, speed, energy, distance and stopwatch time values are zero. The default goal value is 1,000 steps. The default prompt to move value is 100 seconds 

3. At startup the OLED board should display the number of steps counted since the last reset or power cycle.

4. LEFT button behaviour:

   4.1. Pushing the LEFT button should take the user from the Step Count Sisplay to a Set Inactivity Timer display, where the time value for the inactivity alert can be set. 

   4.2. Pushing the LEFT button again should take the user to a Set Weight and Height display, where the users Height and Weight can be set. 

   4.3. Pushing the LEFT button again should take the user to a Set Goal state, where the target number of steps can be set.

   4.4. Pushing the LEFT button should take the user to a Stopwatch display, where the current stopwatch time is shown. Below the elapsed time should display the last two logged times from the user. 

   4.5. Pushing the LEFT button again should take the user to the Distance Display, where they are presented with the Distance, Speed and Energy burnt since the first step. 

   4.6. Pushing the LEFT button again should display the number of steps counted since the last reset or power cycle.

5. RIGHT button behaviour. Pushing the RIGHT button should achieve the same change in state as pushing the LEFT button, but in the opposite direction.

6. Pushing the UP button on the TIVA board should toggle the units used by the fitness monitor.

   6.1. For the total number of steps, the units are either the raw number of steps or the number of steps as a percentage of the step goal.

   6.2. For the total distance travelled, the units are either the total distance in kilometres (km) or the total distance in miles (mi).    

   6.3. For the Speed, the units are either kilometers per hour (kph) or miles per hour (mph)

   6.4. For the Energy burnt, the units are either kilojoules (kJ) or Calorise (cal)

   6.5. For the Weight the units are either kilograms (kg) or pounds (lb)

   6.6. For the Height the units are either centimeters (cm) or feet (ft) 

   6.7. In all the above cases, numbers shown on the OLED display should be accompanied by units.

7. Briefly pushing the DOWN button on the TIVA board should set the value selected in each set display.

    7.1. Pressing the DOWN button in the Set Goal Display should set the new step goal to the number displayed.

    7.2. Pressing the DOWN button in the Set Parameters display should set the respective unit. When the SW2 (LEFT switch) is set to DOWN, the user height should be set to the number displayed. When the SW2 is set to UP, the user weight should be set to the number shown.

    7.3. Pressing the DOWN button in the Set Inactivity Time display should set the prompt to move time value to the number on the display. The time since last step should return to zero. 

    7.4. The Parameters display should remain on the same display when the height or weight is set. All other occurances of setting values should take the user back to the Step Count Display. 

    7.5. Pressing the DOWN button in the Stopwatch display should activate the stopwatch

    * When the DOWN button is pressed, the timer should begin. Pressing the DOWN button again will result in the timer being stopped. Pressing the DOWN button again will log the timer value from the Elapsed row down to the number 1 slot. The Elapsed row will be reset to zero.

    * After recording a new time, pressing the DOWN button again will bring the row 1 time to the number 2 row. The elapsed time will be moved to row 1. The elapsed time will again be set to zero. When a new time is logged again the row 2 time will be purged. 

8. When not in the Set Goal display a long press on the DOWN button should reset the number of steps counted and distance travelled to zero. All variables should be set to their default values. The user should be brought back to the Step Count display

9. The UI should provide feedback to the user that acknowledges and diﬀerentiates between long and short pushes on the DOWN button.

10. Setting the step goal, height, weight and inactivity time value:

    10.1. On the Set step goal, set parameter and set inactivity time displays, rotating the potentiometer on the Orbit board clockwise should increase the number displayed. Rotating the potentiometer on the Orbit board anti-clockwise should decrease the displayed value.

    10.2. The Goal value should increment in units of 100 steps. The height value should increment in units of 1 centimeter. The weight should increment in 0.1 kilograms. The inactivity time value should increment in values of 10 seconds. 

11. Setting SW1 (RIGHT switch) to the UP position should put the fitness monitor in a test mode, where the functionality of the GUI can be verified. In the test mode:

    11.1. Each push of the UP button should increment the step count by 100.

    11.2. Each push of the DOWN button should decrement the step count by 500.

    11.3. Other functions of the UP and DOWN buttons, namely toggling units, setting values and resetting counts, should be disabled while SW1 is UP.

    11.4. The functionality of the LEFT and RIGHT buttons should not be aﬀected by SW1.

    11.5. Setting SW1 to DOWN should restore the normal functionality of the UP and DOWN buttons. The values changed in the test mode should continue over to the normal functionality. 

12. Flash screens:

    12.1. When the Step Goal value is surpassed or equal to the step count value the fitness monitor should display the goal reached flash screen for 3 seconds. 

    12.2. When the time since the last step exceeds the inactivity time value, the fitness monitor should prompt the user to move, and exit the screen when it detects a step. 


