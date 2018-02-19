#ifndef	SUBSCRIBER_HPP
#define	SUBSCRIBER_HPP

#include <UAVCAN.hpp>
#include <uavcan/equipment/actuator/ArrayCommand.hpp>
#include <uavcan/equipment/indication/LightsCommand.hpp>

#include <Arduino.h>
#include <torpedoControl.hpp>
#include <libARVPpwm.h>
#include <actuatorID.h>
#include <lightID.h>



//PWM channel definitions for PCA9685 chip
#define PWM_CHANNEL_SERVO_0 0
#define PWM_CHANNEL_SERVO_1 1
#define PWM_CHANNEL_SERVO_2 2
#define PWM_CHANNEL_SERVO_3 3
#define PWM_CHANNEL_SERVO_4 4
#define PWM_CHANNEL_SERVO_5 5
#define PWM_CHANNEL_SERVO_6 6
#define PWM_CHANNEL_SERVO_7 7
#define PWM_CHANNEL_SERVO_8 8
#define PWM_CHANNEL_SERVO_9 9
#define PWM_CHANNEL_SERVO_10 10
#define PWM_CHANNEL_SERVO_11 11



using namespace uavcan;

//Subscriber object declerations
Subscriber<equipment::actuator::ArrayCommand> *actuatorSubscriber;
Subscriber<equipment::indication::LightsCommand> *lightsSubscriber;

Adafruit_PWMServoDriver *pwm;

bool enableExternalLEDActions = true; //Boolean used for allowing external control of LED strip 

//Functions to enable/disable external LED control
void disableExternalLEDControl()
{
  enableExternalLEDActions = false;
}

void enableExternalLEDControl()
{
  enableExternalLEDActions = true;
}

//Initialization function for assigning a PCA9685 interface object to be used by subscriber callbacks
void getPWMObject(Adafruit_PWMServoDriver *ptr)
{
  pwm = ptr;
}

/*Callback function for uavcan actuator command array. Checks all array elements for IDs and executes torpedo or servo functions accordingly.*/
void actuatorMessageCallback(const uavcan::equipment::actuator::ArrayCommand& actuatorCommands)
{
  Serial.println("Received actuator command array");
  for(uint8_t i = 0; i < sizeof(actuatorCommands.commands)/sizeof(uavcan::equipment::actuator::Command); i++)
  {
    switch(actuatorCommands.commands[i].actuator_id)
    {
      case ACTUATOR_ID_TORPEDO_0:
        //Torpedo fires when command value equals 1
        if(actuatorCommands.commands[i].command_value == 1)requestLaunch(TORPEDO_0); 
        //Torpedo is unlocked when command value equals -1
        else if(actuatorCommands.commands[i].command_value == -1)resetTorpedoStatus(TORPEDO_0);
        break;

      case ACTUATOR_ID_TORPEDO_1:
        if(actuatorCommands.commands[i].command_value == 1)requestLaunch(TORPEDO_1);
        else if(actuatorCommands.commands[i].command_value == -1)resetTorpedoStatus(TORPEDO_1);
        break;

      case ACTUATOR_ID_SERVO_0:
        //command value must be between 0 to 180 degrees or 0 to 3.14159 radians
        pwm->setServoAngle(PWM_CHANNEL_SERVO_0, actuatorCommands.commands[i].command_value, SERVOMIN, SERVOMAX, UNIT_RADIANS);
        break;

      case ACTUATOR_ID_SERVO_1:
        pwm->setServoAngle(PWM_CHANNEL_SERVO_1, actuatorCommands.commands[i].command_value, SERVOMIN, SERVOMAX, UNIT_RADIANS);
        break;

      case ACTUATOR_ID_SERVO_2:
        pwm->setServoAngle(PWM_CHANNEL_SERVO_2, actuatorCommands.commands[i].command_value, SERVOMIN, SERVOMAX, UNIT_RADIANS);
        break;

      case ACTUATOR_ID_SERVO_3:
        pwm->setServoAngle(PWM_CHANNEL_SERVO_3, actuatorCommands.commands[i].command_value, SERVOMIN, SERVOMAX, UNIT_RADIANS);
        break;

      case ACTUATOR_ID_SERVO_4:
        pwm->setServoAngle(PWM_CHANNEL_SERVO_4, actuatorCommands.commands[i].command_value, SERVOMIN, SERVOMAX, UNIT_RADIANS);
        break;

      case ACTUATOR_ID_SERVO_5:
        pwm->setServoAngle(PWM_CHANNEL_SERVO_5, actuatorCommands.commands[i].command_value, SERVOMIN, SERVOMAX, UNIT_RADIANS);
        break;

      case ACTUATOR_ID_SERVO_6:
        pwm->setServoAngle(PWM_CHANNEL_SERVO_6, actuatorCommands.commands[i].command_value, SERVOMIN, SERVOMAX, UNIT_RADIANS);
        break;

      case ACTUATOR_ID_SERVO_7:
        pwm->setServoAngle(PWM_CHANNEL_SERVO_7, actuatorCommands.commands[i].command_value, SERVOMIN, SERVOMAX, UNIT_RADIANS);
        break;

      case ACTUATOR_ID_SERVO_8:
        pwm->setServoAngle(PWM_CHANNEL_SERVO_8, actuatorCommands.commands[i].command_value, SERVOMIN, SERVOMAX, UNIT_RADIANS);
        break;

      case ACTUATOR_ID_SERVO_9:
        pwm->setServoAngle(PWM_CHANNEL_SERVO_9, actuatorCommands.commands[i].command_value, SERVOMIN, SERVOMAX, UNIT_RADIANS);
        break;

      case ACTUATOR_ID_SERVO_10:
        pwm->setServoAngle(PWM_CHANNEL_SERVO_10, actuatorCommands.commands[i].command_value, SERVOMIN, SERVOMAX, UNIT_RADIANS);
        break;

      case ACTUATOR_ID_SERVO_11:
        pwm->setServoAngle(PWM_CHANNEL_SERVO_11, actuatorCommands.commands[i].command_value, SERVOMIN, SERVOMAX, UNIT_RADIANS);
        break;

    }
  }
}

void lightsMessageCallback(const uavcan::equipment::indication::LightsCommand& lightCommand)
{
  Serial.println("Received lights command array");
  if(enableExternalLEDActions)
  {
    for(uint8_t i = 0; i < sizeof(lightCommand.commands)/sizeof(uavcan::equipment::indication::SingleLightCommand); i++)
    {
      switch(lightCommand.commands[i].light_id)
      {
        case LIGHT_ID_RGB_STRIP_0:
          pwm->setRGB(lightCommand.commands[i].color.red/31, lightCommand.commands[i].color.green/63, lightCommand.commands[i].color.blue/31, 0, 1);
          break;
      }
    }
  }
  else
  {
    Serial.println("LED external control disabled");
  }
}

void initSubscriber(Node<NodeMemoryPoolSize> *node)
{
  // create subscriber
  actuatorSubscriber = new Subscriber<equipment::actuator::ArrayCommand>(*node);

  // start subscriber  
  if(actuatorSubscriber->start(actuatorMessageCallback) < 0)
  {
    Serial.println("Unable to start actuator message subscriber");
  }
}

#endif