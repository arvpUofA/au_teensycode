#ifndef SENSOR_FUNCTIONS_H
#define SENSOR_FUNCTIONS_H

#include "pressure.h"

// humidity/temperature sensor
#define HIH7120ADDRESS 0x27

// pressure/temperature sensor
#define MPLADDRESS 0x60

#define SAMPLES_PER_SECOND 10


float humidity(void);
float temp(void);
uint32_t pressure(void);

void measureHIH7120(void);
void setupMPL(void);
pressure_StructDef readPressureMPL(void);

float publishHumidity(void);
float publishTemp(void);
float publishPress(void);

#endif
