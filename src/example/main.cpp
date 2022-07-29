#include <Arduino.h>
#include <INA.h>
#include <watchdog.h>

void setup()
{
  pinMode(13, OUTPUT);
}

void loop()
{
  KickDog();
  delay(100);
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);

}
