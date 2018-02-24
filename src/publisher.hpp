#ifndef	PUBLISHER_HPP
#define	PUBLISHER_HPP

#include <UAVCAN.hpp>
#include <uavcan/equipment/air_data/StaticPressure.hpp>
#include <uavcan/equipment/air_data/StaticTemperature.hpp>
#include "internalEnvBoard.h"

using namespace uavcan;

// publisher
Publisher<equipment::air_data::StaticPressure> *pressurePublisher;
Publisher<equipment::air_data::StaticTemperature> *temperaturePublisher;


void initPublisher(Node<NodeMemoryPoolSize> *node)
{
  // create publishers
  pressurePublisher = new Publisher<equipment::air_data::StaticPressure>(*node);
  temperaturePublisher = new Publisher<equipment::air_data::StaticTemperature>(*node);

  // initiliaze publishers
  if(pressurePublisher->init() < 0)
  {
    Serial.println("Unable to initialize pressure message publisher!");
  }
  if(temperaturePublisher->init() < 0)
  {
    Serial.println("Unable to initialize temperature message publisher!");
  }

  // set TX timeout
  pressurePublisher->setTxTimeout(MonotonicDuration::fromUSec(800));
  temperaturePublisher->setTxTimeout(MonotonicDuration::fromUSec(800));
}


void cyclePublisher()
{
  // send a very important log message to everyone
  {
    equipment::air_data::StaticTemperature temp;

    temp.static_temperature = publishTemp();
    Serial.println(publishTemp());

    const int pres = temperaturePublisher->broadcast(temp);
    if (pres < 0)
    {
      Serial.println("Error while broadcasting temperature message");
    }
  }


  // send everyone the truth
  {
    equipment::air_data::StaticPressure press;

    press.static_pressure = publishPress();
    Serial.println(publishPress());

    const int pres = pressurePublisher->broadcast(press);
    if (pres < 0)
    {
      Serial.println("Error while broadcasting pressure message");
    }
  }
}


#endif
