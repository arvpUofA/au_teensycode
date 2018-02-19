#ifndef	SUBSCRIBER_HPP
#define	SUBSCRIBER_HPP

#include <UAVCAN.hpp>
// #include <uavcan/protocol/debug/LogMessage.hpp>
// #include <uavcan/protocol/debug/KeyValue.hpp>
#include <uavcan/equipment/actuator/ArrayCommand.hpp>

#include <torpedoControl.hpp>
#include <libARVPpwm.h>

//8 bit actuator ID definitions
#define ACTUATOR_ID_TORPEDO_0 0
#define ACTUATOR_ID_TORPEDO_1 1

#define ACTUATOR_ID_SERVO_0 2
#define ACTUATOR_ID_SERVO_1 3
#define ACTUATOR_ID_SERVO_2 4
#define ACTUATOR_ID_SERVO_3 5
#define ACTUATOR_ID_SERVO_4 6
#define ACTUATOR_ID_SERVO_5 7
#define ACTUATOR_ID_SERVO_6 8
#define ACTUATOR_ID_SERVO_7 9
#define ACTUATOR_ID_SERVO_8 10
#define ACTUATOR_ID_SERVO_9 11
#define ACTUATOR_ID_SERVO_10 12
#define ACTUATOR_ID_SERVO_11 13

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

/* Subscriber<protocol::debug::LogMessage> *logSubscriber;
Subscriber<protocol::debug::KeyValue> *keySubscriber; */

Subscriber<equipment::actuator::ArrayCommand> *actuatorSubscriber;

//PCA9685 object, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(PCA9685_BASEADDR);

/* void logMessageCallback(const uavcan::protocol::debug::LogMessage& msg)
{
  Serial.println("Received log message");
}

void keyMessageCallback(const uavcan::protocol::debug::KeyValue& msg)
{
  Serial.println("Received key message!");
  Serial.print("Value:");
  Serial.println(msg.value);
} */

/*Callback function for uavcan actuator command array. */
void actuatorMessageCallback(const uavcan::equipment::actuator::ArrayCommand& actuatorCommands)
{
  Serial.println("Received actuator command array");
  for(uint8_t i = 0; i < sizeof(actuatorCommands.commands)/sizeof(uavcan::equipment::actuator::Command); i++)
  {
    switch(actuatorCommands.commands[i].actuator_id)
    {
      case ACTUATOR_ID_TORPEDO_0:
        if(actuatorCommands.commands[i].command_value == 1)requestLaunch(TORPEDO_0);
        else if(actuatorCommands.commands[i].command_value == -1)resetTorpedoStatus(TORPEDO_0);
        break;

      case ACTUATOR_ID_TORPEDO_1:
        if(actuatorCommands.commands[i].command_value == 1)requestLaunch(TORPEDO_1);
        else if(actuatorCommands.commands[i].command_value == -1)resetTorpedoStatus(TORPEDO_1);
        break;

      case ACTUATOR_ID_SERVO_0:
        pwm.setServoAngle(PWM_CHANNEL_SERVO_0, actuatorCommands.commands[i].command_value, SERVOMIN, SERVOMAX, UNIT_RADIANS);
        break;

      case ACTUATOR_ID_SERVO_1:
        pwm.setServoAngle(PWM_CHANNEL_SERVO_1, actuatorCommands.commands[i].command_value, SERVOMIN, SERVOMAX, UNIT_RADIANS);
        break;

      case ACTUATOR_ID_SERVO_2:
        pwm.setServoAngle(PWM_CHANNEL_SERVO_2, actuatorCommands.commands[i].command_value, SERVOMIN, SERVOMAX, UNIT_RADIANS);
        break;

      case ACTUATOR_ID_SERVO_3:
        pwm.setServoAngle(PWM_CHANNEL_SERVO_3, actuatorCommands.commands[i].command_value, SERVOMIN, SERVOMAX, UNIT_RADIANS);
        break;

      case ACTUATOR_ID_SERVO_4:
        pwm.setServoAngle(PWM_CHANNEL_SERVO_4, actuatorCommands.commands[i].command_value, SERVOMIN, SERVOMAX, UNIT_RADIANS);
        break;

      case ACTUATOR_ID_SERVO_5:
        pwm.setServoAngle(PWM_CHANNEL_SERVO_5, actuatorCommands.commands[i].command_value, SERVOMIN, SERVOMAX, UNIT_RADIANS);
        break;

      case ACTUATOR_ID_SERVO_6:
        pwm.setServoAngle(PWM_CHANNEL_SERVO_6, actuatorCommands.commands[i].command_value, SERVOMIN, SERVOMAX, UNIT_RADIANS);
        break;

      case ACTUATOR_ID_SERVO_7:
        pwm.setServoAngle(PWM_CHANNEL_SERVO_7, actuatorCommands.commands[i].command_value, SERVOMIN, SERVOMAX, UNIT_RADIANS);
        break;

      case ACTUATOR_ID_SERVO_8:
        pwm.setServoAngle(PWM_CHANNEL_SERVO_8, actuatorCommands.commands[i].command_value, SERVOMIN, SERVOMAX, UNIT_RADIANS);
        break;

      case ACTUATOR_ID_SERVO_9:
        pwm.setServoAngle(PWM_CHANNEL_SERVO_9, actuatorCommands.commands[i].command_value, SERVOMIN, SERVOMAX, UNIT_RADIANS);
        break;

      case ACTUATOR_ID_SERVO_10:
        pwm.setServoAngle(PWM_CHANNEL_SERVO_10, actuatorCommands.commands[i].command_value, SERVOMIN, SERVOMAX, UNIT_RADIANS);
        break;

      case ACTUATOR_ID_SERVO_11:
        pwm.setServoAngle(PWM_CHANNEL_SERVO_11, actuatorCommands.commands[i].command_value, SERVOMIN, SERVOMAX, UNIT_RADIANS);
        break;

    }
  }
}

void initSubscriber(Node<NodeMemoryPoolSize> *node)
{
  // create a subscriber
  // logSubscriber = new Subscriber<protocol::debug::LogMessage>(*node);
  // keySubscriber = new Subscriber<protocol::debug::KeyValue>(*node);
  actuatorSubscriber = new Subscriber<equipment::actuator::ArrayCommand>(*node);

  // start subscriber
/*   if(logSubscriber->start(logMessageCallback) < 0)
  {
    Serial.println("Unable to start log message subscriber!");
  }

  if(keySubscriber->start(keyMessageCallback) < 0)
  {
    Serial.println("Unable to start log message subscriber!");
  } */

  if(actuatorSubscriber->start(actuatorMessageCallback) < 0)
  {
    Serial.println("Unable to start actuator message subscriber");
  }
}

#endif