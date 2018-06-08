#define BOARD_SELECT_INTERNAL_ENV_BOARD

#include <Arduino.h>
#include <internalEnvBoard.hpp>
#include <Metro.h>

#include <uavcanNodeIDs.h>
#include <watchdog.h>

// UAVCAN Node settings
static constexpr uint32_t nodeID = UAVCAN_NODE_ID_INTERNAL_SENSOR_BOARD;
static constexpr uint8_t swVersion = 1;
static constexpr uint8_t hwVersion = 1;
static const char *nodeName = "org.arvp.internalSensor";

// instantiate the timer for reading values/publishing message
// interval in milliseconds
Metro timer = Metro(100);
uint8_t timerCounter;

// re-instantiate (for code clarity) averaging classes
extern Running_Average<uint32_t, 10> avg_pressure;
extern Running_Average<float, 10> avg_temperature;
extern Running_Average<float, 10> avg_humidity;

void setup() {
    Wire.begin();

    initLeds();

    // ensure counter starts at 0
    timerCounter = 0;

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
      timerCounter++;

      // read humidity and temperature
      measureHIH7120();
      avg_humidity.AddSample(humidity());
      avg_temperature.AddSample(temp());

      // read pressure
      readPressureMPL();
      avg_pressure.AddSample(pressure());

      // publish once every second
      if (timerCounter == 10) {
        cyclePublisher();
        Serial.println("Reading...");
        Serial.print("Humidity: ");
        Serial.println(avg_humidity.Average());
        Serial.print("Temperature: ");
        Serial.println(avg_temperature.Average());
        Serial.print("Pressure: ");
        Serial.println(avg_pressure.Average());

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
