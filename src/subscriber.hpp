#ifndef	SUBSCRIBER_HPP
#define	SUBSCRIBER_HPP

#include <UAVCAN.hpp>
#include <uavcan/equipment/actuator/ArrayCommand.hpp>
#include <uavcan/equipment/indication/LightsCommand.hpp>
#include <uavcan/equipment/power/BatteryInfo.hpp>

#include <Arduino.h>
#include <torpedoControl.hpp>
#include <libARVPpwm.h>
#include <actuatorID.h>
#include <lightID.h>
#include <batteryStatus.hpp>
#include <servoControl.hpp>
#include <ledIndicationControl.hpp>

using namespace uavcan;

//Subscriber object declerations
Subscriber<equipment::actuator::ArrayCommand> *actuatorSubscriber;
Subscriber<equipment::indication::LightsCommand> *lightsSubscriber;
Subscriber<equipment::power::BatteryInfo> *batterySubscriber;



bool enableExternalLEDActions = true; //Boolean used for allowing external control of LED strip 

//Functions to enable/disable external LED control
bool disableExternalLEDControl()
{
  if(!enableExternalLEDActions)
  {
    return false;
  }
  enableExternalLEDActions = false;
  return true;
}

bool enableExternalLEDControl()
{
  if(enableExternalLEDActions)
  {
    return false;
  }
  enableExternalLEDActions = true;
  return true;
}



/*Callback function for uavcan actuator command array. Checks all array elements for IDs and executes torpedo or servo functions accordingly.*/
void actuatorControlCallback(const uavcan::equipment::actuator::ArrayCommand& actuatorCommands)
{
  //Serial.println("Received actuator command array");
  for(uint8_t i = 0; i < actuatorCommands.commands.size(); i++)
  {
    //Serial.println(i);
    switch(actuatorCommands.commands[i].actuator_id)
    {
      case ACTUATOR_ID_TORPEDO_0:
        //Torpedo fires when command value equals 1
        if(actuatorCommands.commands[i].command_value == 1)requestLaunch(TORPEDO_0); 
        //Torpedo is armed when command value equals -1
        else if(actuatorCommands.commands[i].command_value == -1)armTorpedo(TORPEDO_0);
        //Serial.println("case 1 complete");
        break;

      case ACTUATOR_ID_TORPEDO_1:
        if(actuatorCommands.commands[i].command_value == 1)requestLaunch(TORPEDO_1);
        else if(actuatorCommands.commands[i].command_value == -1)armTorpedo(TORPEDO_1);
        break;

      case ACTUATOR_ID_SERVO_0:
        //command value must be between 0 to 180 degrees or 0 to 3.14159 radians
        actuateServo(PWM_CHANNEL_SERVO_0, actuatorCommands.commands[i].command_value);
        break;

      case ACTUATOR_ID_SERVO_1:
        actuateServo(PWM_CHANNEL_SERVO_1, actuatorCommands.commands[i].command_value);
        break;

      case ACTUATOR_ID_SERVO_2:
        actuateServo(PWM_CHANNEL_SERVO_2, actuatorCommands.commands[i].command_value);
        break;

      case ACTUATOR_ID_SERVO_3:
        actuateServo(PWM_CHANNEL_SERVO_3, actuatorCommands.commands[i].command_value);
        break;

      case ACTUATOR_ID_SERVO_4:
        actuateServo(PWM_CHANNEL_SERVO_4, actuatorCommands.commands[i].command_value);
        break;

      case ACTUATOR_ID_SERVO_5:
        actuateServo(PWM_CHANNEL_SERVO_5, actuatorCommands.commands[i].command_value);
        break;

      case ACTUATOR_ID_SERVO_6:
        actuateServo(PWM_CHANNEL_SERVO_6, actuatorCommands.commands[i].command_value);
        break;

      case ACTUATOR_ID_SERVO_7:
        actuateServo(PWM_CHANNEL_SERVO_7, actuatorCommands.commands[i].command_value);
        break;

      case ACTUATOR_ID_SERVO_8:
        actuateServo(PWM_CHANNEL_SERVO_8, actuatorCommands.commands[i].command_value);
        break;

      case ACTUATOR_ID_SERVO_9:
        actuateServo(PWM_CHANNEL_SERVO_9, actuatorCommands.commands[i].command_value);
        break;

      case ACTUATOR_ID_SERVO_10:
        actuateServo(PWM_CHANNEL_SERVO_10, actuatorCommands.commands[i].command_value);
        break;

      case ACTUATOR_ID_SERVO_11:
        actuateServo(PWM_CHANNEL_SERVO_11, actuatorCommands.commands[i].command_value);
        break;
    }
    //Serial.println("switch done");
  }
  //Serial.println("for complete");
}

/*Callback function for uavcan led command array. Checks all array elements for IDs and executes LED functions accordingly.*/
void lightsControlCallback(const uavcan::equipment::indication::LightsCommand& lightCommand)
{
  //Serial.println("Received lights command array");
  if(enableExternalLEDActions)
  {
    for(uint8_t i = 0; i < lightCommand.commands.size(); i++)
    {
      switch(lightCommand.commands[i].light_id)
      {
        case LIGHT_ID_RGB_STRIP_0:
          setLEDColour(lightCommand.commands[i].color.red, lightCommand.commands[i].color.green, lightCommand.commands[i].color.blue);
          break;
      }
    }
  }
  else
  {
    Serial.println("LED external control disabled");
  }
}

/*Callback function for uavcan battery info messages. Stores values into storage array */
void batteryInfoCallback(const uavcan::equipment::power::BatteryInfo& batteryData)
{
  //Serial.println("Received lights command array");
  storeVoltageInfo(batteryData.battery_id, batteryData.voltage);
}

void initSubscriber(Node<NodeMemoryPoolSize> *node)
{
  // create subscriber
  actuatorSubscriber = new Subscriber<equipment::actuator::ArrayCommand>(*node);
  lightsSubscriber = new Subscriber<equipment::indication::LightsCommand>(*node);
  batterySubscriber = new Subscriber<equipment::power::BatteryInfo>(*node);

  // start subscriber  
  if(actuatorSubscriber->start(actuatorControlCallback) < 0)
  {
    Serial.println("Unable to start actuator subscriber");
  }
  if(lightsSubscriber->start(lightsControlCallback) < 0)
  {
    Serial.println("Unable to start lights subscriber");
  }
  if(batterySubscriber->start(batteryInfoCallback) < 0)
  {
    Serial.println("Unable to start battery subscriber");
  }
}

#endif