#ifndef	PUBLISHER_HPP
#define	PUBLISHER_HPP

#include <UAVCAN.hpp>
#include <uavcan/protocol/debug/LogMessage.hpp>
#include <uavcan/protocol/debug/KeyValue.hpp>
#include <uavcan/equipment/power/BatteryInfo.hpp>
#include <uavcan/equipment/power/CircuitStatus.hpp>
#include "INA_functions.hpp"

using namespace uavcan;

// battery publishers
Publisher<equipment::power::BatteryInfo> *battPublisher_0;
Publisher<equipment::power::BatteryInfo> *battPublisher_1;
Publisher<equipment::power::BatteryInfo> *battPublisher_2;
Publisher<equipment::power::BatteryInfo> *battPublisher_3;

// power rail publishers
Publisher<equipment::power::CircuitStatus> *pwrRailPublisher_3v3;
Publisher<equipment::power::CircuitStatus> *pwrRailPublisher_5v;
Publisher<equipment::power::CircuitStatus> *pwrRailPublisher_12v;


void initPublisher(Node<NodeMemoryPoolSize> *node)
{
  // create battery publishers
  battPublisher_0 = new Publisher<equipment::power::BatteryInfo>(*node);
  battPublisher_1 = new Publisher<equipment::power::BatteryInfo>(*node);
  battPublisher_2 = new Publisher<equipment::power::BatteryInfo>(*node);
  battPublisher_3 = new Publisher<equipment::power::BatteryInfo>(*node);

  // create power rail publishers
  pwrRailPublisher_3v3 = new Publisher<equipment::power::CircuitStatus>(*node);
  pwrRailPublisher_5v  = new Publisher<equipment::power::CircuitStatus>(*node);
  pwrRailPublisher_12v = new Publisher<equipment::power::CircuitStatus>(*node);

  if (battPublisher_0->init() < 0) {
    Serial.println("Unable to initialize battery publisher 0!");
  }

  if (battPublisher_1->init() < 0) {
    Serial.println("Unable to initialize battery publisher 1!");
  }

  if (battPublisher_2->init() < 0) {
    Serial.println("Unable to initialize battery publisher 2!");
  }

  if (battPublisher_3->init() < 0) {
    Serial.println("Unable to initialize battery publisher 3!");
  }

  if (pwrRailPublisher_3v3->init() < 0) {
    Serial.println("Unable to intialize 3v3 power rail publisher!");
  }

  if (pwrRailPublisher_5v->init() < 0) {
    Serial.println("Unable to intialize 5v power rail publisher!");
  }

  if (pwrRailPublisher_12v->init() < 0) {
    Serial.println("Unable to intialize 12v power rail publisher!");
  }

  battPublisher_0->setTxTimeout(MonotonicDuration::fromUSec(800)); //should these be this?
  battPublisher_1->setTxTimeout(MonotonicDuration::fromUSec(800));
  battPublisher_2->setTxTimeout(MonotonicDuration::fromUSec(800));
  battPublisher_3->setTxTimeout(MonotonicDuration::fromUSec(800));

  pwrRailPublisher_3v3->setTxTimeout(MonotonicDuration::fromUSec(800));
  pwrRailPublisher_5v->setTxTimeout(MonotonicDuration::fromUSec(800));
  pwrRailPublisher_12v->setTxTimeout(MonotonicDuration::fromUSec(800));
}

static void publishBatteries(void) {
  // Publish Battery 0 info
  {
    equipment::power::BatteryInfo msg;

    msg.voltage = battery_0.volts;
    msg.current = battery_0.current;
    msg.average_power_10sec = battery_0.power;

    const int rc = battPublisher_0->broadcast(msg);
    if (rc < 0) {
      Serial.println("Error while broadcasting battery 0 message");
    }
  }

  // Publish Battery 1 info
  {
    equipment::power::BatteryInfo msg;

    msg.voltage = battery_1.volts;
    msg.current = battery_1.current;
    msg.average_power_10sec = battery_1.power;

    const int rc = battPublisher_1->broadcast(msg);
    if (rc < 0) {
      Serial.println("Error while broadcasting battery 1 message");
    }
  }

  // Publish Battery 2 info
  {
    equipment::power::BatteryInfo msg;

    msg.voltage = battery_2.volts;
    msg.current = battery_2.current;
    msg.average_power_10sec = battery_2.power;

    const int rc = battPublisher_2->broadcast(msg);
    if (rc < 0) {
      Serial.println("Error while broadcasting battery 2 message");
    }
  }

  // Publish Battery 3 info
  {
    equipment::power::BatteryInfo msg;

    msg.voltage = battery_3.volts;
    msg.current = battery_3.current;
    msg.average_power_10sec = battery_3.power;

    const int rc = battPublisher_3->broadcast(msg);
    if (rc < 0) {
      Serial.println("Error while broadcasting battery 3 message");
    }
  }
}

static void publishPwrRails(void) {
  // Publish 3v3 power rail info
  {
    equipment::power::CircuitStatus msg;

    msg.voltage = power_rail_0.volts;
    msg.current = power_rail_0.current;

    const int rc = pwrRailPublisher_3v3->broadcast(msg);
    if (rc < 0) {
      Serial.println("Error while broadcasting 3v3 power rail message");
    }
  }

  // Publish 5v power rail info
  {
    equipment::power::CircuitStatus msg;

    msg.voltage = power_rail_1.volts;
    msg.current = power_rail_1.current;

    const int rc = pwrRailPublisher_5v->broadcast(msg);
    if (rc < 0) {
      Serial.println("Error while broadcasting 5v power rail message");
    }
  }

  // Publish 12v power rail info
  {
    equipment::power::CircuitStatus msg;

    msg.voltage = power_rail_2.volts;
    msg.current = power_rail_2.current;

    const int rc = pwrRailPublisher_12v->broadcast(msg);
    if (rc < 0) {
      Serial.println("Error while broadcasting 12v power rail message");
    }
  }
}

void cyclePublisher()
{
  publishBatteries();
  publishPwrRails();
}

#endif
