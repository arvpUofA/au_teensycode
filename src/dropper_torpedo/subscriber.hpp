#ifndef	SUBSCRIBER_HPP
#define	SUBSCRIBER_HPP

#define TORPEDO_ARM_ACTUATOR_VALUE 0 
#define TORPEDO_DISARM_ACTUATOR_VALUE 1
#define TORPEDO_FIRE_ACTUATOR_VALUE 2

#include <uavcan/uavcan.hpp>
#include <uavcan/equipment/actuator/ArrayCommand.hpp>
#include <uavcan/equipment/indication/LightsCommand.hpp>
#include <uavcan/equipment/power/BatteryInfo.hpp>
#include <uavcan/equipment/air_data/StaticPressure.hpp>

#include "parameter.h"

#include <Arduino.h>
#include "torpedoControl.hpp"
#include <libARVPpwm.h>
#include "batteryStatus.hpp"
#include "servoControl.hpp"
#include "ledIndicationControl.hpp"
#include "pressureStatus.hpp"

using namespace uavcan;

//Subscriber object declerations
Subscriber<equipment::actuator::ArrayCommand> *actuatorSubscriber;
Subscriber<equipment::indication::LightsCommand> *lightsSubscriber;
Subscriber<equipment::power::BatteryInfo> *batterySubscriber;
Subscriber<equipment::air_data::StaticPressure> *pressureSubscriber;

bool enableExternalLEDActions = true; //Boolean used for allowing external control of LED strip

//Functions to enable/disable external LED control. Only changes state boolean if current state is different.
bool disableExternalLEDControl()
{
	if (!enableExternalLEDActions)
		return false;

	enableExternalLEDActions = false;
	return true;
}

bool enableExternalLEDControl()
{
	if (enableExternalLEDActions)
		return false;
	enableExternalLEDActions = true;
	return true;
}

// TODO basically every if statement here should be a look up table instead.
// All the lookup values should just be converted to integers, instead of comparing floats.

struct torpedo_action {
	void (*fp)(uint8_t);
};

struct torpedo_action torpedo_action_lut[] = {
	{.fp = requestLaunch },
	{ .fp = armTorpedo },
	{ .fp = disarmTorpedo },
};

/*Callback function for uavcan actuator command array. Checks all array elements for IDs and executes torpedo or servo functions accordingly.*/
void actuatorControlCallback(const uavcan::equipment::actuator::ArrayCommand& actuatorCommands)
{
	for (uint8_t i = 0; i < actuatorCommands.commands.size(); i++) {
	if (actuatorCommands.commands[i].actuator_id == (int)boardConfig[ACT_ID_TORP_0].value) {
		torpedo_action_lut[(uint8_t) actuatorCommands.commands[i].command_value].fp(TORPEDO_0);
	}

	if (actuatorCommands.commands[i].actuator_id == (int)boardConfig[ACT_ID_TORP_1].value) {
		torpedo_action_lut[actuatorCommands.commands[i].command_value].fp(TORPEDO_1);
	}

	// TODO lookup table here
	if (actuatorCommands.commands[i].actuator_id == (int)boardConfig[ACT_ID_SERVO_0].value)
		actuateServo(PWM_CHAN_0, actuatorCommands.commands[i].command_value);

	if (actuatorCommands.commands[i].actuator_id == (int)boardConfig[ACT_ID_SERVO_1].value)
		actuateServo(PWM_CHAN_1, actuatorCommands.commands[i].command_value);

	if (actuatorCommands.commands[i].actuator_id == (int)boardConfig[ACT_ID_SERVO_2].value)
		actuateServo(PWM_CHAN_2, actuatorCommands.commands[i].command_value);

	if (actuatorCommands.commands[i].actuator_id == (int)boardConfig[ACT_ID_SERVO_3].value)
		actuateServo(PWM_CHAN_3, actuatorCommands.commands[i].command_value);

	if (actuatorCommands.commands[i].actuator_id == (int)boardConfig[ACT_ID_SERVO_4].value)
		actuateServo(PWM_CHAN_4, actuatorCommands.commands[i].command_value);

	if (actuatorCommands.commands[i].actuator_id == (int)boardConfig[ACT_ID_SERVO_5].value)
		actuateServo(PWM_CHAN_5, actuatorCommands.commands[i].command_value);

	if (actuatorCommands.commands[i].actuator_id == (int)boardConfig[ACT_ID_SERVO_6].value)
		actuateServo(PWM_CHAN_6, actuatorCommands.commands[i].command_value);

	if (actuatorCommands.commands[i].actuator_id == (int)boardConfig[ACT_ID_SERVO_7].value)
		actuateServo(PWM_CHAN_7, actuatorCommands.commands[i].command_value);

	if (actuatorCommands.commands[i].actuator_id == (int)boardConfig[ACT_ID_SERVO_8].value)
		actuateServo(PWM_CHAN_8, actuatorCommands.commands[i].command_value);

	if (actuatorCommands.commands[i].actuator_id == (int)boardConfig[ACT_ID_SERVO_9].value)
		actuateServo(PWM_CHAN_9, actuatorCommands.commands[i].command_value);

	if (actuatorCommands.commands[i].actuator_id == (int)boardConfig[ACT_ID_SERVO_10].value)
		actuateServo(PWM_CHAN_10, actuatorCommands.commands[i].command_value);

	if (actuatorCommands.commands[i].actuator_id == (int)boardConfig[ACT_ID_SERVO_11].value)
		actuateServo(PWM_CHAN_11, actuatorCommands.commands[i].command_value);
	}
}

