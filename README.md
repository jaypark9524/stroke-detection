# Shock Detection Module
Shock Detection Module for Arduino Uno.

## Materials used
* Arduino Uno
* Arduino shield
* MPU-6050 accelerometer & gyroscope
* 5V battery
* A few M-M & M-F wires & breadboard
* Sound buzzer
* LED socket kit
* Arduino buzzer

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

## Possible extensions
1. Bluetooth connectivity to interface with a phone or a similar nearby device, which could then be utilized to make phone calls to helpful sources like 911.
