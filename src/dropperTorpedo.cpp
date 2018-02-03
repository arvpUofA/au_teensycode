// idea: adjust shot power of tropedo

#include <arduino.h>
#include <Adafruit_PWMServoDriver.h>

//Servo myservo;
int angle = 0;

int redPin =  12;
int greenPin =  15;
int bluePin =  14;

/*void mosfetSwitch()
{
  
}

void servoAngle()
{
  for(angle = 10; angle < 170; angle += 1)  // goes from 10 degrees to 170 degrees 
  {                                  
    myservo.write(angle);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
}
void rgbControl()
{

  // not sure if this will work lol ->

  // set all 3 pins to the desired intensity
  analogWrite(redPin, redIntensity);
  analogWrite(greenPin, 255 - redIntensity);
  analogWrite(bluePin, 0);
  
  // remain at this color, but not for very long
  delay(10);
  
  // increase the red
  redIntensity = redIntensity + 1;
  
  // since 255 is the maximum, set it back to 0
  // when it increments beyond 255
  if (redIntensity > 255) {
    redIntensity = 0;
}*/

void setup() 
{
  // assign servo pin
  //myservo.attach(20)

  // assign color pins


  // assign board leds
  pinMode(16, OUTPUT); // set pin 16 to output
  pinMode(17, OUTPUT); // set pin 16 to output

  // assign rgb pins
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() 
{
 
  // blink board leds
  digitalWrite(16, HIGH);
  digitalWrite(17, HIGH);
  delay(250);
  digitalWrite(16, LOW);
  digitalWrite(17, LOW);
  delay(1000);
}