/*Callback function for uavcan led command array. Checks all array elements for IDs and executes LED functions accordingly.*/
void lightsControlCallback(const uavcan::equipment::indication::LightsCommand& lightCommand)
{
	if (enableExternalLEDActions) {
		for (uint8_t i = 0; i < lightCommand.commands.size(); i++) {
			if (lightCommand.commands[i].light_id == (int)boardConfig[LIGHT_ID_STRIP_0].value)
				setLEDColour(lightCommand.commands[i].color.red, lightCommand.commands[i].color.green, lightCommand.commands[i].color.blue);
			if (lightCommand.commands[i].light_id == (int)boardConfig[LIGHT_ID_STRIP_0_STROBE].value)
				pulseLED(lightCommand.commands[i].color.red, lightCommand.commands[i].color.green, lightCommand.commands[i].color.blue, boardConfig[STROBE_INTERVAL].value);
		}
	} else {
		//Serial.println("LED external control disabled");
	}
}

/*Callback function for uavcan battery info messages. Stores values into storage array */
void batteryInfoCallback(const uavcan::equipment::power::BatteryInfo& batteryData)
{
	storeVoltageInfo(batteryData.battery_id, batteryData.voltage);
}

void pressureInfoCallback(const uavcan::equipment::air_data::StaticPressure& pressureData)
{
	checkPressure((double)pressureData.static_pressure);
}

void initSubscriber(Node<NodeMemoryPoolSize> *node)
{
	// create subscriber
	actuatorSubscriber = new Subscriber<equipment::actuator::ArrayCommand>(*node);
	lightsSubscriber = new Subscriber<equipment::indication::LightsCommand>(*node);
	batterySubscriber = new Subscriber<equipment::power::BatteryInfo>(*node);
	pressureSubscriber = new Subscriber<equipment::air_data::StaticPressure>(*node);

	// start subscriber
	if (actuatorSubscriber->start(actuatorControlCallback) < 0)
		Serial.println("Unable to start actuator subscriber");
	if (lightsSubscriber->start(lightsControlCallback) < 0)
		Serial.println("Unable to start lights subscriber");
	if (batterySubscriber->start(batteryInfoCallback) < 0)
		Serial.println("Unable to start battery subscriber");
	if (pressureSubscriber->start(pressureInfoCallback) < 0)
		Serial.println("Unable to start pressure subscriber");
}

#endif