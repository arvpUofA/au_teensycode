#include <uavcan/equipment/power/BatteryInfo.hpp>

#include <uavcanNodeIDs.h>
#include <teensy_uavcan.hpp>
#include <watchdog.h>

#include "batt_leveling.h"

#define NUM_OF_BATTERIES 4

void storeVoltageInfo(uint8_t i, float f) {};

static const char *nodename = "org.arvp.batteryleveling";

static Subscriber<equipment::power::BatteryInfo> *battSubscriber;
static Publisher<equipment::power::BatteryInfo> *battPublisher;

/* Callback function for uavcan battery info messages. Stores values into storage array */
static void
batteryInfoCallback(const uavcan::equipment::power::BatteryInfo& batteryData)
{
	storeVoltageInfo(batteryData.battery_id, batteryData.voltage);
}

static void 
initSubscriber(Node<NodeMemoryPoolSize> *node)
{
	battSubscriber = new Subscriber<equipment::power::BatteryInfo>(*node);

	if (battSubscriber->start(batteryInfoCallback) < 0)
		Serial.println("Unable to start battery subscriber");
}

static void
initPublisher(Node<NodeMemoryPoolSize> *node)
{
	int i;
	Publisher<equipment::power::BatteryInfo> *batt_info = battPublisher;

	for (i = 0; i < NUM_OF_BATTERIES; i++) {
		batt_info = new Publisher<equipment::power::BatteryInfo>(*node);

        	if (batt_info->init() < 0)
			Serial.println("Unable to initialize battery message publisher!");

		batt_info->setTxTimeout(MonotonicDuration::fromUSec(800));

		batt_info++;
	}
}


void
setup(void)
{
	KickDog();
	Serial.begin(9600);
	Serial.println("Setup start");

	/* Create a node */
	systemClock = &initSystemClock();
	canDriver = &initCanDriver();
	node = new Node<NodeMemoryPoolSize>(*canDriver, *systemClock);
	initNode(node, UAVCAN_NODE_ID_BATTERY_LEVELING_BOARD, nodename, SW_VERSION, HW_VERSION);

	initSubscriber(node);
	initPublisher(node);

	/* set up filters (must be after publisher & subscriber setup!) */
	configureCanAcceptanceFilters(*node);

	/* start up node */
	node->setModeOperational();

	/* start parameter server */
	Serial.println("Setup Complete");
}

void
loop(void)
{
}