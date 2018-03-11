#ifndef	PUBLISHER_HPP
#define	PUBLISHER_HPP

#include <UAVCAN.hpp>
#include <uavcan/equipment/power/BatteryInfo.hpp>
#include <uavcan/equipment/power/CircuitStatus.hpp>
#include "teensy_uavcan.hpp"
#include "INA_Monitoring.h"

using namespace uavcan;

// publisher
Publisher<equipment::power::BatteryInfo> *batteryPublisher;
Publisher<equipment::power::CircuitStatus> *powerRailPublisher;


void initPublisher(Node<NodeMemoryPoolSize> *node)
{
    // create publishers
    batteryPublisher = new Publisher<equipment::power::BatteryInfo>(*node);
    powerRailPublisher = new Publisher<equipment::power::CircuitStatus>(*node);

    // initiliaze publisher
    if(batteryPublisher->init() < 0)
    {
        Serial.println("Unable to initialize battery message publisher!");
    }
    if(powerRailPublisher->init() < 0)
    {
        Serial.println("Unable to initialize power rail message publisher!");
    }

    // set TX timeout
    batteryPublisher->setTxTimeout(MonotonicDuration::fromUSec(800));
    powerRailPublisher->setTxTimeout(MonotonicDuration::fromUSec(800));
}

void updateBatteryInfo(equipment::power::BatteryInfo *battery, uint8_t id)
{
    (battery + id)->voltage = getBusVoltage(id);
    (battery + id)->current = getCurrent(id);
    (battery + id)->average_power_10sec = getAveragePower(id);
}

void updatePowerRailInfo(equipment::power::CircuitStatus *powerRail, uint8_t id)
{
    // the NUM_OF_BATTERIES is used to set the first power rail to index 0
    (powerRail + id - NUM_OF_BATTERIES)->voltage = getBusVoltage(id);
    (powerRail + id - NUM_OF_BATTERIES)->current = getCurrent(id);
}

void cyclePublisher()
{
    // send a very important log message to everyone
    equipment::power::BatteryInfo *battery;
    equipment::power::CircuitStatus *powerRail;

    updateBatteryInfo(battery, BATTERY_1_ID);
    updateBatteryInfo(battery, BATTERY_2_ID);
    updateBatteryInfo(battery, BATTERY_3_ID);
    updateBatteryInfo(battery, BATTERY_4_ID);
    updatePowerRailInfo(powerRail, POWER_RAIL_1_ID);
    updatePowerRailInfo(powerRail, POWER_RAIL_2_ID);
    updatePowerRailInfo(powerRail, POWER_RAIL_3_ID);

    const int pres1 = batteryPublisher->broadcast(*(battery + BATTERY_1_ID));
    if (pres1 < 0)
    {
        Serial.println("Error while broadcasting first battery message");
    }
    const int pres2 = batteryPublisher->broadcast(*(battery + BATTERY_2_ID));
    if (pres2 < 0)
    {
        Serial.println("Error while broadcasting second battery message");
    }
    const int pres3 = batteryPublisher->broadcast(*(battery + BATTERY_3_ID));
    if (pres3 < 0)
    {
        Serial.println("Error while broadcasting third battery message");
    }
    const int pres4 = batteryPublisher->broadcast(*(battery + BATTERY_4_ID));
    if (pres4 < 0)
    {
        Serial.println("Error while broadcasting fourth battery message");
    }
    const int pres5 = powerRailPublisher->broadcast(*(powerRail + POWER_RAIL_1_ID - NUM_OF_BATTERIES));
    if (pres5 < 0)
    {
        Serial.println("Error while broadcasting first power rail message");
    }
    const int pres6 = powerRailPublisher->broadcast(*(powerRail + POWER_RAIL_2_ID - NUM_OF_BATTERIES));
    if (pres6 < 0)
    {
        Serial.println("Error while broadcasting second power rail message");
    }
    const int pres7 = powerRailPublisher->broadcast(*(powerRail + POWER_RAIL_3_ID - NUM_OF_BATTERIES));
    if (pres7 < 0)
    {
        Serial.println("Error while broadcasting third power rail message");
    }
}


#endif
