#ifndef SERVO_CONTROL_HPP
#define SERVO_CONTROL_HPP

#include <libARVPpwm.h>
#include <actuatorID.h>

#include <parameter.hpp>

//Servo limits in us
#define SERVOMIN  600
#define SERVOMAX  2600

Adafruit_PWMServoDriver *pwmServo;

//Initialization function for assigning a PCA9685 interface object to be used for actuating servos
void initServoControl(Adafruit_PWMServoDriver *ptr)
{
    pwmServo = ptr;
}

//Provides simplified servo control interface for UAVCAN callbacks
void actuateServo(uint8_t pwmChannel, float angle)
{
    pwmServo->setServoAngle(pwmChannel, angle, (uint16_t)boardConfig[PARAM_INDEX_MIN_SERVO_PULSE].paramValue, (uint16_t)boardConfig[PARAM_INDEX_MAX_SERVO_PULSE].paramValue, UNIT_RADIANS);
}

#endif