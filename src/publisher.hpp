#ifndef	PUBLISHER_HPP
#define	PUBLISHER_HPP

#include <UAVCAN.hpp>
#include <uavcan/equipment/air_data/StaticPressure.hpp>
#include <uavcan/equipment/air_data/StaticTemperature.hpp>
//#include <arvp/InternalSensor.hpp>
#include "internalEnvBoard.h"

using namespace uavcan;

// publisher
Publisher<equipment::air_data::StaticPressure> *pressurePublisher;
Publisher<equipment::air_data::StaticTemperature> *temperaturePublisher;
//Publisher<arvp::InternalSensor> *sensorPublisher;

void initPublisher(Node<NodeMemoryPoolSize> *node)
{
  // create publishers
  pressurePublisher = new Publisher<equipment::air_data::StaticPressure>(*node);
  temperaturePublisher = new Publisher<equipment::air_data::StaticTemperature>(*node);
  //sensorPublisher = new Publisher<arvp::InternalSensor>(*node);

  // initiliaze publishers
  if(pressurePublisher->init() < 0)
  {
    Serial.println("Unable to initialize pressure message publisher!");
  }
  if(temperaturePublisher->init() < 0)
  {
    Serial.println("Unable to initialize temperature message publisher!");
  }
  /*if(sensorPublisher->init() < 0)
  {
    Serial.println("Unable to initialize sensor message publisher!");
  }*/

  // set TX timeout
  pressurePublisher->setTxTimeout(MonotonicDuration::fromUSec(800));
  temperaturePublisher->setTxTimeout(MonotonicDuration::fromUSec(800));
  //sensorPublisher->setTxTimeout(MonotonicDuration::fromUSec(800));
}


void cyclePublisher()
{
    equipment::air_data::StaticTemperature temp;
    equipment::air_data::StaticPressure press;
    //arvp::InternalSensor sensor;

    temp.static_temperature = publishTemp();
    //Serial.println(publishTemp());

    const int pres = temperaturePublisher->broadcast(temp);
    if (pres < 0)
    {
      Serial.println("Error while broadcasting temperature message");
    }

    press.static_pressure = publishPress();
    //Serial.println(publishPress());

    const int pres2 = pressurePublisher->broadcast(press);
    if (pres2 < 0)
    {
      Serial.println("Error while broadcasting pressure message");
    }

    /*sensor.temperature = publishTemp();
    sensor.pressure = publishPress();
    sensor.humidity = publishHumidity();

    const int pres3 = sensorPublisher->broadcast(sensor);
    if (pres3 < 0)
    {
      Serial.println("Error while broadcasting sensor message");
    }*/
}


#endif
