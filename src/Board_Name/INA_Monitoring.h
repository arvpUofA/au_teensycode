#ifndef	INA_MONITORING_H
#define INA_MONITORING_H

#include <Arduino.h>

#define NUM_OF_BATTERIES 5
#define NUM_OF_POWER_RAILS 3
#define BATTERY_1_ID 0
#define BATTERY_2_ID 1
#define BATTERY_3_ID 2
#define BATTERY_4_ID 3
#define BATTERY_5_ID 4
#define POWER_RAIL_1_ID 4
#define POWER_RAIL_2_ID 5
#define POWER_RAIL_3_ID 6

struct monitorInfo
{
    float busVoltage_V;
    float shuntVoltage_mV;
    float current_mA;
    float average_power_W;
};

float getBusVoltage(uint8_t id);
float getShuntVoltage(uint8_t id);
float getCurrent(uint8_t id);
float getAveragePower(uint8_t id);

#endif