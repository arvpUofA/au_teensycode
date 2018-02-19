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
//test comment

//pin definitions
#define TORPEDO_0 16
#define TORPEDO_1 17

//gas canister pulse duration in us
#define PULSE_DURATION 250000

bool firingStatus0 = true;
bool firingStatus1 = true;

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(PCA9685_BASEADDR);

IntervalTimer fireTimer0;
IntervalTimer fireTimer1;

float sinWaveTime = 0;
float sinWave0 = 0;
float sinWave120 = 0;
float sinWave240 = 0;

IntervalTimer sinWaveTimer;

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

void fireTorpdeo(uint8_t trp)
{
    

    if((trp == TORPEDO_0) & !digitalRead(TORPEDO_0) & firingStatus0)
    {
        fireTimer0.begin(trpControl0, PULSE_DURATION);
    }
    else if((trp == TORPEDO_1) & !digitalRead(TORPEDO_1) & firingStatus1)
    {
        fireTimer1.begin(trpControl1, PULSE_DURATION);
    }
    else if((trp == TORPEDO_0) & !digitalRead(TORPEDO_0) & !firingStatus0)
    {
      fireTimer0.end();
    }
    else if((trp == TORPEDO_1) & !digitalRead(TORPEDO_1) & !firingStatus1)
    {
      fireTimer1.end();
    }
    
}

void stepSinWave()
{
  sinWave0 = sin(sinWaveTime);
  sinWave120 = sin(sinWaveTime + 3.14/1.5);
  sinWave240 = sin(sinWaveTime - 3.14/1.5);
  sinWaveTime = sinWaveTime + 0.01;
}

int t = 0;

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