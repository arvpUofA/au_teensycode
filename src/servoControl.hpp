#ifndef SERVO_CONTROL_HPP
#define SERVO_CONTROL_HPP

#include <libARVPpwm.h>
#include <actuatorID.h>

//Servo limits in us
#define SERVOMIN  1000
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
    pwmServo->setServoAngle(pwmChannel, angle, SERVOMIN, SERVOMAX, UNIT_RADIANS);
}

#endif