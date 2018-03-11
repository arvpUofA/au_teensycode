#ifndef	PUBLISHER_HPP
#define	PUBLISHER_HPP

#include <UAVCAN.hpp>
#include <uavcan/equipment/power/BatteryInfo.hpp>
#include "teensy_uavcan.hpp"
#include "INA_Monitoring.h"

using namespace uavcan;

// publisher
Publisher<equipment::power::BatteryInfo> *batteryPublisher;

void initPublisher(Node<NodeMemoryPoolSize> *node)
{
    // create publisher
    batteryPublisher = new Publisher<equipment::power::BatteryInfo>(*node);

    // initiliaze publisher
    if(batteryPublisher->init() < 0)
    {
        Serial.println("Unable to initialize battery message publisher!");
    }

    // set TX timeout
    batteryPublisher->setTxTimeout(MonotonicDuration::fromUSec(800));
}

void updateBatteryInfo(equipment::power::BatteryInfo *batteryInfo, uint8_t id)
{
    (batteryInfo + id)->voltage = getBusVoltage(id);
    (batteryInfo + id)->current = getCurrent(id);
    (batteryInfo + id)->average_power_10sec = getAveragePower(id);
}

void cyclePublisher()
{
    // send a very important log message to everyone
    equipment::power::BatteryInfo *btry;

    updateBatteryInfo(btry, BATTERY_1_ID);
    updateBatteryInfo(btry, BATTERY_2_ID);
    updateBatteryInfo(btry, BATTERY_3_ID);
    updateBatteryInfo(btry, BATTERY_4_ID);
    updateBatteryInfo(btry, POWER_RAIL_1_ID);
    updateBatteryInfo(btry, POWER_RAIL_2_ID);
    updateBatteryInfo(btry, POWER_RAIL_3_ID);

    const int pres1 = batteryPublisher->broadcast(*(btry + BATTERY_1_ID));
    if (pres1 < 0)
    {
        Serial.println("Error while broadcasting first battery message");
    }
    const int pres2 = batteryPublisher->broadcast(*(btry + BATTERY_2_ID));
    if (pres2 < 0)
    {
        Serial.println("Error while broadcasting second battery message");
    }
    const int pres3 = batteryPublisher->broadcast(*(btry + BATTERY_3_ID));
    if (pres3 < 0)
    {
        Serial.println("Error while broadcasting third battery message");
    }
    const int pres4 = batteryPublisher->broadcast(*(btry + BATTERY_4_ID));
    if (pres4 < 0)
    {
        Serial.println("Error while broadcasting fourth battery message");
    }
    const int pres5 = batteryPublisher->broadcast(*(btry + POWER_RAIL_1_ID));
    if (pres5 < 0)
    {
        Serial.println("Error while broadcasting fifth battery message");
    }
    const int pres6 = batteryPublisher->broadcast(*(btry + POWER_RAIL_2_ID));
    if (pres6 < 0)
    {
        Serial.println("Error while broadcasting sixth battery message");
    }
    const int pres7 = batteryPublisher->broadcast(*(btry + POWER_RAIL_3_ID));
    if (pres7 < 0)
    {
        Serial.println("Error while broadcasting seventh battery message");
    }
}


#endif
