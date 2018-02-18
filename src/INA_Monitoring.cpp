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
#include "SDL_Arduino_INA3221.h"

#define RATE 100
#define REDC_5V  (2.1/3.1) //Correction for 5V rail
#define REDC_12V (1.3/2)   //Correction for 12V rail

SDL_Arduino_INA3221 *battery1_2;
SDL_Arduino_INA3221 *battery3_4;
SDL_Arduino_INA3221 *power_rails;
const int rate_delay = 1000 / RATE;

void setup(void)
{
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
}

void loop(void)
{

  long start = millis();

  battery1_2->tick(1); battery1_2->tick(2);
  battery3_4->tick(1); battery3_4->tick(2);
  power_rails->tick(1); power_rails->tick(2); power_rails->tick(3);

  //INA 1 (Battery Monitoring Board)
  Serial.print("INA3221_1\n");
  Serial.print("Battery 1: ");
  battery1_2->print(1);
  Serial.print("Battery 2: ");
  battery1_2->print(2);

  //INA 2 (Battery Monitoring Board)
  Serial.print("Battery 3: ");
  battery3_4->print(1);
  Serial.print("Battery 4: ");
  battery3_4->print(2);

  //INA 4 (Power Monitoring Board)
    
  Serial.print("INA3221_4\n");
  Serial.print("3.3V  Rail: ");
  power_rails->print(1);
  Serial.print("5.0V  Rail: ");
  power_rails->print(2);       
  Serial.print("12.0V Rail: ");
  power_rails->print(3);

  // long d = max(rate_delay - (millis() - start), 0);
 delay(100);
}
