#include <arvp/BatteryLeveling.hpp>
#include <uavcan/equipment/power/BatteryInfo.hpp>

#include <uavcanNodeIDs.h>
#include <teensy_uavcan.hpp>
#include <watchdog.h>

#include "batt_leveling.h"

static const char *nodename = "org.arvp.batteryleveling";

static Subscriber<equipment::power::BatteryInfo> *batt_subscriber;
static Publisher<arvp::BatteryLeveling> *batt_level_publisher;

static int ee_batt_state;
static int motor_batt_state;
static float voltages[NUM_OF_BATTERIES];

/* swaps batt src and updates src field */
static void
batt_swap(void)
{
	ee_batt_state = ~ee_batt_state;
	motor_batt_state = ~motor_batt_state;

	digitalWrite(MOTOR_BATT_CONT, motor_batt_state);
	digitalWrite(EE_BATT_CONT, ee_batt_state);
}

/* returns 1 if v_1 src and v_2 src should be swapped */
static int
voltcmp(float v_1, float v_2)
{
	return abs(v_1 - v_2) > V_DELTA;
}

/* store voltage level in voltage array */
static void
store_voltage_info(uint8_t i, float f)
{
	voltages[i] = f;
}

/* Callback function for uavcan battery info messages. Stores values into storage array */
static void
batt_info_callback(const uavcan::equipment::power::BatteryInfo& batt_data)
{
	store_voltage_info(batt_data.battery_id, batt_data.voltage);
}

static void
publish_leveling_info(void)
{
	arvp::BatteryLeveling leveling_info;

	leveling_info.ee_batt_state = ee_batt_state;
	leveling_info.motor_batt_state = motor_batt_state;

	if (batt_level_publisher->broadcast(leveling_info) < 0)
		Serial.println("Failed to publish Battery Leveling info");
}

static void 
init_subscriber(Node<NodeMemoryPoolSize> *node)
{
	int i;
	Subscriber<equipment::power::BatteryInfo> *batt_sub = batt_subscriber;
	for (i = 0; i < NUM_OF_BATTERIES; i++) {
		batt_sub = new Subscriber<equipment::power::BatteryInfo>(*node);

		if (batt_sub->start(batt_info_callback) < 0)
			Serial.printf("Battery [%d]: failed to start subscriber\n", i);

		batt_sub++;
	}

}

static void
init_publisher(Node<NodeMemoryPoolSize> *node)
{
	batt_level_publisher = new Publisher<arvp::BatteryLeveling>(*node);

	if (batt_level_publisher->init() < 0)
		Serial.println("Failed to start Battery Leveling publisher");

	batt_level_publisher->setTxTimeout(MonotonicDuration::fromUSec(800));
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

	init_subscriber(node);
	init_publisher(node);

	/* set up filters (must be after publisher & subscriber setup!) */
	configureCanAcceptanceFilters(*node);

	/* start up node */
	node->setModeOperational();

	/* initialize pin states */
	ee_batt_state = digitalRead(EE_BATT_CONT);
	motor_batt_state = digitalRead(MOTOR_BATT_CONT);

	/* make sure the pins start out in opposite states */
	if (ee_batt_state == motor_batt_state) {
		ee_batt_state = ~motor_batt_state;
		digitalWrite(EE_BATT_CONT, ee_batt_state);
	}

	Serial.println("Setup Complete");
}

void
loop(void)
{
	int i;
	for (i = 1; i < NUM_OF_BATTERIES; i++) {
		if (voltcmp(voltages[0], voltages[i]))
			batt_swap();
	}

	publish_leveling_info();
	delay(500);
}