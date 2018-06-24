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
    delay(2000);
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
        }
    }
    //battery1_2->tick(1); battery1_2->tick(2);
    //battery3_4->tick(1); battery3_4->tick(2);
    //power_rails->tick(1); power_rails->tick(2); power_rails->tick(3);

    /*//INA 1 (Battery Monitoring Board)
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
    power_rails->print(3);*/

    // long d = max(rate_delay - (millis() - start), 0);
    delay(rate_delay);
}
