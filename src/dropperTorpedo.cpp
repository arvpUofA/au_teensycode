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

#include <libARVPpwm.h>
#include <IntervalTimer.h>
#include <torpedoControl.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(PCA9685_BASEADDR);

float sinWaveTime = 0;
float sinWave0 = 0;
float sinWave120 = 0;
float sinWave240 = 0;

IntervalTimer sinWaveTimer;



void stepSinWave()
{
  sinWave0 = sin(sinWaveTime);
  sinWave120 = sin(sinWaveTime + 3.14/1.5);
  sinWave240 = sin(sinWaveTime - 3.14/1.5);
  sinWaveTime = sinWaveTime + 0.01;
}

// this runs once to setup everything
void setup() 
{
  Serial.begin(9600);
  Serial.println("8 channel Servo test!");
  pwm.begin();
  sinWaveTimer.begin(stepSinWave, 1000);

  pinMode(TORPEDO_0, OUTPUT);
  pinMode(TORPEDO_1, OUTPUT);

  fireTorpdeo(TORPEDO_0);
    fireTorpdeo(TORPEDO_1);

  delay(10);
}

void loop() 
{
  noInterrupts();
  pwm.setRGB((1+sinWave0)/2, (1+sinWave120)/2, (1+sinWave240)/2, 0, 0.01);
  interrupts();
  delay(1);

}