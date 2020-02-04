# Shock Detection Module
Shock Detection Module for Arduino Uno.

## Demo Video
https://www.youtube.com/watch?v=MOOXCn93uBI&feature=youtu.be

## Materials used
* Arduino Uno
* Arduino shield
* MPU-6050 accelerometer & gyroscope
* 5V battery
* A few M-M & M-F wires & breadboard
* Sound buzzer
* LED socket kit

## Functionality
Detects sudden changes in movement caused by shock. It gauges fall acceleration and angle of fall to determine whether someone has fallen and needs to send a distress signal after a short delay. The distress signal consists of a loud, obnoxious alert noise, alongside a blue LED and an alternating red LED. 

## Changelog
* February 1st, 2020
  * Implemented accelerometer and gyroscope functionality using MPU-6050.
  * Programmed sound buzzer with alternating note tone.
  * Simplified the circuit using shorter electrical wires.
  * Modified the circuit using Arduino shield to bring together the main board and the sound source
  * Added LED output on the side for an augmented visual functionality.

## Known bugs

## Images
![1](images/1.jpg)

![2](images/2.jpg)

![3](images/3.jpg)

![4](images/4.jpg)

![5](images/5.jpg)

![6](images/6.png)

## Possible extensions
1. Bluetooth connectivity to interface with a phone or a similar nearby device, which could then be utilized to make phone calls to helpful sources like 911.
