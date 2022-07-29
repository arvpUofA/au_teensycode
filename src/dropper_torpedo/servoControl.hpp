#ifndef SERVO_CONTROL_HPP
#define SERVO_CONTROL_HPP

#include <math.h>
#include <libARVPpwm.h>

#include "parameter.h"

//Servo limits in us
#define SERVOMIN	600
#define SERVOMAX	2600

#define SERVO_ANGLE_OFFSET (M_PI / 2)

Adafruit_PWMServoDriver *pwmServo;

/* Servo angle relative to initial position [radians] */
double servoAngle = 0;

void updateAngle(uint8_t pwm_chan, float angle)
{
	if ((servoAngle + angle) > (2 * M_PI))
		servoAngle = (servoAngle + angle) - (2 * M_PI);
	else
		servoAngle = servoAngle + angle;
}

void resetServo(uint8_t pwm_chan)
{
	actuateServo(pwm_chan, -1 * (float)servoAngle);
}

double getAngle(uint8_t pwm_chan)
{
	return servoAngle;
}

//Provides simplified servo control interface for UAVCAN callbacks
void actuateServo(uint8_t pwm_chan, float angle)
{
	pwmServo->setServoAngle(pwm_chan, angle + SERVO_ANGLE_OFFSET,
		900,
		2000,
		UNIT_RADIANS);
	updateAngle(pwm_chan, angle);
}

//Initialization function for assigning a PCA9685 interface object to be used for actuating servos
void initServoControl(Adafruit_PWMServoDriver *ptr)
{
	pwmServo = ptr;
	for (uint8_t i = 0; i < PWM_CHANNELS; i++) {
		//ptr->setServoAngle(i, 0, (uint16_t)boardConfig[MIN_SERVO_PULSE].paramValue, (uint16_t)boardConfig[MAX_SERVO_PULSE].paramValue, UNIT_RADIANS);
		actuateServo(i, 0);
	}
}

#endif