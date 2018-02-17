/* 

shot power of torpedo controlled by opening valve for set amount of time
maybe can adjust this?

tropedo function
    - tropedo number can be selected
    - configure delays (paramter)

rgb leds
    - use adafruit library
    - refer to schematic for channel numbers (pwm)
    - take value between 0 and 100 for each rgb channel

servo
    - input is servo number (1 to 12)
    - configure angle

*/

// #include "arduino.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// this is the 'minimum' and 'maximum' pulse length count (out of 4096)
#define SERVOMIN  150 /
#define SERVOMAX  600 /

// our servo # counter
uint8_t servonum = 0;

// this runs once to setup everything
void setup() 
{
  Serial.begin(9600);
  Serial.println("8 channel Servo test!");
  pwm.begin();

  // Analog servos run at ~50 Hz updates
  pwm.setPWMFreq(50);  

  delay(10);
}

// this can set pulse length (seconds)
void setServoPulse(uint8_t n, double pulse) 
{
  double pulselength;
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 50;   // 50 Hz
  Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse *= 1000000;  // convert to us
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}

// runs continuously =
void loop() {
  // Drive each servo one at a time
  /*Serial.println(servonum);
  for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
    pwm.setPWM(servonum, 0, pulselen);
  }

  delay(500);
  for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
    pwm.setPWM(servonum, 0, pulselen);
  }

  delay(500);

  servonum ++;
  if (servonum > 7) servonum = 0;*/

  // moves servo 90 degrees
  setServoPulse(0, 0.0016);
  delay(1000);
  // 0 degrees
  setServoPulse(0, 0.0006);
  delay(1000);
  // 180 degrees
  setServoPulse(0, 0.0026);
  delay(1000);
}