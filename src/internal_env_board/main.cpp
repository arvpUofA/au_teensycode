#include "Arduino.h"
#include <teensy_uavcan.hpp>
#include "publisher.hpp"
#include "running_average.hpp"
#include <uavcanNodeIDs.h>
#include <watchdog.h>
#include <Metro.h>
#include <Wire.h>
#include "sensor_functions.h"
#include "lcd_functions.h"

// UAVCAN application settings
static constexpr float framerate = 100;
extern Running_Average<uint32_t, SAMPLES_PER_SECOND> avg_pressure;
extern Running_Average<float, SAMPLES_PER_SECOND> avg_temperature;
extern Running_Average<float, SAMPLES_PER_SECOND> avg_humidity;

#include "batteryStatus.hpp"
#include "subscriber.hpp"
#include <teensy_uavcan.hpp>

// UAVCAN Node settings
static constexpr uint32_t nodeID = UAVCAN_NODE_ID_INTERNAL_SENSOR_BOARD;
static constexpr uint8_t swVersion = 1;
static constexpr uint8_t hwVersion = 1;
static const char *nodeName = "org.arvp.internalSensor";

// instantiate the timer for reading values/publishing message
// interval in milliseconds
Metro timer = Metro(1000/SAMPLES_PER_SECOND);
uint8_t timerCounter;
uint16_t lcdCounter = 0;
boolean lcdMode = false;


void setup() {
    Wire.begin();

    initLeds();

    // ensure counter starts at 0
    timerCounter = 0;

    Serial.begin(9600);
    delay(1000);
    Serial.println("Setup start");

    setupMPL();

    setup_lcd();
    
    // Create a node
    systemClock = &initSystemClock();
    canDriver = &initCanDriver();
    node = new Node<NodeMemoryPoolSize>(*canDriver, *systemClock);
    initNode(node, nodeID, nodeName, swVersion, hwVersion);

	//init subscriber
	initSubscriber(node);
	
    // init publisher
    initPublisher(node);

    // start up node
    configureCanAcceptanceFilters(*node);
    node->setModeOperational();
    Serial.println("Setup complete"); 
    
}

void loop() {
    KickDog();
    if(timer.check() == 1) {
      timerCounter++;

      // read humidity and temperature
      measureHIH7120();
      avg_humidity.AddSample(humidity());
      avg_temperature.AddSample(temp());

      // read pressure
      readPressureMPL();
      avg_pressure.AddSample(pressure());

      // publish once every second
      if (timerCounter == SAMPLES_PER_SECOND) {
        cyclePublisher();
        Serial.println("Reading...");
        Serial.print("Humidity: ");
        Serial.println(avg_humidity.Average());
        Serial.print("Temperature: ");
        Serial.println(avg_temperature.Average());
        Serial.print("Pressure: ");
        Serial.println(avg_pressure.Average());
		
		//Print out battery voltages

		Serial.print("Battery 1: ");
		Serial.println(batteryVoltage[0]);
		Serial.print("Battery 2: ");
		Serial.println(batteryVoltage[1]);
		Serial.print("Battery 3: ");
		Serial.println(batteryVoltage[2]);
		Serial.print("Battery 4: ");
		Serial.println(batteryVoltage[3]);

        lcdCounter++;

        if(lcdCounter%6 == 0){
            lcdMode = !lcdMode;
        }

        //Alternate data displayed to LCD every six seconds
        if(lcdMode == true){
            display_to_lcd(avg_temperature.Average(), avg_humidity.Average(), avg_pressure.Average());
        } 
        else {
            display_voltages_to_lcd(batteryVoltage[0], batteryVoltage[1], batteryVoltage[2], batteryVoltage[3]);
        }


        timerCounter = 0;
      }
    }

    //--UAVCAN cycles--//
    // wait in cycle
    cycleWait(framerate);

    // do some CAN stuff
    cycleNode(node);

    // toggle heartbeat
    toggleHeartBeat();
}
