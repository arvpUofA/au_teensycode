#include "Arduino.h"
#include <Wire.h>
#include <Metro.h>

#include <teensy_uavcan.hpp>
#include <publisher.hpp>
#include <uavcanNodeIDs.h>
#include "internalEnvBoard.h"

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

// instantiate the timer for publishing message
Metro timer = Metro(1000);

//use this for reading out pressure data in two parts
struct pressure_StructDef {
  uint32_t whole;
  uint8_t fractional;
};

pressure_StructDef pressure;

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
    readPressureMPL();
    return pressure.whole;
}

void setup() {
    Wire.begin();

    initLeds();

    Serial.begin(9600);
    delay(2000);
    Serial.println("Setup start");
    
    setupMPL();

    // Create a node
    systemClock = &getSystemClock();
    canDriver = &getCanDriver();
    node = new Node<NodeMemoryPoolSize>(*canDriver, *systemClock);
    initNode(node, nodeID, nodeName, swVersion, hwVersion);

    // init subscriber
    initPublisher(node);

    // start up node
    node->setModeOperational();
    Serial.println("Setup complete");
}

void loop() {

    if(timer.check() == 1) {
      cyclePublisher();
      hum_measurement_req();
      Wire.requestFrom(HIH7120ADDRESS, 4);
      measureHIH7120();
      Serial.println("Reading");
      Serial.print("Humidity: ");
      Serial.println(humidity());
      Serial.print("Temperature: ");
      Serial.println(temp());

      //pressure readings
      readPressureMPL();
      Serial.print("Pressure value: ");
      Serial.print(pressure.whole);
      Serial.print(".");
      Serial.println(pressure.fractional);
    }

    //--UAVCAN cycles--//
    // wait in cycle
    cycleWait(framerate);

    // do some CAN stuff
    cycleNode(node);

    // toggle heartbeat
    toggleHeartBeat();   
}
