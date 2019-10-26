#ifndef	PARAMETER_HPP
#define	PARAMETER_HPP

#include <uavcan/uavcan.hpp>
#include <uavcan/protocol/param_server.hpp>
#include <teensy_uavcan.hpp>
#include <EEPROM.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define NUMBER_OF_PARAMETERS 26
#define MAX_NUMBER_OF_PARAMETERS 64 //based on sizeof(genericParam) (32 bytes) and 2048 byte EEPROM capacity in Teensy 3.2

#define PARAM_INDEX_TORPEDO_PULSE 0
#define PARAM_INDEX_MIN_SERVO_PULSE 1
#define PARAM_INDEX_MAX_SERVO_PULSE 2
#define PARAM_INDEX_ACT_ID_TORP_0 3
#define PARAM_INDEX_ACT_ID_TORP_1 4
#define PARAM_INDEX_ACT_ID_SERVO_0 5
#define PARAM_INDEX_ACT_ID_SERVO_1 6
#define PARAM_INDEX_ACT_ID_SERVO_2 7
#define PARAM_INDEX_ACT_ID_SERVO_3 8
#define PARAM_INDEX_ACT_ID_SERVO_4 9
#define PARAM_INDEX_ACT_ID_SERVO_5 10
#define PARAM_INDEX_ACT_ID_SERVO_6 11
#define PARAM_INDEX_ACT_ID_SERVO_7 12
#define PARAM_INDEX_ACT_ID_SERVO_8 13
#define PARAM_INDEX_ACT_ID_SERVO_9 14
#define PARAM_INDEX_ACT_ID_SERVO_10 15
#define PARAM_INDEX_ACT_ID_SERVO_11 16
#define PARAM_INDEX_LIGHT_ID_STRIP_0 17
#define PARAM_INDEX_LIGHT_ID_STRIP_0_STROBE 18
#define PARAM_INDEX_STROBE_INTERVAL 19
#define PARAM_INDEX_DEMO_MODE 20
#define PARAM_INDEX_ENABLE_LOW_VOLT_INDICATOR 21
#define PARAM_INDEX_LOW_PRESSURE_THRESHOLD 22
#define PARAM_INDEX_ENABLE_PRESSURE_ALERT 23
#define PARAM_INDEX_ENABLE_TORPEDO_INDICATOR 24
#define PARAM_INDEX_SERVO_START_ANGLE 25

using namespace uavcan;

// To ease parameter management, all names and default values of parameters are stored in this array as strings.
const char *defaultParameterArray[NUMBER_OF_PARAMETERS][2] =
{
	{"torpedoPulseInterval", "250"}, // in ms
	{"minServoPulse", "900"}, // in us
	{"maxServoPulse", "2000"}, // in us
	{"actuatorIDTorpedo0", "0"},
	{"actuatorIDTorpedo1", "1"},

	{"actuatorIDServo0", "2"},
	{"actuatorIDServo1", "3"},
	{"actuatorIDServo2", "4"},
	{"actuatorIDServo3", "5"},
	{"actuatorIDServo4", "6"},

	{"actuatorIDServo5", "7"},
	{"actuatorIDServo6", "8"},
	{"actuatorIDServo7", "9"},
	{"actuatorIDServo8", "10"},
	{"actuatorIDServo9", "11"},

	{"actuatorIDServo10", "12"},
	{"actuatorIDServo11", "13"},
	{"lightIDRGBStrip0", "0"},
	{"lightIDRGBStrip0_strobe", "248"},
	{"strobeInterval", "500"}, // in ms

	{"demoMode", "0"},
	{"enableLowVoltIndicator", "1"}, //changed to 0 since it's not working right
	{"lowPressureThreshold", "20000"}, // in Pascals
	{"enablePressureAlert", "0"},
	{"enableTorpedoIndicator", "1"},

	{"servoStartAngle", "-1.5"} //in radians
};
//--- Update NUMBER_OF_PARAMETERS when adding new parameters.

struct genericParam {
	char paramName[24]; //Up to 23 characters + 1 null character
	double paramValue; //Be sure to cast to appropriate type when retreiving this value in main program.
} boardConfig[MAX_NUMBER_OF_PARAMETERS];

//Store default parameter names and values into boardConfig struct array.
void initParameters()
{
	for (uint8_t i = 0; i < NUMBER_OF_PARAMETERS && i < MAX_NUMBER_OF_PARAMETERS; i++) {
		strcpy(boardConfig[i].paramName, defaultParameterArray[i][0]);
		boardConfig[i].paramValue = atof(defaultParameterArray[i][1]);
	}
}

void printParameters()
{
	Serial.println("");
	Serial.println("--Printing Parameters--");
	Serial.println("");
	for (uint8_t i = 0; i < NUMBER_OF_PARAMETERS && i < MAX_NUMBER_OF_PARAMETERS; i++) {
		Serial.print(boardConfig[i].paramName);
		Serial.print(": ");
		Serial.println(boardConfig[i].paramValue);
	}
	Serial.println("");
	Serial.println("----");
	Serial.println("");
}

void saveConfig()
{
	Serial.println("Save parameters to EEPROM");
	EEPROM.put(0, boardConfig);
}

void resetConfig()
{
	Serial.println("Reset all params to default values.");
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
		if (index<NUMBER_OF_PARAMETERS)
			out_name = boardConfig[index].paramName;
	}

	void assignParamValue(const Name& name, const Value& value) override
	{
		for (uint8_t i = 0; i < NUMBER_OF_PARAMETERS; i++) {
			if (!strcmp(boardConfig[i].paramName, name.c_str())) {
				boardConfig[i].paramValue = *value.as<uavcan::protocol::param::Value::Tag::real_value>();
				return;
			}
		}
		Serial.println("Can't assign parameter!");
	}

	void readParamValue(const Name& name, Value& out_value) const override
	{
		for (uint8_t i = 0; i < NUMBER_OF_PARAMETERS; i++) {
			if (boardConfig[i].paramName == name) {
				out_value.to<uavcan::protocol::param::Value::Tag::real_value>() = boardConfig[i].paramValue;
				return;
			}
		}
		Serial.println("Can't read parameter!");
	}

	int saveAllParams() override
	{
		saveConfig();
		return 0;	 // Zero means that everything is fine.
	}

	int eraseAllParams() override
	{
		resetConfig();
		return 0;
	}

} param_manager;

uavcan::ParamServer* server;

void initParameter(Node<NodeMemoryPoolSize> *node)
{
	initParameters(); //Prepare boardConfig array
	//saveConfig(); //Run this here if saving new parameters for first time.
	EEPROM.get(0, boardConfig); //Load saved array
	printParameters();
	server = new uavcan::ParamServer(*node);
	const int server_start_res = server->start(&param_manager);
	if (server_start_res < 0)
		Serial.println("Failed to start ParamServer!");
	else
		Serial.println("Started Parameter server successfully!");
}

#endif