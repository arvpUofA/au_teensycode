#ifndef INA_FUNCTIONS_HPP
#define INA_FUNCTIONS_HPP

#include "SDL_Arduino_INA3221.h"
#include "running_average.hpp"

#define NUMBER_OF_SAMPLES 100

struct railInfo
{
    float volts;
    float current;
    float power;
};

struct railInfo battery_0;
struct railInfo battery_1;
struct railInfo battery_2;
struct railInfo battery_3;

struct railInfo power_rail_0;
struct railInfo power_rail_1;
struct railInfo power_rail_2;

SDL_Arduino_INA3221 *battery1_2;
SDL_Arduino_INA3221 *battery3_4;
SDL_Arduino_INA3221 *power_rails;

Running_Average<float, NUMBER_OF_SAMPLES> avg_pwr_0;
Running_Average<float, NUMBER_OF_SAMPLES> avg_pwr_1;
Running_Average<float, NUMBER_OF_SAMPLES> avg_pwr_2;
Running_Average<float, NUMBER_OF_SAMPLES> avg_pwr_3;
Running_Average<float, NUMBER_OF_SAMPLES> avg_pwr_4;
Running_Average<float, NUMBER_OF_SAMPLES> avg_pwr_5;
Running_Average<float, NUMBER_OF_SAMPLES> avg_pwr_6;

void updateBatteries(void) {
  // Update battery 0
  battery_0.volts = battery1_2->getBusVoltage_V(1);
  battery_0.current = battery1_2->getCurrent_mA(1);
  battery_0.power = avg_pwr_0.Average();

  // Update battery 1
  battery_1.volts = battery1_2->getBusVoltage_V(2);
  battery_1.current = battery1_2->getCurrent_mA(2);
  battery_1.power = avg_pwr_1.Average();

  // Update battery 2
  battery_2.volts = battery3_4->getBusVoltage_V(1);
  battery_2.current = battery3_4->getCurrent_mA(1);
  battery_2.power = avg_pwr_2.Average();

  // Update battery 3
  battery_3.volts = battery3_4->getBusVoltage_V(2);
  battery_3.current = battery3_4->getCurrent_mA(2);
  battery_3.power = avg_pwr_3.Average();
}

void updatePwrRails(void) {
  // Update 3v3 rail
  power_rail_0.volts = power_rails->getBusVoltage_V(1);
  power_rail_0.current = power_rails->getCurrent_mA(1);
  power_rail_0.power = avg_pwr_4.Average();

  // Update 3v3 rail
  power_rail_1.volts = power_rails->getBusVoltage_V(2);
  power_rail_1.current = power_rails->getCurrent_mA(2);
  power_rail_1.power = avg_pwr_5.Average();

  // Update 12v rail
  power_rail_2.volts = power_rails->getBusVoltage_V(3);
  power_rail_2.current = power_rails->getCurrent_mA(3);
  power_rail_2.power = avg_pwr_6.Average();
}

#endif
