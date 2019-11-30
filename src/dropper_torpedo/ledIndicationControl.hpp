#ifndef LED_INDICATION_CONTROL_HPP
#define LED_INDICATION_CONTROL_HPP

#include <libARVPpwm.h>
#include <Metro.h>

#define MAX_RED_INPUT 31
#define MAX_GREEN_INPUT 31
#define MAX_BLUE_INPUT 31

bool strobeActivated = false;

Adafruit_PWMServoDriver *pwmLED;
Metro *strobeTimer;

//Initialization function for assigning a PCA9685 interface object to be used for driving RGB LED strip
void initLEDControl(Adafruit_PWMServoDriver *ptr, Metro *ptr2)
{
	pwmLED = ptr;
	strobeTimer = ptr2;
	pwmLED->setRGB(0,0,0,0,0);
}

//Provides simplified LED control interface for UAVCAN callbacks
void setLEDColour(float red, float green, float blue)
{
	pwmLED->setRGB(red/MAX_RED_INPUT, green/MAX_GREEN_INPUT, blue/MAX_BLUE_INPUT, 0, 1);
}

void activateStrobe(float red, float green, float blue)
{
	strobeTimer->reset();
	pwmLED->setRGB(red/MAX_RED_INPUT, green/MAX_GREEN_INPUT, blue/MAX_BLUE_INPUT, 0, 1);
	strobeActivated = true;
}

void pulseLED(float red, float green, float blue, double interval)
{
	strobeTimer->reset();
	strobeTimer->interval(interval);
	pwmLED->setRGB(red/MAX_RED_INPUT, green/MAX_GREEN_INPUT, blue/MAX_BLUE_INPUT, 0, 1);
	strobeActivated = true;
}

#endif