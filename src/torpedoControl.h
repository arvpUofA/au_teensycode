#ifndef TORPEDO_CONTROL_H
#define TORPEDO_CONTROL_H

#include <Arduino.h>
#include <IntervalTimer.h>

//pin definitions
#define TORPEDO_0 16
#define TORPEDO_1 17

//gas canister pulse duration in us
#define PULSE_DURATION 250000

bool firingStatus0 = true;
bool firingStatus1 = true;

IntervalTimer fireTimer0;
IntervalTimer fireTimer1;

void trpControl0()
{
    digitalWrite(TORPEDO_0, firingStatus0);
    firingStatus0 = false;
}

void trpControl1()
{
    digitalWrite(TORPEDO_1, firingStatus1);
    firingStatus1 = false;
}

void armTorpedo(uint8_t trp)
{
    if((trp == TORPEDO_0)& !digitalRead(TORPEDO_0))
    {
        firingStatus0 = true;
    }
    if((trp == TORPEDO_1)& !digitalRead(TORPEDO_1))
    {
        firingStatus1 = true;
    }
}

bool fireTorpdeo(uint8_t trp)
{  
    if((trp == TORPEDO_0) & !digitalRead(TORPEDO_0) & firingStatus0)
    {
        fireTimer0.begin(trpControl0, PULSE_DURATION);
        return true;
    }
    else if((trp == TORPEDO_1) & !digitalRead(TORPEDO_1) & firingStatus1)
    {
        fireTimer1.begin(trpControl1, PULSE_DURATION);
        return true;
    }
    return false;
}

bool cleanUpTorpedoTimer(uint8_t trp)
{
    if((trp == TORPEDO_0) & !digitalRead(TORPEDO_0) & !firingStatus0)
    {
      fireTimer0.end();
      return true;
    }
    else if((trp == TORPEDO_1) & !digitalRead(TORPEDO_1) & !firingStatus1)
    {
      fireTimer1.end();
      return true;
    }
    return false;
}

#endif