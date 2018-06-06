#define BOARD_SELECT_INTERNAL_ENV_BOARD

#include <Arduino.h>
#include <internalEnvBoard.hpp>

pressure_StructDef pressure;



void setup() {
    Wire.begin();

    initLeds();

    Serial.begin(9600);
    delay(1000);
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
    KickDog();
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
      pressure = readPressureMPL();
      Serial.print("Pressure value: ");
      Serial.print(pressure.whole);
      Serial.print(".");
      Serial.println(pressure.fractional);
      Serial.print("Average pressure value: ");
      Serial.println(avg_pressure.Average());
    }

    //--UAVCAN cycles--//
    // wait in cycle
    cycleWait(framerate);

    // do some CAN stuff
    cycleNode(node);

    // toggle heartbeat
    toggleHeartBeat();
}
