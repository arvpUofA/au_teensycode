dropper_torpedo teensy board
============================
to make use of these functions include an
	#include "parameter.h"

PWM Channel Definitions
-----------------------
	PWM_CHAN_0     /* dropper servo */
	PWM_CHAN_1     /* light */
	/* remaining channels are currently unused */

Available fucntions
-------------------
	/* actuates servo attached to pwm_chan a given angle in radians */
	actuateServo(uint8_t pwm_chan, float radians);

	/* resets servo attached to pwm_chan to its inital position */
	resetServo(uint8_t pwm_chan);

	/* returns the current of servo attached to pwm_chan in radains */
	getAngle(uint8_t pwm_chan);

# Notes

## Torpedoes

Torpedoes are controlled through the UAVCAN actuator array command.
- Array IDs are 0 or 1 by default
- Send a command value of -1 to arm
- Send a command value of 1 to fire
- Send a command value of -2 to disarm
- Torpedos are disarmed upon startup and after firing.

## Servos

Servos are controlled through the UAVCAN actuator array command.
- Array IDs range between 2 to 13 by default
- Send a command value between -pi/2 to pi/2 to set servo angle
- Servos are set to 0 rad upon startup

## LED Strip

RGB strip is controlled through the UAVCAN light command array.
- Light ID is 0 for continuous control, 248 for pulse control
- Send light command with RGB values between 0 and 31 (integers)

### LED Patterns

The firmware can override incoming UAVCAN light commands to display critical alerts or complex patterns.

#### List of Patterns in Order of Priority:
- Dropping pressure alert: bright fast in phase sine pattern of all three colours.
Activated when pressure drops below desired threshold set in UAVCAN parameter lowPressureThreshold.
- Torpedoes firing: solid bright red
- Torpedoes armed: slow red sine pattern with 180° phase shifted blue and/or green patterns depending on which torpedoes are armed
- Very poor battery voltage level, > 12.75 V: fast red sine pattern
- Poor battery voltage, > 13.25 V: slow red sine pattern
- Demo mode: 120° phase shifted patterns on all 3 colours

## Parameters

Parameters are configurable through UAVCAN node parameter server.

### List of Parameters and Default Values:
- torpedoPulseInterval = 250[ms]
- minServoPulse = 900[us]
- maxServoPulse = 2000[us]
- Torpedo0_ID = 0
- Torpedo1_ID = 1
- Servo0_ID = 2
- Servo1_ID = 3
- Servo2_ID = 4
- Servo3_ID = 5
- Servo4_ID = 6
- Servo5_ID = 7
- Servo6_ID = 8
- Servo7_ID = 9
- Servo8_ID = 10
- Servo9_ID = 11
- Servo10_ID = 12
- Servo11_ID = 13
- RGBStrip0_ID = 0
- RGBStrip0_strobe = 248
- strobeInterval = 500[ms]
- demoMode = 0
- enableLowVoltIndicator = 1
- lowPressureThreshold = 20000[Pa]
- enablePressureAlert = 0
- enableTorpedoIndicator = 1
- servoStartAngle = -1.5[rad]

Parameters are saved to Teensy EEPROM when save command is sent.
Parameters are reset to default values when erase command is sent.
Reset and save parameters to default when falshing a new Teensy or when changing defaultParameterArray to properly initialize EEPROM.

## Serial Command Line Interface (CLI)

Some board actions can be controlled through a serial CLI

### Command list (command [arguments]):
- arm [0 or 1]: arms torpedo 0 or 1
- disarm [0 or 1]: disarms torpedo 0 or 1
- fire [0 or 1]: fires torpedo 0 or 1
- setdemo [0 or 1]: controls RGB LED pattern for demo/display
- printparam: displays UAVCAN parameter names and values in serial monitor
- restoreparam: resets all UAVCAN parameters to default values
- saveparam: saves all UAVCAN parameters to EEPROM
- displayvoltages: displays voltage data of all batteries on serial monitor
- enablelvindicator [0 or 1]: controls red LED low battery voltage alert pattern by setting enableLowVoltIndicator parameter value to argument
- enablepressurealert [0 or 1]: controls low hull air pressure LED alert pattern by setting enablePressureAlert parameter value to argument
- enabletorpedoindicator [0 or 1]: controls torpedo arming/firing LED pattern by setting enableTorpedoIndicator parameter value according to argument.


## Watchdog Timer

Teensy 3.2 internal watchdog timer is enabled through watchdog.h with 2000 ms timeout.
WDT reset using KickDog().