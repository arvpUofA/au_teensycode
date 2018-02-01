#include <Arduino.h>
//
//   SDL_Arduino_INA3221 Library Test Code
//   SDL_Arduino_INA3221.cpp Arduino code - runs in continuous mode
//   Version 1.1
//   SwitchDoc Labs   January 31, 2015
//
//
// This was designed for SunAirPlus - Solar Power Controller - www.switchdoc.com
//
 
#include <i2c_t3.h>
#include "SDL_Arduino_INA3221.h"

SDL_Arduino_INA3221 ina3221;
SDL_Arduino_INA3221_2 ina3221_2;
SDL_Arduino_INA3221_3 ina3221_3;
SDL_Arduino_INA3221_4 ina3221_4;

// the three channels of the INA3221 named for SunAirPlus Solar Power Controller channels (www.switchdoc.com)
#define LIPO_BATTERY_CHANNEL 1
#define SOLAR_CELL_CHANNEL 2
#define OUTPUT_CHANNEL 3
 
void setup(void)
{
  Serial.begin(9600);
  Serial.println("SDA_Arduino_INA3221_Test");

  Serial.println("Measuring voltage and current with ina3221 ...");
  ina3221.begin();
  ina3221_2.begin(); 
}

void loop(void)
{
//INA 1 (Battery Monitoring Board)
  float shuntvoltage1 = 0;
  float busvoltage1 = 0;
  float current_mA1 = 0;
  float loadvoltage1 = 0;

  busvoltage1 = ina3221.getBusVoltage_V(LIPO_BATTERY_CHANNEL);
  shuntvoltage1 = ina3221.getShuntVoltage_mV(LIPO_BATTERY_CHANNEL);
  current_mA1 = -ina3221.getCurrent_mA(LIPO_BATTERY_CHANNEL);  // minus is to get the "sense" right.   - means the battery is charging, + that it is discharging
  loadvoltage1 = busvoltage1 + (shuntvoltage1 / 1000);

  float shuntvoltage2 = 0;
  float busvoltage2 = 0;
  float current_mA2 = 0;
  float loadvoltage2 = 0;

  busvoltage2 = ina3221.getBusVoltage_V(SOLAR_CELL_CHANNEL);
  shuntvoltage2 = ina3221.getShuntVoltage_mV(SOLAR_CELL_CHANNEL);
  current_mA2 = -ina3221.getCurrent_mA(SOLAR_CELL_CHANNEL);
  loadvoltage2 = busvoltage2 + (shuntvoltage2 / 1000);

  float shuntvoltage3 = 0;
  float busvoltage3 = 0;
  float current_mA3 = 0;
  float loadvoltage3 = 0;

  busvoltage3 = ina3221.getBusVoltage_V(OUTPUT_CHANNEL);
  shuntvoltage3 = ina3221.getShuntVoltage_mV(OUTPUT_CHANNEL);
  current_mA3 = ina3221.getCurrent_mA(OUTPUT_CHANNEL);
  loadvoltage3 = busvoltage3 + (shuntvoltage3 / 1000);

Serial.print("INA3221_1\n");
Serial.print("Battery 1:");
  Serial.print(busvoltage1);  Serial.print(",");
  Serial.print(shuntvoltage1); Serial.print(",");
  Serial.print(loadvoltage1); Serial.print(",");
  Serial.print(current_mA1);  Serial.print(",");
Serial.print("\n Battery 2:");
  Serial.print(busvoltage2);  Serial.print(",");
  Serial.print(shuntvoltage2); Serial.print(",");
  Serial.print(loadvoltage2);  Serial.print(",");
  Serial.print(current_mA2); Serial.print(",");
  //Serial.print(busvoltage3); Serial.print(",");
  //Serial.print(shuntvoltage3); Serial.print(",");
  //Serial.print(loadvoltage3); Serial.print(",");
  //Serial.print(current_mA3); Serial.print(",");
  //Serial.println("");

//INA 2 (Battery Monitoring Board)
  float shuntvoltage1_2 = 0;
  float busvoltage1_2 = 0;
  float current_mA1_2 = 0;
  float loadvoltage1_2 = 0;

  busvoltage1_2 = ina3221_2.getBusVoltage_V(LIPO_BATTERY_CHANNEL);
  shuntvoltage1_2 = ina3221_2.getShuntVoltage_mV(LIPO_BATTERY_CHANNEL);
  current_mA1_2 = -ina3221_2.getCurrent_mA(LIPO_BATTERY_CHANNEL);  // minus is to get the "sense" right.   - means the battery is charging, + that it is discharging
  loadvoltage1_2 = busvoltage1_2 + (shuntvoltage1_2 / 1000);

  float shuntvoltage2_2 = 0;
  float busvoltage2_2 = 0;
  float current_mA2_2 = 0;
  float loadvoltage2_2 = 0;

  busvoltage2_2 = ina3221_2.getBusVoltage_V(SOLAR_CELL_CHANNEL);
  shuntvoltage2_2 = ina3221_2.getShuntVoltage_mV(SOLAR_CELL_CHANNEL);
  current_mA2_2 = -ina3221_2.getCurrent_mA(SOLAR_CELL_CHANNEL);
  loadvoltage2_2 = busvoltage2_2 + (shuntvoltage2_2 / 1000);

  float shuntvoltage3_2 = 0;
  float busvoltage3_2 = 0;
  float current_mA3_2 = 0;
  float loadvoltage3_2 = 0;

  busvoltage3_2 = ina3221_2.getBusVoltage_V(OUTPUT_CHANNEL);
  shuntvoltage3_2 = ina3221_2.getShuntVoltage_mV(OUTPUT_CHANNEL);
  current_mA3_2 = ina3221_2.getCurrent_mA(OUTPUT_CHANNEL);
  loadvoltage3_2 = busvoltage3_2 + (shuntvoltage3_2 / 1000);
  
Serial.print("INA3221_2\n");
Serial.print("Battery 3:");
  Serial.print(busvoltage1_2);  Serial.print(",");
  Serial.print(shuntvoltage1_2); Serial.print(",");
  Serial.print(loadvoltage1_2); Serial.print(",");
  Serial.print(current_mA1_2);  Serial.print(",");
Serial.print("\n Battery 4:");
  Serial.print(busvoltage2_2);  Serial.print(",");
  Serial.print(shuntvoltage2_2); Serial.print(",");
  Serial.print(loadvoltage2_2);  Serial.print(",");
  Serial.print(current_mA2_2); Serial.print(",");
  //Serial.print(busvoltage3_2); Serial.print(",");
  //Serial.print(shuntvoltage3_2); Serial.print(",");
  //Serial.print(loadvoltage3_2); Serial.print(",");
  //Serial.print(current_mA3_2); Serial.print(",");
  //Serial.println("");

//INA 3 (Power Monitoring Board)
float shuntvoltage1_3 = 0;
  float busvoltage1_3 = 0;
  float current_mA1_3 = 0;
  float loadvoltage1_3 = 0;

  busvoltage1_3 = ina3221_3.getBusVoltage_V(LIPO_BATTERY_CHANNEL);
  shuntvoltage1_3 = ina3221_3.getShuntVoltage_mV(LIPO_BATTERY_CHANNEL);
  current_mA1_3 = -ina3221_3.getCurrent_mA(LIPO_BATTERY_CHANNEL);  // minus is to get the "sense" right.   - means the battery is charging, + that it is discharging
  loadvoltage1_3 = busvoltage1_3 + (shuntvoltage1_3 / 1000);

  float shuntvoltage2_3 = 0;
  float busvoltage2_3 = 0;
  float current_mA2_3 = 0;
  float loadvoltage2_3 = 0;

  busvoltage2_3 = ina3221_3.getBusVoltage_V(SOLAR_CELL_CHANNEL);
  shuntvoltage2_3 = ina3221_3.getShuntVoltage_mV(SOLAR_CELL_CHANNEL);
  current_mA2_3 = -ina3221_3.getCurrent_mA(SOLAR_CELL_CHANNEL);
  loadvoltage2_3 = busvoltage2_3 + (shuntvoltage2_3 / 1000);

  float shuntvoltage3_3 = 0;
  float busvoltage3_3 = 0;
  float current_mA3_3 = 0;
  float loadvoltage3_3 = 0;

  busvoltage3_3 = ina3221_3.getBusVoltage_V(OUTPUT_CHANNEL);
  shuntvoltage3_3 = ina3221_3.getShuntVoltage_mV(OUTPUT_CHANNEL);
  current_mA3_3 = ina3221_3.getCurrent_mA(OUTPUT_CHANNEL);
  loadvoltage3_3 = busvoltage3_3 + (shuntvoltage3_3 / 1000);
  
Serial.print("INA3221_3\n");
Serial.print("INA3 Channel 1:");
  Serial.print(busvoltage1_3);  Serial.print(",");
  Serial.print(shuntvoltage1_3); Serial.print(",");
  Serial.print(loadvoltage1_3); Serial.print(",");
  Serial.print(current_mA1_3);  Serial.print(",");
Serial.print("\n INA 3 Channel 2:");
  Serial.print(busvoltage2_3);  Serial.print(",");
  Serial.print(shuntvoltage2_3); Serial.print(",");
  Serial.print(loadvoltage2_3);  Serial.print(",");
  Serial.print(current_mA2_3); Serial.print(",");
Serial.print("\n INA 3 Channel 3:");
  Serial.print(busvoltage3_3); Serial.print(",");
  Serial.print(shuntvoltage3_3); Serial.print(",");
  Serial.print(loadvoltage3_3); Serial.print(",");
  Serial.print(current_mA3_3); Serial.print(",");
  Serial.println("");

//INA 4 (Power Monitoring Board)
float shuntvoltage1_4 = 0;
  float busvoltage1_4 = 0;
  float current_mA1_4 = 0;
  float loadvoltage1_4 = 0;

  busvoltage1_4 = ina3221_4.getBusVoltage_V(LIPO_BATTERY_CHANNEL);
  shuntvoltage1_4 = ina3221_4.getShuntVoltage_mV(LIPO_BATTERY_CHANNEL);
  current_mA1_4 = -ina3221_4.getCurrent_mA(LIPO_BATTERY_CHANNEL);  // minus is to get the "sense" right.   - means the battery is charging, + that it is discharging
  loadvoltage1_4 = busvoltage1_4 + (shuntvoltage1_4 / 1000);

  float shuntvoltage2_4 = 0;
  float busvoltage2_4 = 0;
  float current_mA2_4 = 0;
  float loadvoltage2_4 = 0;

  busvoltage2_4 = ina3221_4.getBusVoltage_V(SOLAR_CELL_CHANNEL);
  shuntvoltage2_4 = ina3221_4.getShuntVoltage_mV(SOLAR_CELL_CHANNEL);
  current_mA2_4 = -ina3221_4.getCurrent_mA(SOLAR_CELL_CHANNEL);
  loadvoltage2_4 = busvoltage2_4 + (shuntvoltage2_4 / 1000);

  float shuntvoltage3_4 = 0;
  float busvoltage3_4 = 0;
  float current_mA3_4 = 0;
  float loadvoltage3_4 = 0;

  busvoltage3_4 = ina3221_4.getBusVoltage_V(OUTPUT_CHANNEL);
  shuntvoltage3_4 = ina3221_4.getShuntVoltage_mV(OUTPUT_CHANNEL);
  current_mA3_4 = ina3221_4.getCurrent_mA(OUTPUT_CHANNEL);
  loadvoltage3_4 = busvoltage3_4 + (shuntvoltage3_4 / 1000);
  
Serial.print("INA3221_4\n");
Serial.print("INA 4 Channel 1:");
  Serial.print(busvoltage1_4);  Serial.print(",");
  Serial.print(shuntvoltage1_4); Serial.print(",");
  Serial.print(loadvoltage1_4); Serial.print(",");
  Serial.print(current_mA1_4);  Serial.print(",");
Serial.print("\n INA 4 Channel 2:");
  Serial.print(busvoltage2_4);  Serial.print(",");
  Serial.print(shuntvoltage2_4); Serial.print(",");
  Serial.print(loadvoltage2_4);  Serial.print(",");
  Serial.print(current_mA2_4); Serial.print(",");
Serial.print("\n INA 4 Channel 3:");
  Serial.print(busvoltage3_4); Serial.print(",");
  Serial.print(shuntvoltage3_4); Serial.print(",");
  Serial.print(loadvoltage3_4); Serial.print(",");
  Serial.print(current_mA3_4); Serial.print(",");
  Serial.println("");
}
