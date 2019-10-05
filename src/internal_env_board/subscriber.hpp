#ifndef	SUBSCRIBER_HPP
#define	SUBSCRIBER_HPP


#include <uavcan/uavcan.hpp>
#include <uavcan/equipment/power/BatteryInfo.hpp>
//#include <parameter.hpp>
#include <Arduino.h>
#include <libARVPpwm.h>
#include "batteryStatus.hpp"

using namespace uavcan;

//Subscriber object declerations
Subscriber<equipment::power::BatteryInfo> *batterySubscriber;


/*Callback function for uavcan battery info messages. Stores values into storage array */
void batteryInfoCallback(const uavcan::equipment::power::BatteryInfo& batteryData)
{
  storeVoltageInfo(batteryData.battery_id, batteryData.voltage);
}


void initSubscriber(Node<NodeMemoryPoolSize> *node)
{
  // create subscriber
  batterySubscriber = new Subscriber<equipment::power::BatteryInfo>(*node);

  // start subscriber  
 
  if(batterySubscriber->start(batteryInfoCallback) < 0)
  {
    Serial.println("Unable to start battery subscriber");
  }
}

#endif