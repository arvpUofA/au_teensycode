#include <arvp/BatteryLevelling.hpp>
#include <uavcan/equipment/power/BatteryInfo.hpp>

#include <uavcanNodeIDs.h>
#include <teensy_uavcan.hpp>
#include <watchdog.h>

#include "batt_levelling.h"

static const char *nodename = "org.arvp.batterylevelling";

static Subscriber<equipment::power::BatteryInfo> *batt_subscriber;
static Publisher<arvp::BatteryLevelling> *batt_level_publisher;

static int ee_batt_state;
static int motor_batt_state;
static float voltages[NUM_OF_BATTERIES];

/* swaps batt src and updates src field */
static void
batt_swap(void)
{
	switch (ee_batt_state) {
	case 0:
		digitalWrite(MOTOR_BATT_CONT, ~motor_batt_state);
		digitalWrite(EE_BATT_CONT, ~ee_batt_state);
		break;
	case 1:
		digitalWrite(EE_BATT_CONT, ~ee_batt_state);
		digitalWrite(MOTOR_BATT_CONT, ~motor_batt_state);
	}

	ee_batt_state = ~ee_batt_state;
	motor_batt_state = ~motor_batt_state;
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
publish_levelling_info(void)
{
	arvp::BatteryLevelling levelling_info;

	levelling_info.ee_batt_state = ee_batt_state;
	levelling_info.motor_batt_state = motor_batt_state;

	if (batt_level_publisher->broadcast(levelling_info) < 0)
		Serial.println("Failed to publish Battery Levelling info");
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
	batt_level_publisher = new Publisher<arvp::BatteryLevelling>(*node);

	if (batt_level_publisher->init() < 0)
		Serial.println("Failed to start Battery Levelling publisher");

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
	initNode(node, UAVCAN_NODE_ID_BATTERY_LEVELLING_BOARD, nodename, SW_VERSION, HW_VERSION);

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

	/* make sure ee batt is starting voltage src */
	if (!ee_batt_state) {
		Serial.println("Swapping to EE battery");
		batt_swap();
	}

	Serial.println("Setup Complete");
}

void
loop(void)
{
	int i;

	delay(500);

	publish_levelling_info();

	if (voltages[0] > EE_MIN_VOLTAGE)
		return;

	for (i = 1; i < NUM_OF_BATTERIES; i++) {
		if (voltcmp(voltages[0], voltages[i]))
			batt_swap();
	}
}