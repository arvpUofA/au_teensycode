#ifndef	PARAMETER_HPP
#define	PARAMETER_HPP

#include <uavcan/uavcan.hpp>
#include <uavcan/protocol/param_server.hpp>
#include <teensy_uavcan.hpp>
#include <EEPROM.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define LEN(a) (sizeof(a) / sizeof(*a))

#define MAX_NUMBER_OF_PARAMETERS 64 //based on sizeof(Param) (32 bytes) and 2048 byte EEPROM capacity in Teensy 3.2

enum param_index {
	TORPEDO_PULSE,
	MIN_SERVO_PULSE,
	MAX_SERVO_PULSE,
	ACT_ID_TORP_0,
	ACT_ID_TORP_1,
	ACT_ID_SERVO_0,
	ACT_ID_SERVO_1,
	ACT_ID_SERVO_2,
	ACT_ID_SERVO_3,
	ACT_ID_SERVO_4,
	ACT_ID_SERVO_5,
	ACT_ID_SERVO_6,
	ACT_ID_SERVO_7,
	ACT_ID_SERVO_8,
	ACT_ID_SERVO_9,
	ACT_ID_SERVO_10,
	ACT_ID_SERVO_11,
	LIGHT_ID_STRIP_0,
	LIGHT_ID_STRIP_0_STROBE,
	STROBE_INTERVAL,
	DEMO_MODE,
	ENABLE_LOW_VOLT_INDICATOR,
	LOW_PRESSURE_THRESHOLD,
	ENABLE_PRESSURE_ALERT,
	ENABLE_TORPEDO_INDICATOR,
	SERVO_START_ANGLE
};

using namespace uavcan;

struct Param {
	char name[24];
	double value;
} defaultBoardConfig[] = {
	[TORPEDO_PULSE]   = {"torpedoPulseInterval", 250}, // in ms
	[MIN_SERVO_PULSE] = {"minServoPulse", 900}, // in us
	[MAX_SERVO_PULSE] = {"maxServoPulse", 2000}, // in us
	[ACT_ID_TORP_0]   = {"Torpedo0_ID", 0},
	[ACT_ID_TORP_1]   = {"Torpedo1_ID", 1},

	[ACT_ID_SERVO_0]  = {"Servo0_ID", 2},
	[ACT_ID_SERVO_1]  = {"Servo1_ID", 3},
	[ACT_ID_SERVO_2]  = {"Servo2_ID", 4},
	[ACT_ID_SERVO_3]  = {"Servo3_ID", 5},
	[ACT_ID_SERVO_4]  = {"Servo4_ID", 6},
	[ACT_ID_SERVO_5]  = {"Servo5_ID", 7},
	[ACT_ID_SERVO_6]  = {"Servo6_ID", 8},
	[ACT_ID_SERVO_7]  = {"Servo7_ID", 9},
	[ACT_ID_SERVO_8]  = {"Servo8_ID", 10},
	[ACT_ID_SERVO_9]  = {"Servo9_ID", 11},
	[ACT_ID_SERVO_10] = {"Servo10_ID", 12},
	[ACT_ID_SERVO_11] = {"Servo11_ID", 13},

	[LIGHT_ID_STRIP_0]        = {"RGBStrip0_ID", 0},
	[LIGHT_ID_STRIP_0_STROBE] = {"RGBStrip0_strobe", 248},
	[STROBE_INTERVAL]         = {"strobeInterval", 500}, // in ms

	[DEMO_MODE] = {"demoMode", 0},

	[ENABLE_LOW_VOLT_INDICATOR] = {"enableLowVoltIndicator", 1}, //changed to 0 since it's not working right
	[LOW_PRESSURE_THRESHOLD]    = {"lowPressureThreshold", 20000}, // in Pascals
	[ENABLE_PRESSURE_ALERT]     = {"enablePressureAlert", 0},
	[ENABLE_TORPEDO_INDICATOR]  = {"enableTorpedoIndicator", 1},

	[SERVO_START_ANGLE] = {"servoStartAngle", -1.5} //in radians
};

struct Param boardConfig[MAX_NUMBER_OF_PARAMETERS];

//Store default parameter names and values into boardConfig struct array.
void initParameters()
{
	for (uint8_t i = 0; i < LEN(defaultBoardConfig) && i < MAX_NUMBER_OF_PARAMETERS; i++) {
		strcpy(boardConfig[i].name, defaultBoardConfig[i].name);
		boardConfig[i].value = defaultBoardConfig[i].value;
	}
}

void printParameters()
{
	Serial.printf("\n--Printing Paramaters--\n");
	for (uint8_t i = 0; i < LEN(defaultBoardConfig); i++)
		Serial.printf("%s: %d\n", boardConfig[i].name, boardConfig[i].value);
	Serial.printf("\n----\n");
}

void saveConfig()
{
	Serial.println("Saving parameters to EEPROM");
	EEPROM.put(0, boardConfig);
}

void resetConfig()
{
	Serial.println("Resetting all params to default values.");
	initParameters();
	EEPROM.put(0, boardConfig);
}

/*
 * Now, we need to define some glue logic between the server (below) and our configuration storage (above).
 * This is done via the interface uavcan::IParamManager.
 */
class : public uavcan::IParamManager
{
	void getParamNameByIndex(Index index, Name& out_name) const override
	{
		if (index < LEN(defaultBoardConfig))
			out_name = boardConfig[index].name;
	}

	void assignParamValue(const Name& name, const Value& value) override
	{
		for (uint8_t i = 0; i < LEN(defaultBoardConfig); i++) {
			if (!strcmp(boardConfig[i].name, name.c_str())) {
				boardConfig[i].value = *value.as<uavcan::protocol::param::Value::Tag::real_value>();
				return;
			}
		}
		Serial.println("Can't assign parameter!");
	}

	void readParamValue(const Name& name, Value& out_value) const override
	{
		for (uint8_t i = 0; i < LEN(defaultBoardConfig); i++) {
			if (boardConfig[i].name == name) {
				out_value.to<uavcan::protocol::param::Value::Tag::real_value>() = boardConfig[i].value;
				return;
			}
		}
		Serial.println("Can't read parameter!");
	}

	int saveAllParams() override
	{
		saveConfig();
		return 0;
	}

	int eraseAllParams() override
	{
		resetConfig();
		return 0;
	}

} param_manager;

uavcan::ParamServer *server;

void initParameter(Node<NodeMemoryPoolSize> *node)
{
	initParameters(); //Prepare boardConfig array
	//saveConfig(); //Run this here if saving new parameters for first time.
	EEPROM.get(0, boardConfig); //Load saved array
	printParameters();
	server = new uavcan::ParamServer(*node);
	if (server->start(&param_manager) < 0)
		Serial.println("Failed to start ParamServer!");
	else
		Serial.println("Started Parameter server successfully!");
}

#endif