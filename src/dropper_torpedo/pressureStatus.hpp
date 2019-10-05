#ifndef PRESSURE_STATUS_HPP
#define PRESSURE_STATUS_HPP

#include "parameter.hpp"

enum pState {NORMAL, DROPPING} pressureState = NORMAL;

double initialPressure = 0;

void checkPressure(double pressure)
{
    if(!initialPressure)
    {
        initialPressure = pressure;
        return;
    }
    else if((initialPressure - pressure) >= boardConfig[PARAM_INDEX_LOW_PRESSURE_THRESHOLD].paramValue)
    {
        pressureState = DROPPING;
    }
    else
    {
        pressureState = NORMAL;
    }
}

void resetAlert(double pressure)
{
    initialPressure = pressure;
    pressureState = NORMAL;
}

#endif