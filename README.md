# Stroke Detection Module
Stroke Detection Module for Arduino Uno.

## Materials used
* Arduino Uno
* Arduino Shield
* MPU-6050 accelerometer & gyroscope
* 5V battery
* A few M-M & M-F wires & breadboard
* Electrical wires
* Sound buzzer
* LED socket kit
* Arduino buzzer

## Functionality
Detects stroke attacks by gauging fall acceleration and angle of fall to determine whether someone has fallen and needs to send a distress signal. The distress signal consists of a loud, obnoxious noise followed by an SOS signal to an outside help (like 911), using Bluetooth.

## Changelog
* 2/1/2020
  * Implemented accelerometer and gyroscope functionality using MPU-6050.
  * Programmed sound buzzer with alternating note tone.
  * Simplified the circuit using shorter electrical wires.
  * Modified the circuit using Arduino Shield to bring together the main board and the sound source
  * Added LED output on the side for an augmented visual functionality.

## Known bugs
1. The Bluetooth functionality is yet to be implemented.

## Images
