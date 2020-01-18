//
//   SDL_Arduino_INA3221 Library Test Code
//   SDL_Arduino_INA3221.cpp Arduino code - runs in continuous mode
//   Version 1.1
//   SwitchDoc Labs   January 31, 2015
//
//
// This was designed for SunAirPlus - Solar Power Controller - www.switchdoc.com
//

#include <Arduino.h> 
#include <i2c_t3.h>
#include <Metro.h>
#include <uavcanNodeIDs.h>
#include "INA_Monitoring.h"
#include "SDL_Arduino_INA3221.h"
#include "teensy_uavcan.hpp"
#include "publisher.hpp"

#define RATE 100
#define REDC_5V  (2.1/3.1) //Correction for 5V rail
#define REDC_12V (1.3/2)   //Correction for 12V rail

SDL_Arduino_INA3221 *battery1_2;
SDL_Arduino_INA3221 *battery3_4;
SDL_Arduino_INA3221 *battery5;
SDL_Arduino_INA3221 *power_rails;
const int rate_delay = 1000 / RATE;

// UAVCAN Node settings
static constexpr uint32_t nodeID = UAVCAN_NODE_ID_BATTERY_MONITORING_BOARD;
static constexpr uint8_t swVersion = 1;
static constexpr uint8_t hwVersion = 1;
static const char *nodeName = "org.arvp.batteryMonitor";

// UAVCAN application settings
static constexpr float framerate = 100;

Metro timer = Metro(1000);

monitorInfo batteryInfo[7];

void updateMonitorInfo(SDL_Arduino_INA3221* ina, uint8_t id, int channel)
{
    (batteryInfo + id)->busVoltage_V = ina->getBusVoltage_V(channel);
    (batteryInfo + id)->shuntVoltage_mV = ina->getShuntVoltage_mV(channel);
    (batteryInfo + id)->current_mA = ina->getCurrent_mA(channel);
    (batteryInfo + id)->average_power_W = ina->getAveragePower_W(channel);
}

float getBusVoltage(uint8_t id)
{
    return (batteryInfo + id)->busVoltage_V;
}

float getShuntVoltage(uint8_t id)
{
    return (batteryInfo + id)->shuntVoltage_mV;
}

float getCurrent(uint8_t id)
{
    return (batteryInfo + id)->current_mA;
}

float getAveragePower(uint8_t id)
{
    return (batteryInfo + id)->average_power_W;
}

void setup(void)
{
    Serial.begin(9600);

    Serial.println("SDA_Arduino_INA3221_Test");
    Serial.println("Measuring voltage and current with ina3221 ...");
    
    battery1_2 = new SDL_Arduino_INA3221(INA3221_ADDRESS_1, SHUNT_RESISTOR_VALUE_1, RATE);
    battery3_4 = new SDL_Arduino_INA3221(INA3221_ADDRESS_2, SHUNT_RESISTOR_VALUE_2, RATE); 
    battery5 = new SDL_Arduino_INA3221(INA3221_ADDRESS_3, SHUNT_RESISTOR_VALUE_3, RATE)
    power_rails = new SDL_Arduino_INA3221(INA3221_ADDRESS_4, SHUNT_RESISTOR_VALUE_4, RATE);

    power_rails->setReduction(2, REDC_5V);
    power_rails->setReduction(3, REDC_12V);

    battery1_2->begin();
    battery3_4->begin();
    battery5->begin();
    power_rails->begin();

    // Create a node
    systemClock = &initSystemClock();
    canDriver = &initCanDriver();
    node = new Node<NodeMemoryPoolSize>(*canDriver, *systemClock);
    initNode(node, nodeID, nodeName, swVersion, hwVersion);

    // init subscriber
    initPublisher(node);

    // start up node
    node->setModeOperational();

    initLeds();
}

void loop(void)
{
    battery1_2->tick(1); battery1_2->tick(2);
    battery3_4->tick(1); battery3_4->tick(2);
    battery5->tick(1);
    power_rails->tick(1); power_rails->tick(2); power_rails->tick(3);

    if (timer.check() == 1) 
    {
        // update info recieved from INA 1
        updateMonitorInfo(battery1_2, BATTERY_1_ID, 1);
        updateMonitorInfo(battery1_2, BATTERY_2_ID, 2);

        //INA 1 (Battery Monitoring Board)
        Serial.print("INA3221_1\n");
        Serial.print("Battery 1: ");
        battery1_2->print(1);
        Serial.print("Battery 2: ");
        battery1_2->print(2);

        // update info recieved from INA 2
        updateMonitorInfo(battery3_4, BATTERY_3_ID, 1);
        updateMonitorInfo(battery3_4, BATTERY_4_ID, 2);

        //INA 2 (Battery Monitoring Board)
        Serial.print("Battery 3: ");
        battery3_4->print(1);
        Serial.print("Battery 4: ");
        battery3_4->print(2);

        //update info recieved from INA 3
        updateMonitorInfo(battery5,BATTERY_5_ID,1);

        //INA 3 (Battery Monitoring Board)
        Serial.print("Battery 5: ");
        battery5->print(1);

        // update info recieved from INA 4
        updateMonitorInfo(power_rails, POWER_RAIL_1_ID, 1);
        updateMonitorInfo(power_rails, POWER_RAIL_2_ID, 2);
        updateMonitorInfo(power_rails, POWER_RAIL_3_ID, 3);

        //INA 4 (Power Monitoring Board)
        Serial.print("INA3221_4\n");
        Serial.print("3.3V  Rail: ");
        power_rails->print(1);
        Serial.print("5.0V  Rail: ");
        power_rails->print(2);       
        Serial.print("12.0V Rail: ");
        power_rails->print(3);

        cyclePublisher();
    }

    cycleWait(framerate);
    cycleNode(node);
    toggleHeartBeat();
}
