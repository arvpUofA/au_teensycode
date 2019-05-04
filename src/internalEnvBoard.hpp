#ifndef	INTERNAL_ENV_BOARD_HPP
#define INTERNAL_ENV_BOARD_HPP

#ifdef BOARD_SELECT_INTERNAL_ENV_BOARD

#include "Arduino.h"
#include <Wire.h>

#include <internalEnvBoard/teensy_uavcan.hpp>
#include <internalEnvBoard/publisher.hpp>
#include <running_average.hpp>
#include "pressure.h"


// UAVCAN application settings
static constexpr float framerate = 100;

// humidity/temperature sensor
#define HIH7120ADDRESS 0x27

// pressure/temperature sensor
#define MPLADDRESS 0x60

// data buffer for humidity and temperature
unsigned char HIH_buffer[5];

pressure_StructDef press_value;

// averaging class initialising
Running_Average<uint32_t, 10> avg_pressure;
Running_Average<float, 10> avg_temperature;
Running_Average<float, 10> avg_humidity;

uint8_t OUT_P_MSB, OUT_P_CSB, OUT_P_LSB;

// calculates humidity from raw data
float humidity(){
    float hum = (HIH_buffer[0] << 8) + HIH_buffer[1];
    float humidity = float(hum)/16382*100;
    return humidity;
}
// calculates temperature from raw data
float temp(){
    float temp = (HIH_buffer[2] << 6) + (HIH_buffer[3] >> 2);
    float temperature = (temp/16382*165)-40;
    return temperature;
}

uint32_t pressure() {
  return press_value.whole;
}

// read humidity and temperature data from HIH7120
// there must be a delay between measurement request and retrieving
// the measurement from the sensor which is built into the function
// below
void measureHIH7120(){
  Wire.beginTransmission(HIH7120ADDRESS);
  Wire.write(HIH7120ADDRESS<<1);
  Wire.endTransmission();
  delay(50);

  Wire.requestFrom(HIH7120ADDRESS, 4);

  if (Wire.available()==4){
    for (int i = 0; i <4; i++){
        HIH_buffer[i]=Wire.read();
    }
    if(!bitRead(HIH_buffer[0], 7)&&bitRead(HIH_buffer[0], 6)){
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
        press_value.whole = 0;
        press_value.whole += (OUT_P_MSB << 10);
        press_value.whole += (OUT_P_CSB << 2);
        press_value.whole += (OUT_P_LSB >> 6);

        press_value.fractional = 0;
        //only bytes 4 and 5 are relevant, so bitshift 4 right
        //then clear everything other than 4 and 5
        press_value.fractional = ((OUT_P_LSB >> 4) & 0b00000011) / 4;
    }
    Wire.endTransmission();
    return press_value;
}

float publishHumidity() {
    return avg_humidity.Average();
}

float publishTemp() {
    return avg_temperature.Average();
}

// here's an issue: avg_pressure returns uint32_t, but it thinks its a float
// do these values match?
// the older code apparently worked, and it did thinks
// TODO: make this proper
float publishPress() {
    return avg_pressure.Average();
}

#endif

#endif
