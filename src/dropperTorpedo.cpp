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
#include <torpedoControl.hpp>

#include <teensy_uavcan.hpp>
//#include <publisher.hpp>
#include <subscriber.hpp>

// UAVCAN Node settings
static constexpr uint32_t nodeID = 11;
static constexpr uint8_t swVersion = 1;
static constexpr uint8_t hwVersion = 1;
static const char* nodeName = "org.arvp.dropperTorpedo";

// UAVCAN application settings
static constexpr float framerate = 100;



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

  initTorpedos();

  //--UAVCAN init--//

  // init LEDs
  initLeds();

  // Create a node
  systemClock = &getSystemClock();
  canDriver = &getCanDriver();
  node = new Node<NodeMemoryPoolSize>(*canDriver, *systemClock);
  initNode(node, nodeID, nodeName, swVersion, hwVersion);

  // init publisher
  //initPublisher(node);

  // init subscriber
  initSubscriber(node);

  // start up node
  node->setModeOperational();

  delay(10);
}

void loop() 
{
  noInterrupts();
  pwm.setRGB((1+sinWave0)/2, (1+sinWave120)/2, (1+sinWave240)/2, 0, 0.01);
  interrupts();
  
  torpedoRoutine();

  //--UAVCAN cycles--//

  // wait in cycle
  cycleWait(framerate);

  // do some CAN stuff
  cycleNode(node);

  // publish messages
  //cyclePublisher();

  // toggle heartbeat
  toggleHeartBeat();

}