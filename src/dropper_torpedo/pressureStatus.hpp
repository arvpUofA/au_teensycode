#ifndef PRESSURE_STATUS_HPP
#define PRESSURE_STATUS_HPP

#include "parameter.h"

enum pState {NORMAL, DROPPING} pressureState = NORMAL;

double initialPressure = 0;

void checkPressure(double pressure)
{
	if (!initialPressure) {
		initialPressure = pressure;
		return;
	} else if ((initialPressure - pressure) >= boardConfig[LOW_PRESSURE_THRESHOLD].value) {
		pressureState = DROPPING;
	} else {
		pressureState = NORMAL;
	}
}

void resetAlert(double pressure)
{
	initialPressure = pressure;
	pressureState = NORMAL;
}

#endif