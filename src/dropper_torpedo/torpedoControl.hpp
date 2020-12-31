#ifndef TORPEDO_CONTROL_H
#define TORPEDO_CONTROL_H

#include <Arduino.h>
#include <IntervalTimer.h>
#include "ledIndicationControl.hpp"
#include "parameter.h"

// Move to a hardware.h file or something
//Pin definitions
#define TORPEDO_0 16
#define TORPEDO_1 17

//Solenoid pulse duration in us
double pulseDuration = 250000;

//Interval timers for solenoid control. Uses ISRs for highly accurate timing control.
IntervalTimer fireTimer0;
IntervalTimer fireTimer1;

struct Torpedo torpedoes[N_TORPEDOES] = {
	{ TORPEDO_0, READY, false },
	{ TORPEDO_1, READY, false }
};

//Call this function in setup()
void initTorpedos()
{
	uint8_t i;
	for (i = 0; i < LEN(torpedoes); i++) {
		pinMode(torpedoes[i].pin, OUTPUT);
		digitalWrite(torpedoes[i].pin, LOW);
	}
}

//ISR functions. Can't pass parameters to this type of function, so one unique function per torpedo is required.
void trpControl0()
{
	digitalWrite(TORPEDO_0, LOW);
	torpedoes[0].launch = false;
}

void trpControl1()
{
	digitalWrite(TORPEDO_1, LOW);
	torpedoes[1].launch = false;
}

void changeInterval(double inter)
{
	pulseDuration = inter;
	pulseLED(0, 0, 0.2, boardConfig[TORPEDO_PULSE].value);
}

//Call this function to trigger firing of torpedo. Torpedo must be in ARMED state to fire.
void requestLaunch(uint8_t trp)
{
	if ((trp == TORPEDO_0) && !digitalRead(TORPEDO_0) && (torpedoes[0].state == ARMED))
		torpedoes[0].launch = true;
	if ((trp == TORPEDO_1) && !digitalRead(TORPEDO_1) && (torpedoes[1].state == ARMED))
		torpedoes[1].launch = true;
}

//Fires torpedo and starts interval timer for a selected torpedo according to specified solenoid pulse duration
static bool fireTorpedo(uint8_t trp)
{
	if ((trp == TORPEDO_0) && !digitalRead(TORPEDO_0) && torpedoes[0].launch) {
		digitalWrite(TORPEDO_0, HIGH);
		torpedoes[0].launch = false;
		fireTimer0.begin(trpControl0, boardConfig[TORPEDO_PULSE].value * 1000); //ms to us
		//Serial.println("Torpedo 0 pulse timer started");
		return true;
	} else if ((trp == TORPEDO_1) && !digitalRead(TORPEDO_1) && torpedoes[1].launch) {
		digitalWrite(TORPEDO_1, HIGH);
		torpedoes[1].launch = false;
		fireTimer1.begin(trpControl1, boardConfig[TORPEDO_PULSE].value * 1000);
		//Serial.println("Torpedo 1 pulse timer started");
		return true;
	}
	return false;
}

//Cleans up interval timers. Up to 4 interval timers may be active at any time on Teensy 3.2
//Only cleans up when torpedo pins are LOW.
bool cleanUpTorpedoTimer(uint8_t trp)
{
	if ((trp == TORPEDO_0) && !digitalRead(TORPEDO_0) && !torpedoes[0].launch) {
		fireTimer0.end();
		Serial.println("Torpedo 0 pulse timer cleaned up");
		return true;
	} else if ((trp == TORPEDO_1) && !digitalRead(TORPEDO_1) && !torpedoes[1].launch) {
		fireTimer1.end();
		Serial.println("Torpedo 1 pulse timer cleaned up");
		return true;
	}
	return false;
}

//Torpedos are locked-out after firing and upon power-up until this function is called.
void armTorpedo(uint8_t trp)
{
	if ((trp == TORPEDO_0) && ((torpedoes[0].state == DISCHARGED) || (torpedoes[0].state == READY))) {
		torpedoes[0].state = ARMED;
		Serial.println("Torpedo 0 ARMED");
	} else if ((trp == TORPEDO_1) && ((torpedoes[1].state == DISCHARGED) || (torpedoes[1].state == READY))) {
		torpedoes[1].state = ARMED;
		Serial.println("Torpedo 1 ARMED");
	}
}

//Sets torpedo state to READY
void disarmTorpedo(uint8_t trp)
{
	if ((trp == TORPEDO_0) && (torpedoes[0].state == ARMED)) {
		torpedoes[0].state = READY;
		Serial.println("Torpedo 0 disarmed");
	} else if ((trp == TORPEDO_1) && (torpedoes[1].state == ARMED)) {
		torpedoes[1].state = READY;
		Serial.println("Torpedo 1 disarmed");
	}
}

//Call this function in loop() to run torpedo system
void torpedoRoutine()
{
	uint8_t i;
	for (i = 0; i < LEN(torpedoes); i++) {
		switch (torpedoes[i].state) {
		case READY:
			if (torpedoes[i].launch) {
				Serial.printf("Torpedo %d: Not ARMED. Unable to fire.\n", i);
				torpedoes[i].launch = ~torpedoes[i].launch;
			}
			continue;
		case ARMED:
			if (torpedoes[i].launch) {
				Serial.printf("Torpedo %d: FIRING\n", i);
				fireTorpedo(torpedoes[i].pin);
				torpedoes[i].state = FIRING;
			}
			continue;
		case FIRING:
			if (cleanUpTorpedoTimer(torpedoes[i].pin)) {
				Serial.printf("Torpedo %d: DISCHARGED\n", i);
				torpedoes[i].state = READY;
			}
			continue;
		case DISCHARGED: /* FIXME: unused right now could be deleted */
			if (torpedoes[i].launch) {
				Serial.printf("Torpedo %d: Unable to fire. Torpedo in DISCHARGED state.\n", i);
				torpedoes[i].launch = ~torpedoes[i].launch;
			}
		}
	}
}

#endif