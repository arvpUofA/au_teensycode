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
- actuatorIDTorpedo0 = 0
- actuatorIDTorpedo1 = 1
- actuatorIDServo0 = 2
- actuatorIDServo1 = 3
- actuatorIDServo2 = 4
- actuatorIDServo3 = 5
- actuatorIDServo4 = 6
- actuatorIDServo5 = 7
- actuatorIDServo6 = 8
- actuatorIDServo7 = 9
- actuatorIDServo8 = 10
- actuatorIDServo9 = 11
- actuatorIDServo10 = 12
- actuatorIDServo11 = 13
- lightIDRGBStrip0 = 0
- lightIDRGBStrip0_strobe = 248
- strobeInterval = 500[ms]
- demoMode = 0
- enableLowVoltIndicator = 1
- lowPressureThreshold = 20000[Pa]
- enablePressureAlert = 0
- enableTorpedoIndicator = 1
- servoStartAngle = -1.5[rad]

Parameters are saved to Teensy EEPROM when save command is sent.
Parameters are reset to default values when erase command is sent.
Reset and save parameters to default when falshing a new Teensy or hen changing defaultParameterArray to properly initialize EEPROM.

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