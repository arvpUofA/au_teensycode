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
#include <teensy_uavcan.hpp>
#include <subscriber.hpp>
#include <parameter.hpp>
#include "INA_functions.hpp"
#include <watchdog.h>
#include <uavcanNodeIDs.h>

#include <publisher.hpp>

#define RATE 100
#define REDC_5V  (2.1/3.1) //Correction for 5V rail
#define REDC_12V (1.3/2)   //Correction for 12V rail

extern SDL_Arduino_INA3221 *battery1_2;
extern SDL_Arduino_INA3221 *battery3_4;
extern SDL_Arduino_INA3221 *power_rails;
const int rate_delay = 1000 / NUMBER_OF_SAMPLES;

extern Running_Average<float, NUMBER_OF_SAMPLES> avg_pwr_0;
extern Running_Average<float, NUMBER_OF_SAMPLES> avg_pwr_1;
extern Running_Average<float, NUMBER_OF_SAMPLES> avg_pwr_2;
extern Running_Average<float, NUMBER_OF_SAMPLES> avg_pwr_3;
extern Running_Average<float, NUMBER_OF_SAMPLES> avg_pwr_4;
extern Running_Average<float, NUMBER_OF_SAMPLES> avg_pwr_5;
extern Running_Average<float, NUMBER_OF_SAMPLES> avg_pwr_6;

// Node settings
static constexpr uint32_t nodeID = UAVCAN_NODE_ID_BATTERY_MONITORING_BOARD;
static constexpr uint8_t swVersion = 1;
static constexpr uint8_t hwVersion = 1;
static const char* nodeName = "org.arvp.batteryMonitor";

// application settings
static constexpr float framerate = 100;

Metro loopTimer = Metro(10000/NUMBER_OF_SAMPLES);
uint8_t timerCounter;

void printData(struct railInfo railInfo)
{
    Serial.print(railInfo.volts); Serial.print(" V, ");
    Serial.print(railInfo.current); Serial.print(" mA, ");
    Serial.print(railInfo.power); Serial.println(" W");
}

void setup(void)
{
    KickDog();
    Serial.begin(9600);
    Serial.println("SDA_Arduino_INA3221_Test");

    Serial.println("Measuring voltage and current with ina3221 ...");

    battery1_2 = new SDL_Arduino_INA3221(INA3221_ADDRESS_1, SHUNT_RESISTOR_VALUE_1, RATE);
    battery3_4 = new SDL_Arduino_INA3221(INA3221_ADDRESS_2, SHUNT_RESISTOR_VALUE_2, RATE);
    power_rails = new SDL_Arduino_INA3221(INA3221_ADDRESS_4, SHUNT_RESISTOR_VALUE_4, RATE);

    power_rails->setReduction(2, REDC_5V);
    power_rails->setReduction(3, REDC_12V);

    battery1_2->begin();
    battery3_4->begin();
    power_rails->begin();

    // init LEDs
    initLeds();

    // Create a node
    systemClock = &getSystemClock();
    canDriver = &getCanDriver();
    node = new Node<NodeMemoryPoolSize>(*canDriver, *systemClock);
    initNode(node, nodeID, nodeName, swVersion, hwVersion);

    // init publisher
    initPublisher(node);

    // start up node
    node->setModeOperational();
}

void loop(void)
{
    KickDog();
    if(loopTimer.check())
    {
        timerCounter++;

        avg_pwr_0.AddSample(battery1_2->getPower(1)); //Horizontal thrusters
        avg_pwr_1.AddSample(battery1_2->getPower(2)); //Vertical thrusters
        avg_pwr_2.AddSample(battery3_4->getPower(1)); //Strafing thrusters
        avg_pwr_3.AddSample(battery3_4->getPower(2)); //Electronics

        avg_pwr_4.AddSample(power_rails->getPower(1)); //3.3V
        avg_pwr_5.AddSample(power_rails->getPower(2)); //5V
        avg_pwr_6.AddSample(power_rails->getPower(3)); //12V

        if (timerCounter == 10) //10 samples per second
        {
            updateBatteries();
            updatePwrRails();

            Serial.println("Battery 0:");
            printData(battery_0);

            Serial.println("Battery 1:");
            printData(battery_1);

            Serial.println("Battery 2:");
            printData(battery_2);

            Serial.println("Battery 3:");
            printData(battery_3);

            Serial.println("3.3V");
            printData(power_rail_0);

            Serial.println("5V");
            printData(power_rail_1);

            Serial.println("12V");
            printData(power_rail_2);

            timerCounter = 0;

            // publish messages
            cyclePublisher();

        }
    }
    // wait in cycle
    cycleWait(framerate);

    // do some CAN stuff
    cycleNode(node);

    // toggle heartbeat
    toggleHeartBeat();
}
