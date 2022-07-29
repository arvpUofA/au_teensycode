#ifndef	SUBSCRIBER_HPP
#define	SUBSCRIBER_HPP

#define TORPEDO_ARM_ACTUATOR_VALUE -1
#define TORPEDO_DISARM_ACTUATOR_VALUE -2
#define TORPEDO_FIRE_ACTUATOR_VALUE 1

#include <uavcan/uavcan.hpp>
#include <uavcan/equipment/actuator/ArrayCommand.hpp>
#include <uavcan/equipment/indication/LightsCommand.hpp>
#include <uavcan/equipment/power/BatteryInfo.hpp>
#include <uavcan/equipment/air_data/StaticPressure.hpp>
#include <uavcan/protocol/NodeStatus.hpp>

#include <Arduino.h>
#include <libARVPpwm.h>
#include "servoControl.hpp"

using namespace uavcan;

//Subscriber object declerations
Subscriber<equipment::actuator::ArrayCommand> *actuatorSubscriber;

/*Callback function for uavcan actuator command array. Checks all array elements for IDs and executes torpedo or servo functions accordingly.*/
void actuatorControlCallback(const uavcan::equipment::actuator::ArrayCommand& actuatorCommands)
{
	Serial.println("Acutator command received");
	for (int i = 0; i < actuatorCommands.commands.size(); ++i) {
		if (actuatorCommands.commands[i].actuator_id == 2)
			actuateServo(0, actuatorCommands.commands[i].command_value);
	}
}

void nodeCallback(const uavcan::protocol::NodeStatus& cmd) {
	Serial.println("ioniowdnadw");
}

Subscriber<protocol::NodeStatus> *statusSubscriber;

void initSubscriber(Node<NodeMemoryPoolSize> *node)
{
	// create subscriber
	actuatorSubscriber = new Subscriber<equipment::actuator::ArrayCommand>(*node);
	statusSubscriber = new Subscriber<protocol::NodeStatus>(*node);

	// start subscriber
	if (actuatorSubscriber->start(actuatorControlCallback) < 0)
		Serial.println("Unable to start actuator subscriber");

	if (statusSubscriber->start(nodeCallback) < 0)
		Serial.println("Heartbeat bad");
}

#endif