#ifndef BATTERY_STATUS_HPP
#define BATTERY_STATUS_HPP

#define BATTERY_ID_MAIN_0 0
#define BATTERY_ID_MOTOR_0 1
#define BATTERY_ID_MOTOR_1 2
#define BATTERY_ID_MOTOR_2 3

#define NUMBER_OF_BATTERIES 4

#define ENABLE_BATT_DEBUG false

float batteryVoltage[NUMBER_OF_BATTERIES] = {15, 15, 15, 15}; //default set to 15 volts

enum voltageState {GOOD, POOR, DANGER};

//Provides simple voltage data storage interface for uavcan callbacks.
void storeVoltageInfo(size_t index, float value)
{
	if (index < NUMBER_OF_BATTERIES) batteryVoltage[index] = value;
	if (ENABLE_BATT_DEBUG) {
		Serial.print("Received battery message: ");
		Serial.print(index);
		Serial.print(" : ");
		Serial.println(value);
	}
}

//Checks all voltage values in batteryVoltage array and returns state of worst battery
voltageState checkVoltages(float lowVolt, float criticalVolt)
{
	for (uint8_t i = 0; i < sizeof(batteryVoltage)/sizeof(float); i++) {
		if (batteryVoltage[i] <= criticalVolt)
			return DANGER;
	}
	for (uint8_t i = 0; i < sizeof(batteryVoltage)/sizeof(float); i++) {
		if (batteryVoltage[i] <= lowVolt)
			return POOR;
	}
	return GOOD;
}

void displayVoltages()
{
	Serial.print("Battery 0:");
	Serial.println(batteryVoltage[0]);
	Serial.print("Battery 1:");
	Serial.println(batteryVoltage[1]);
	Serial.print("Battery 2:");
	Serial.println(batteryVoltage[2]);
	Serial.print("Battery 3:");
	Serial.println(batteryVoltage[3]);
}

#endif