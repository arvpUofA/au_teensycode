#ifndef	INTERNAL_ENV_BOARD_H
#define INTERNAL_ENV_BOARD_H


#include "Arduino.h"
#include <Wire.h>

#include <internalEnvBoard/teensy_uavcan.hpp>
#include <internalEnvBoard/publisher.hpp>
#include <running_average.hpp>
#include "pressure.h"

#define framerate 100

float publishHumidity();
float publishTemp();
float publishPress();

#endif
