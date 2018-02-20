#ifndef LED_INDICATION_CONTROL_HPP
#define LED_INDICATION_CONTROL_HPP

#include <lightID.h>
#include <libARVPpwm.h>

#define MAX_RED_INPUT 31
#define MAX_GREEN_INPUT 63
#define MAX_BLUE_INPUT 31

Adafruit_PWMServoDriver *pwmLED;

//Initialization function for assigning a PCA9685 interface object to be used for driving RGB LED strip
void initLEDControl(Adafruit_PWMServoDriver *ptr)
{
    pwmLED = ptr;
}

//Provides simplified LED control interface for UAVCAN callbacks
void setLEDColour(float red, float green, float blue)
{
    pwmLED->setRGB(red/MAX_RED_INPUT, green/MAX_GREEN_INPUT, blue/MAX_BLUE_INPUT, 0, 1);
}

#endif