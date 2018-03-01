#ifndef	PARAMETER_HPP
#define	PARAMETER_HPP

#include <UAVCAN.hpp>
#include <uavcan/protocol/param_server.hpp>
#include <teensy_uavcan.hpp>
#include <EEPROM.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define NUMBER_OF_PARAMETERS 21
#define MAX_NUMBER_OF_PARAMETERS 64 //based on sizeof(genericParam) (32 bytes) and 2048 byte EEPROM capacity in Teensy 3.2

using namespace uavcan;

// To ease parameter management, all names and default values of parameters are stored in this array as strings.
const char *defaultParameterArray[NUMBER_OF_PARAMETERS][2] =
{
    {"torpedoPulseInterval", "250"}, // in ms
    {"minServoPulse", "1000"}, // in us
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
    {"actuatorIDServo11", "12"},
    {"lightIDRGBStrip0", "0"},
    {"lightIDRGBStrip0_strobe", "248"},
    {"strobeInterval", "500"}, // in ms

    {"demoMode", "1"}
};
//--- Update NUMBER_OF_PARAMETERS when adding new parameters.

struct genericParam
{
    char paramName[24]; //Up to 23 characters + 1 null character
    double paramValue; //Be sure to cast to appropriate type when retreiving this value in main program.
}
boardConfig[NUMBER_OF_PARAMETERS];

//Store default parameter names and values into boardConfig struct array.
void initParameters()
{
    for(uint8_t i = 0; i < NUMBER_OF_PARAMETERS && i < MAX_NUMBER_OF_PARAMETERS; i++)
    {
        strcpy(boardConfig[i].paramName, defaultParameterArray[i][0]);
        boardConfig[i].paramValue = atof(defaultParameterArray[i][1]);
    }
}

/*
 * Now, we need to define some glue logic between the server (below) and our configuration storage (above).
 * This is done via the interface uavcan::IParamManager.
 */
class : public uavcan::IParamManager
{
    void getParamNameByIndex(Index index, Name& out_name) const override
    {
        if(index<NUMBER_OF_PARAMETERS) 
        {
            out_name = boardConfig[index].paramName;
        }
    }

    void assignParamValue(const Name& name, const Value& value) override
    {
        for (uint8_t i = 0; i < NUMBER_OF_PARAMETERS; i++)
        {
            if(!strcmp(boardConfig[i].paramName, name.c_str()))
            {
                boardConfig[i].paramValue = *value.as<uavcan::protocol::param::Value::Tag::real_value>();
                return;
            }
        }
        Serial.println("Can't assign parameter!");
    }

    void readParamValue(const Name& name, Value& out_value) const override
    {
        for (uint8_t i = 0; i < NUMBER_OF_PARAMETERS; i++)
        {
            if(boardConfig[i].paramName == name)
            {
                out_value.to<uavcan::protocol::param::Value::Tag::real_value>() = boardConfig[i].paramValue;
                return;
            }
        }
        Serial.println("Can't read parameter!");
    }

    int saveAllParams() override
    {
        Serial.println("Save to EEPROM");
        EEPROM.put(0, boardConfig);
        return 0;     // Zero means that everything is fine.
    }

    int eraseAllParams() override
    {
        Serial.println("Erase - all params reset to default values stored in EEPROM");
        initParameters();
        return 0;
    }

} param_manager;

uavcan::ParamServer* server;

void initParameter(Node<NodeMemoryPoolSize> *node)
{
    initParameters(); //Prepare boardConfig array
    EEPROM.get(0, boardConfig); //Load previously saved array
    server = new uavcan::ParamServer(*node);
    const int server_start_res = server->start(&param_manager);
    if (server_start_res < 0)
    {
        Serial.println("Failed to start ParamServer!");
    }
    else
    {
        Serial.println("Started Parameterserver successfully!");
    }
}

#endif