#ifndef TORPEDO_CONTROL_H
#define TORPEDO_CONTROL_H

#include <Arduino.h>
#include <IntervalTimer.h>
#include "ledIndicationControl.hpp"
#include "parameter.hpp"

//Pin definitions
#define TORPEDO_0 16
#define TORPEDO_1 17

//Solenoid pulse duration in us
double pulseDuration = 250000;

//Set to true when torpedo firing request is initiated
bool launchRequest0 = false;
bool launchRequest1 = false;

//Interval timers for solenoid control. Uses ISRs for highly accurate timing control.
IntervalTimer fireTimer0;
IntervalTimer fireTimer1;

//Torpedo state definitions
enum torpedoState {READY, ARMED, FIRING, DISCHARGED};

torpedoState torpedoState0 = READY;
torpedoState torpedoState1 = READY;

//Call this function in setup()
void initTorpedos()
{
    pinMode(TORPEDO_0, OUTPUT);
    pinMode(TORPEDO_1, OUTPUT);
    digitalWrite(TORPEDO_0, LOW);
    digitalWrite(TORPEDO_1, LOW);
}

//ISR functions. Can't pass parameters to this type of function, so one unique function per torpedo is required.
void trpControl0()
{
    digitalWrite(TORPEDO_0, LOW);
    launchRequest0 = false;
}

void trpControl1()
{
    digitalWrite(TORPEDO_1, LOW);
    launchRequest1 = false;
}

void changeInterval(double inter)
{
    pulseDuration = inter;
    pulseLED(0, 0, 0.2, boardConfig[PARAM_INDEX_TORPEDO_PULSE].paramValue);
}

//Call this function to trigger firing of torpedo. Torpedo must be in ARMED state to fire.
void requestLaunch(uint8_t trp)
{
    if((trp == TORPEDO_0) && !digitalRead(TORPEDO_0) && (torpedoState0 == ARMED))
    {
        launchRequest0 = true;
    }
    if((trp == TORPEDO_1) && !digitalRead(TORPEDO_1) && (torpedoState1 == ARMED))
    {
        launchRequest1 = true;
    }
}

//Fires torpedo and starts interval timer for a selected torpedo according to specified solenoid pulse duration
bool fireTorpdeo(uint8_t trp)
{  
    if((trp == TORPEDO_0) && !digitalRead(TORPEDO_0) && launchRequest0)
    {
        digitalWrite(TORPEDO_0, HIGH);
        launchRequest0 = false;
        fireTimer0.begin(trpControl0, boardConfig[PARAM_INDEX_TORPEDO_PULSE].paramValue*1000); //ms to us
        //Serial.println("Torpedo 0 pulse timer started");
        return true;
    }
    else if((trp == TORPEDO_1) && !digitalRead(TORPEDO_1) && launchRequest1)
    {
        digitalWrite(TORPEDO_1, HIGH);
        launchRequest1 = false;
        fireTimer1.begin(trpControl1, boardConfig[PARAM_INDEX_TORPEDO_PULSE].paramValue*1000);
        //Serial.println("Torpedo 1 pulse timer started");
        return true;
    }
    return false;
}

//Cleans up interval timers. Up to 4 interval timers may be active at any time on Teensy 3.2
//Only cleans up when torpedo pins are LOW.
bool cleanUpTorpedoTimer(uint8_t trp)
{
    if((trp == TORPEDO_0) && !digitalRead(TORPEDO_0) && !launchRequest0)
    {
      fireTimer0.end();
      Serial.println("Torpedo 0 pulse timer cleaned up");
      return true;
    }
    else if((trp == TORPEDO_1) && !digitalRead(TORPEDO_1) && !launchRequest1)
    {
      fireTimer1.end();
      Serial.println("Torpedo 1 pulse timer cleaned up");
      return true;
    }
    return false;
}

//Torpedos are locked-out after firing and upon power-up until this function is called.
void armTorpedo(uint8_t trp)
{
    if ((trp == TORPEDO_0) && ((torpedoState0 == DISCHARGED) || (torpedoState0 == READY)))
    {
        torpedoState0 = ARMED;
        Serial.println("Torpedo 0 ARMED");
    }
    else if ((trp == TORPEDO_1) && ((torpedoState1 == DISCHARGED) || (torpedoState1 == READY)))
    {
        torpedoState1 = ARMED;
        Serial.println("Torpedo 1 ARMED");
    }
}

//Sets torpedo state to READY
void disarmTorpedo(uint8_t trp)
{
    if((trp == TORPEDO_0) && (torpedoState0 == ARMED))
    {
        torpedoState0 = READY;
        Serial.println("Torpedo 0 disarmed");
    }
    else if((trp == TORPEDO_1) && (torpedoState1 == ARMED))
    {
        torpedoState1 = READY;
        Serial.println("Torpedo 1 disarmed");
    }
}

//Call this function in loop() to run torpedo system
void torpedoRoutine()
{
    switch(torpedoState0)
    {
        case READY:
            if(launchRequest0)
            {
                Serial.println("Unable to fire. Torpedo 0 not yet ARMED");
                launchRequest0 = false;
            }
            break;

        case ARMED:
            if(launchRequest0)
            {
                fireTorpdeo(TORPEDO_0);
                Serial.println("Torpedo 0 FIRING");
                torpedoState0 = FIRING;
            }
            break;

        case FIRING:
            if(cleanUpTorpedoTimer(TORPEDO_0))
            {
                torpedoState0 = READY;
                Serial.println("Torpedo 0 DISCHARGED");
            }
            break;
        
        case DISCHARGED: //Not really used at the moment, could delete this.
            if(launchRequest0)
            {
                Serial.println("Unable to fire. Torpedo 0 in DISCHARGED state");
                launchRequest0 = false;
            }
            break;
    }

    switch(torpedoState1)
    {
        case READY:
            if(launchRequest1)
            {
                Serial.println("Unable to fire. Torpedo 1 not yet ARMED");
                launchRequest0 = false;
            }
            break;

        case ARMED:
            if(launchRequest1)
            {
                fireTorpdeo(TORPEDO_1);
                Serial.println("Torpedo 1 FIRING");
                torpedoState1 = FIRING;
            }
            break;

        case FIRING:
            if(cleanUpTorpedoTimer(TORPEDO_1))
            {
                torpedoState1 = READY;
                Serial.println("Torpedo 1 DISCHARGED");
            }
            break;
        
        case DISCHARGED:
            if(launchRequest1)
            {
                Serial.println("Unable to fire. Torpedo 1 in DISCHARGED state");
                launchRequest1 = false;
            }
            break;
    }
}

#endif