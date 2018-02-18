//
//   SDL_Arduino_INA3221 Library
//   SDL_Arduino_INA3221.cpp Arduino code - runs in continuous mode
//   Version 1.1
//   SwitchDoc Labs   January 31, 2015
//
//



/*
    Initial code from INA219 code (Basically just a core structure left)
    @author   K.Townsend (Adafruit Industries)
  @license  BSD (see BSDlicense.txt)
*/

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include "WProgram.h"
#endif

#include <i2c_t3.h>
#include <SDL_Arduino_INA3221.h>

PowerBuffer::PowerBuffer() 
  : PowerBuffer(100)
{

}

PowerBuffer::PowerBuffer(int size)
  : size(size), sum(0)
{

}

void PowerBuffer::push(float power) 
{
  sum += power;
  buffer.push_back(power);

  if (buffer.size() > (size_t) size) {
    sum -= buffer.front();
    buffer.pop_front();
  }
}

float PowerBuffer::getAveragePower()
{
  int size = buffer.size() > 0 ? buffer.size() : 1;
  return sum / size;
}


/**************************************************************************/
/*! 
    @brief  Sends a single command byte over I2C
*/
/**************************************************************************/
void SDL_Arduino_INA3221::wireWriteRegister (uint8_t reg, uint16_t value)
{
  Wire.beginTransmission(INA3221_i2caddr);
  #if ARDUINO >= 100
    Wire.write(reg);                       // Register
    Wire.write((value >> 8) & 0xFF);       // Upper 8-bits
    Wire.write(value & 0xFF);              // Lower 8-bits
  #else
    Wire.send(reg);                        // Register
    Wire.send(value >> 8);                 // Upper 8-bits
    Wire.send(value & 0xFF);               // Lower 8-bits
  #endif
  Wire.endTransmission();
}

/**************************************************************************/
/*! 
    @brief  Reads a 16 bit values over I2C
*/
/**************************************************************************/
void SDL_Arduino_INA3221::wireReadRegister(uint8_t reg, uint16_t *value)
{

  Wire.beginTransmission(INA3221_i2caddr);
  #if ARDUINO >= 100
    Wire.write(reg);                       // Register
  #else
    Wire.send(reg);                        // Register
  #endif
  Wire.endTransmission();
  
  delay(1); // Max 12-bit conversion time is 586us per sample

  Wire.requestFrom(INA3221_i2caddr, (uint8_t)2);  
  #if ARDUINO >= 100
    // Shift values to create properly formed integer
    *value = ((Wire.read() << 8) | Wire.read());
  #else
    // Shift values to create properly formed integer
    *value = ((Wire.receive() << 8) | Wire.receive());
  #endif
}

//
void SDL_Arduino_INA3221::INA3221SetConfig(void)
{
 
 
  // Set Config register to take into account the settings above
  uint16_t config = INA3221_CONFIG_ENABLE_CHAN1 |
                    INA3221_CONFIG_ENABLE_CHAN2 |
                    INA3221_CONFIG_ENABLE_CHAN3 |
                    INA3221_CONFIG_AVG1 |
                    INA3221_CONFIG_VBUS_CT2 |
                    INA3221_CONFIG_VSH_CT2 |
                    INA3221_CONFIG_MODE_2 |
                    INA3221_CONFIG_MODE_1 |
                    INA3221_CONFIG_MODE_0;
  wireWriteRegister(INA3221_REG_CONFIG, config);
}

/**************************************************************************/
/*! 
    @brief  Instantiates a new SDL_Arduino_INA3221 class
*/
/**************************************************************************/
SDL_Arduino_INA3221::SDL_Arduino_INA3221(uint8_t addr, float shuntresistor, int rate)
{
    INA3221_i2caddr = addr;
    INA3221_shuntresistor = shuntresistor;

    for (int i = 0; i < 3; i++) {
      reductions_[i] = 1;
      power_buffers[i] = PowerBuffer(rate);
    }
}

