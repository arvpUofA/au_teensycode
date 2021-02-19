#include <libARVPpwm.h>
#include <IntervalTimer.h>
#include "torpedoControl.hpp"
#include "batteryStatus.hpp"
#include "servoControl.hpp"
#include "ledIndicationControl.hpp"
#include <Metro.h>
#include <teensy_uavcan.hpp>
#include "subscriber.hpp"
#include "parameter.hpp"
#include <uavcanNodeIDs.h>
#include <watchdog.h>
#include <Cmd.h>
#include "serialCommand.hpp"
#include "pressureStatus.hpp"
#include <math.h>

#define BATTERY_VOLTAGE_POOR_VALUE 13.25
#define BATTERY_VOLTAGE_DANGER_VALUE 12.75

#define DEFAULT_STROBE_INTERVAL 250

// UAVCAN Node settings
static constexpr uint32_t nodeID = UAVCAN_NODE_ID_TORPEDO_BOARD;
static constexpr uint8_t swVersion = 1;
static constexpr uint8_t hwVersion = 1;
static const char* nodeName = "org.arvp.dropperTorpedo";

// UAVCAN application settings
static constexpr float framerate = 1000;

//PCA9685 object, uses default address 0x40
Adafruit_PWMServoDriver pwmDriver = Adafruit_PWMServoDriver(PCA9685_BASEADDR);

//strobe light timer
Metro strobeLightTimer = Metro(DEFAULT_STROBE_INTERVAL);

//Sin wave generator definitions
const int numberOfSinTableEntries = 1000;
const int sinWaveAmplitude = 4096;
int baseSinFrq = 1; //Hz

bool demoMode = false;

int theta = 0;
float sinWave0 = 0;
float sinWave120 = 0;
float sinWave180 = 0;
float sinWave240 = 0;

IntervalTimer sinWaveTimer;

void stepSinWave()
{
	sinWave0 = 2048*sin(theta)+2048;
	sinWave120 = 2048*sin(theta + 2*PI/3)+2048;
	sinWave180 = 2048*sin(theta + PI)+2048;
	sinWave240 = 2048*sin(theta + 4*PI/3)+2048;
	theta += (PI/500); //The PI/500 increment matched the preview sinWaveTable
}

void indicatorRoutine() //Add this function to loop() to allow for indication of torpedo and battery status
{
	if (pressureState == DROPPING && boardConfig[ENABLE_PRESSURE_ALERT].value) {
		disableExternalLEDControl();
		sinWaveTimer.begin(stepSinWave, 210);
		pwmDriver.setRGB(1, 1, 1, 0, 0.1*sinWave0/sinWaveAmplitude);
		return;
	}

	if (boardConfig[ENABLE_TORPEDO_INDICATOR].value) {
		if ((torpedoes[0].state == FIRING) || (torpedoes[1].state == FIRING)) {
			disableExternalLEDControl();
			pwmDriver.setRGB(1, 0, 0, 0, 0.5);
			return;
		}
		if ((torpedoes[0].state == ARMED) && (torpedoes[1].state == ARMED)) {
			disableExternalLEDControl();
			sinWaveTimer.begin(stepSinWave, 1000);
			pwmDriver.setRGB(sinWave0/sinWaveAmplitude, 0.1*sinWave180/sinWaveAmplitude, 0.1*sinWave180/sinWaveAmplitude, 0, 0.2);
			return;
		}
		if (torpedoes[0].state == ARMED) {
			disableExternalLEDControl();
			sinWaveTimer.begin(stepSinWave, 1000);
			pwmDriver.setRGB(sinWave0/sinWaveAmplitude, 0.1*sinWave180/sinWaveAmplitude, 0, 0, 0.2);
			return;
		}
		if (torpedoes[1].state == ARMED) {
			disableExternalLEDControl();
			sinWaveTimer.begin(stepSinWave, 1000);
			pwmDriver.setRGB(sinWave0/sinWaveAmplitude, 0, 0.1*sinWave180/sinWaveAmplitude, 0, 0.2);
			return;
		}
	}

	if (checkVoltages(BATTERY_VOLTAGE_POOR_VALUE, BATTERY_VOLTAGE_DANGER_VALUE) == DANGER && (int)boardConfig[ENABLE_LOW_VOLT_INDICATOR].value) {
		//Serial.println("POOR");
		disableExternalLEDControl();
		sinWaveTimer.begin(stepSinWave, 200);
		pwmDriver.setRGB(1, 0, 0, 0, 0.25*sinWave0/sinWaveAmplitude);
		return;
	}
	if (checkVoltages(BATTERY_VOLTAGE_POOR_VALUE, BATTERY_VOLTAGE_DANGER_VALUE) == POOR && (int)boardConfig[ENABLE_LOW_VOLT_INDICATOR].value) {
		//Serial.println("DANGER");
		disableExternalLEDControl();
		sinWaveTimer.begin(stepSinWave, 2000);
		pwmDriver.setRGB(1, 0, 0, 0, 0.25*sinWave0/sinWaveAmplitude);
		return;
	}

	if ((int)boardConfig[DEMO_MODE].value) {
		disableExternalLEDControl();
		sinWaveTimer.begin(stepSinWave, 2000);
		pwmDriver.setRGB(sinWave0/sinWaveAmplitude, sinWave120/sinWaveAmplitude/4, sinWave240/sinWaveAmplitude/6, 0, 0.125);
		return;
	} else {
		if (enableExternalLEDControl()) {//Only turn off LED if external LED control is disabled
			pwmDriver.setRGB(0, 0, 0, 0, 0);
			sinWaveTimer.end();
		}
		if (strobeActivated && strobeLightTimer.check()) {
			pwmDriver.setRGB(0, 0, 0, 0, 0);
			strobeActivated = false;
		}
	}
}

// this runs once to setup everything
void setup() 
{
	KickDog();
	Serial.begin(9600);
	Serial.println("Setup start");

	cmdInit(&Serial);
	serialCMDInitCommands();

	pwmDriver.begin();

	initTorpedos();
	initLEDControl(&pwmDriver, &strobeLightTimer);

	//--UAVCAN init--//
	// init LEDs
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
	initParameter(node);
	initServoControl(&pwmDriver);
	Serial.println("Setup Complete");
}

//Runs continuously
void loop() 
{
	KickDog();
	indicatorRoutine();
	torpedoRoutine();

	cmdPoll();

	//--UAVCAN cycles--//
	// wait in cycle
	cycleWait(framerate);

	// do some CAN stuff
	cycleNode(node);

	// toggle heartbeat
	toggleHeartBeat();
}