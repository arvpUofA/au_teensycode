#include <libARVPpwm.h>
#include <IntervalTimer.h>
#include "servoControl.hpp"
#include <Metro.h>
#include <teensy_uavcan.hpp>
#include "subscriber.hpp"
#include <uavcanNodeIDs.h>
#include <Cmd.h>

// UAVCAN Node settings
static constexpr uint32_t nodeID = UAVCAN_NODE_ID_TORPEDO_BOARD;
static constexpr uint8_t swVersion = 1;
static constexpr uint8_t hwVersion = 1;
static const char* nodeName = "org.arvp.dropperTorpedo";

// UAVCAN application settings
static constexpr float framerate = 1000;

//PCA9685 object, uses default address 0x40
Adafruit_PWMServoDriver pwmDriver = Adafruit_PWMServoDriver(PCA9685_BASEADDR);

uint32_t last;

// this runs once to setup everything
void setup() 
{
	Serial.begin(9600);
	Serial.println("Setup start");

	pwmDriver.begin();

	//--UAVCAN init--//
    initLeds();
	// Create a node
	systemClock = &initSystemClock();
	canDriver = &initCanDriver();
	node = new Node<NodeMemoryPoolSize>(*canDriver, *systemClock);
	initNode(node, nodeID, nodeName, swVersion, hwVersion);

	// init subscriber
	initSubscriber(node);

	// set up filters (must be after publisher & subscriber setup!)
	configureCanAcceptanceFilters(*node);

	// start up node
	node->setModeOperational();

	// start parameter server
	initServoControl(&pwmDriver);
	Serial.println("Setup Complete");

    last = millis();
}


//Runs continuously
void loop() 
{
    Serial.println("loop begin");
	//--UAVCAN cycles--//
	// wait in cycle
    delay(500);

	// do some CAN stuff
    Serial.println("Node cycle");
	cycleNode(node);

	// toggle heartbeat
    Serial.println("Heartbeat");
	toggleHeartBeat();

    if (millis() - last > 1000) {
        Serial.println("wbduiawbdwiua");
        last = millis();
    }
    Serial.println("loop");
}