/**************************************************************************/
/*! 
    @brief  Setups the HW (defaults to 32V and 2A for calibration values)
*/
/**************************************************************************/
void SDL_Arduino_INA3221::begin() {
  Wire.begin();    
  // Set chip to known config values to start
  INA3221SetConfig();
    
    Serial.print("shunt resistor="); Serial.println(INA3221_shuntresistor);
        Serial.print("address="); Serial.println(INA3221_i2caddr);
}

void SDL_Arduino_INA3221::setReduction(int channel, float redc) {
  reductions_[channel - 1] = redc;
}

/**************************************************************************/
/*! 
    @brief  Gets the raw bus voltage (16-bit signed integer, so +-32767)
*/
/**************************************************************************/
int16_t SDL_Arduino_INA3221::getBusVoltage_raw(int channel) {
  uint16_t value;
  wireReadRegister(INA3221_REG_BUSVOLTAGE_1+(channel -1) *2, &value);
//    Serial.print("BusVoltage_raw=");
//    Serial.println(value,HEX);

  // Shift to the right 3 to drop CNVR and OVF and multiply by LSB
  return (int16_t)(value );
}

/**************************************************************************/
/*! 
    @brief  Gets the raw shunt voltage (16-bit signed integer, so +-32767)
*/
/**************************************************************************/
int16_t SDL_Arduino_INA3221::getShuntVoltage_raw(int channel) {
  uint16_t value;
  wireReadRegister(INA3221_REG_SHUNTVOLTAGE_1+(channel -1) *2, &value);
   // Serial.print("ShuntVoltage_raw=");
   // Serial.println(value,HEX);
  return (int16_t)value;
}

/**************************************************************************/
/*! 
    @brief  Gets the shunt voltage in mV (so +-168.3mV)
*/
/**************************************************************************/
float SDL_Arduino_INA3221::getShuntVoltage_mV(int channel) {
  int16_t value;
  value = getShuntVoltage_raw(channel);
  return value * 0.005;
}

/**************************************************************************/
/*! 
    @brief  Gets the bus voltage in volts
*/
/**************************************************************************/
float SDL_Arduino_INA3221::getBusVoltage_V(int channel) {
  int16_t value = getBusVoltage_raw(channel);
  return value * 0.001;
}

/**************************************************************************/
/*! 
    @brief  Gets the current value in mA, taking into account the
            config settings and current LSB
*/
/**************************************************************************/
float SDL_Arduino_INA3221::getCurrent_mA(int channel) {
  float valueDec = getShuntVoltage_mV(channel)/INA3221_shuntresistor;
  float redc = reductions_[channel - 1];

  return redc * fabs(valueDec);
}

void SDL_Arduino_INA3221::tick(int channel) {
  float power = getPower(channel);
  power_buffers[channel - 1].push(power);
}

 /**************************************************************************/
/*! 
    @brief  Gets and displays the busvoltage, shuntvoltage, and current
            redc is for any correction that needs to be made on the current
            Set redc to 1 if no correction is needed
*/
/**************************************************************************/
void SDL_Arduino_INA3221::print(int channel) {
    Serial.print(getBusVoltage_V(channel));      Serial.print(" V ");
    Serial.print(getShuntVoltage_mV(channel));   Serial.print(" mV ");
    Serial.print(getCurrent_mA(channel));        Serial.print(" mA ");

    float average_power = power_buffers[channel - 1].getAveragePower();
    Serial.print(average_power);                 Serial.println(" W ");
}
 /**************************************************************************/
/*! 
    @brief  Gets power with any correction needed for the current measurement
            Set redc to 1 if no correction is needed
*/
/**************************************************************************/

float SDL_Arduino_INA3221::getPower(int channel) {
  float voltage = getBusVoltage_V(channel);

  float power = (getCurrent_mA(channel))*(voltage);
  return power/1000.0f;
}
