#include "Arduino.h"
#include <Wire.h>
// humidity/temperature sensor
#define HIH7120ADDRESS 0x27
// pressure/temperature sensor
#define MPLADDRESS 0xC0
unsigned char buf[5];
unsigned char sta;


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
  Wire.write(0xB8);
  Wire.endTransmission();
  delay(30);
  Wire.beginTransmission(MPLADDRESS);
  Wire.write(0x13);
  Wire.write(0x07);
  Wire.endTransmission();
  delay(30);
  Wire.beginTransmission(MPLADDRESS);
  Wire.write(0x26);
  Wire.write(0xB9);
  Wire.endTransmission();
  delay(30);
}

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  delay(3000);
  Serial.println("Setup");
  setupMPL();

}

void loop() {
  hum_measurement_req();
  Wire.requestFrom(HIH7120ADDRESS, 4);
  measureHIH7120();
  Serial.println("Reading");
  Serial.print("Humidity: ");
  Serial.println(humidity());
  Serial.print("Temperature: ");
  Serial.println(temp());
  delay(1000);
}