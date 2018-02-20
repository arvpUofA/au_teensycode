#ifndef BATTERY_STATUS_HPP
#define BATTERY_STATUS_HPP

#define BATTERY_ID_MAIN_0 0
#define BATTERY_ID_MOTOR_0 1
#define BATTERY_ID_MOTOR_1 2
#define BATTERY_ID_MOTOR_2 3

#define NUMBER_OF_BATTERIES 4

float batteryVoltage[NUMBER_OF_BATTERIES] = {0, 0, 0, 0};

enum voltageState {GOOD, POOR, DANGER};

//Provides simple voltage data storage interface for uavcan callbacks.
void storeVoltageInfo(size_t index, float value)
{
    if(index < NUMBER_OF_BATTERIES) batteryVoltage[index] = value;
}

//Checks all voltage values in batteryVoltage array and returns state of worst battery
voltageState checkVoltages(float lowVolt, float criticalVolt)
{
    for(uint8_t i = 0; i < sizeof(batteryVoltage)/sizeof(float); i++)
    {
        if(batteryVoltage[i] <= criticalVolt)
        {
            return DANGER;
        }
        else if(batteryVoltage[i] <= lowVolt)
        {
            return POOR;
        }
    }
    return GOOD;
}

#endif