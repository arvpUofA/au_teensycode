#ifndef	INTERNAL_ENV_BOARD_HPP
#define INTERNAL_ENV_BOARD_HPP

#ifdef BOARD_SELECT_INTERNAL_ENV_BOARD

#include "Arduino.h"
#include <Wire.h>
#include <Metro.h>

#include <internalEnvBoard/teensy_uavcan.hpp>
#include <internalEnvBoard/publisher.hpp>
#include <uavcanNodeIDs.h>
//#include "internalEnvBoard.h"
#include <watchdog.h>
#include <running_average.hpp>

// UAVCAN Node settings
static constexpr uint32_t nodeID = UAVCAN_NODE_ID_INTERNAL_SENSOR_BOARD;
static constexpr uint8_t swVersion = 1;
static constexpr uint8_t hwVersion = 1;
static const char *nodeName = "org.arvp.internalSensor";

// UAVCAN application settings
static constexpr float framerate = 100;

// humidity/temperature sensor
#define HIH7120ADDRESS 0x27

// pressure/temperature sensor
#define MPLADDRESS 0x60
unsigned char buf[5];
unsigned char sta;

// averaging class initialising
Running_Average<uint32_t, 20> avg_pressure;

// instantiate the timer for publishing message
Metro timer = Metro(1000);

//use this for reading out pressure data in two parts
struct pressure_StructDef {
  uint32_t whole;
  uint8_t fractional;
};

uint8_t OUT_P_MSB, OUT_P_CSB, OUT_P_LSB;

// calculates humidity from raw data
float humidity(){
    float hum = (buf[0] << 8) + buf[1];
    float humidity = float(hum)/16382*100;
    return humidity;
}
// calculates temperature from raw data
float temp(){
    float temp = (buf[2] << 6) + (buf[3] >> 2);
    float temperature = (temp/16382*165)-40;
    return temperature;
}

//must be used before to tell the HIH7120 to make a measurement
//there must be a delay between measurement request and retrieving
//the measurement from the sensor which is built into the function
//below
void hum_measurement_req(){
    Wire.beginTransmission(HIH7120ADDRESS);
    Wire.write(HIH7120ADDRESS<<1);
    Wire.endTransmission();
    delay(50);
}

// read humidity and temperature data from HIH7120
void measureHIH7120(){
    if (Wire.available()==4){
        for (int i = 0; i <4; i++){
            buf[i]=Wire.read();
        }
        if(!bitRead(buf[0], 7)&&bitRead(buf[0], 6)){
            Serial.println("Data is stale");
        }
    } else {
        Serial.print("Error in data.\n");
    }
}

// initialize Mpl sensor based on sample program from data sheet
void setupMPL(){
    Wire.beginTransmission(MPLADDRESS);
    Wire.write(0x26);
    Wire.write(0x39); //Turn on and enable oversampling
    Wire.endTransmission();
    delay(30);
}

pressure_StructDef readPressureMPL() {
    pressure_StructDef pressure;

    Wire.beginTransmission(MPLADDRESS);
    Wire.write(0);
    Wire.requestFrom(MPLADDRESS, 4);
    delay(30);
    if (Wire.available()) {
        Wire.read(); //discard the first byte, it doesn't matter
        //readings start at 0x01
        OUT_P_MSB = Wire.read();
        OUT_P_CSB = Wire.read();
        OUT_P_LSB = Wire.read();
        /*
        * 20 bit value, with 18 bits + 2 bits fractional value
        * MSB [7:0], CSB [7:0], and LSB [7:6] make up 18 bits
        * LSB [5:4] make up the fractional component
        */
        pressure.whole = 0;
        pressure.whole += (OUT_P_MSB << 10);
        pressure.whole += (OUT_P_CSB << 2);
        pressure.whole += (OUT_P_LSB >> 6);

        pressure.fractional = 0;
        //only bytes 4 and 5 are relevant, so bitshift 4 right
        //then clear everything other than 4 and 5
        pressure.fractional = ((OUT_P_LSB >> 4) & 0b00000011) / 4;
    }
    Wire.endTransmission();
    return pressure;
}

float publishTemp() {
    return temp();
}

float publishPress() {
    pressure_StructDef pressure = readPressureMPL();
    avg_pressure.AddSample(pressure.whole);
    return avg_pressure.Average();
}

float publishHumidity() {
    return humidity();
}

#endif

#endif