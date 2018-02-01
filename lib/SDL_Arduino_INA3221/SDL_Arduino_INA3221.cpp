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
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <i2c_t3.h>

#include "SDL_Arduino_INA3221.h"

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

void SDL_Arduino_INA3221_2::wireWriteRegister (uint8_t reg, uint16_t value)
{
  Wire.beginTransmission(INA3221_2_i2caddr);
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

void SDL_Arduino_INA3221_3::wireWriteRegister (uint8_t reg, uint16_t value)
{
  Wire.beginTransmission(INA3221_3_i2caddr);
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

void SDL_Arduino_INA3221_4::wireWriteRegister (uint8_t reg, uint16_t value)
{
  Wire.beginTransmission(INA3221_4_i2caddr);
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

void SDL_Arduino_INA3221_2::wireReadRegister(uint8_t reg, uint16_t *value)
{

  Wire.beginTransmission(INA3221_2_i2caddr);
  #if ARDUINO >= 100
    Wire.write(reg);                       // Register
  #else
    Wire.send(reg);                        // Register
  #endif
  Wire.endTransmission();
  
  delay(1); // Max 12-bit conversion time is 586us per sample

  Wire.requestFrom(INA3221_2_i2caddr, (uint8_t)2);  
  #if ARDUINO >= 100
    // Shift values to create properly formed integer
    *value = ((Wire.read() << 8) | Wire.read());
  #else
    // Shift values to create properly formed integer
    *value = ((Wire.receive() << 8) | Wire.receive());
  #endif
}

//
void SDL_Arduino_INA3221_2::INA3221_2_SetConfig(void)
{
 
 
  // Set Config register to take into account the settings above
  uint16_t config = INA3221_2_CONFIG_ENABLE_CHAN1 |
                    INA3221_2_CONFIG_ENABLE_CHAN2 |
                    INA3221_2_CONFIG_ENABLE_CHAN3 |
                    INA3221_2_CONFIG_AVG1 |
                    INA3221_2_CONFIG_VBUS_CT2 |
                    INA3221_2_CONFIG_VSH_CT2 |
                    INA3221_2_CONFIG_MODE_2 |
                    INA3221_2_CONFIG_MODE_1 |
                    INA3221_2_CONFIG_MODE_0;
  wireWriteRegister(INA3221_2_REG_CONFIG, config);
}

void SDL_Arduino_INA3221_3::wireReadRegister(uint8_t reg, uint16_t *value)
{

  Wire.beginTransmission(INA3221_3_i2caddr);
  #if ARDUINO >= 100
    Wire.write(reg);                       // Register
  #else
    Wire.send(reg);                        // Register
  #endif
  Wire.endTransmission();
  
  delay(1); // Max 12-bit conversion time is 586us per sample

  Wire.requestFrom(INA3221_3_i2caddr, (uint8_t)2);  
  #if ARDUINO >= 100
    // Shift values to create properly formed integer
    *value = ((Wire.read() << 8) | Wire.read());
  #else
    // Shift values to create properly formed integer
    *value = ((Wire.receive() << 8) | Wire.receive());
  #endif
}

//
void SDL_Arduino_INA3221_3::INA3221_3_SetConfig(void)
{
 
 
  // Set Config register to take into account the settings above
  uint16_t config = INA3221_3_CONFIG_ENABLE_CHAN1 |
                    INA3221_3_CONFIG_ENABLE_CHAN2 |
                    INA3221_3_CONFIG_ENABLE_CHAN3 |
                    INA3221_3_CONFIG_AVG1 |
                    INA3221_3_CONFIG_VBUS_CT2 |
                    INA3221_3_CONFIG_VSH_CT2 |
                    INA3221_3_CONFIG_MODE_2 |
                    INA3221_3_CONFIG_MODE_1 |
                    INA3221_3_CONFIG_MODE_0;
  wireWriteRegister(INA3221_3_REG_CONFIG, config);
}

void SDL_Arduino_INA3221_4::wireReadRegister(uint8_t reg, uint16_t *value)
{

  Wire.beginTransmission(INA3221_4_i2caddr);
  #if ARDUINO >= 100
    Wire.write(reg);                       // Register
  #else
    Wire.send(reg);                        // Register
  #endif
  Wire.endTransmission();
  
  delay(1); // Max 12-bit conversion time is 586us per sample

  Wire.requestFrom(INA3221_4_i2caddr, (uint8_t)2);  
  #if ARDUINO >= 100
    // Shift values to create properly formed integer
    *value = ((Wire.read() << 8) | Wire.read());
  #else
    // Shift values to create properly formed integer
    *value = ((Wire.receive() << 8) | Wire.receive());
  #endif
}

//
void SDL_Arduino_INA3221_4::INA3221_4_SetConfig(void)
{
 
 
  // Set Config register to take into account the settings above
  uint16_t config = INA3221_4_CONFIG_ENABLE_CHAN1 |
                    INA3221_4_CONFIG_ENABLE_CHAN2 |
                    INA3221_4_CONFIG_ENABLE_CHAN3 |
                    INA3221_4_CONFIG_AVG1 |
                    INA3221_4_CONFIG_VBUS_CT2 |
                    INA3221_4_CONFIG_VSH_CT2 |
                    INA3221_4_CONFIG_MODE_2 |
                    INA3221_4_CONFIG_MODE_1 |
                    INA3221_4_CONFIG_MODE_0;
  wireWriteRegister(INA3221_4_REG_CONFIG, config);
}
/**************************************************************************/
/*! 
    @brief  Instantiates a new SDL_Arduino_INA3221 class
*/
/**************************************************************************/
SDL_Arduino_INA3221::SDL_Arduino_INA3221(uint8_t addr, float shuntresistor) {
    
    INA3221_i2caddr = addr;
    INA3221_shuntresistor = shuntresistor;
 
}

SDL_Arduino_INA3221_2::SDL_Arduino_INA3221_2(uint8_t addr, float shuntresistor) {
    
    INA3221_2_i2caddr = addr;
    INA3221_2_shuntresistor = shuntresistor;
 
}

SDL_Arduino_INA3221_3::SDL_Arduino_INA3221_3(uint8_t addr, float shuntresistor) {
    
    INA3221_3_i2caddr = addr;
    INA3221_3_shuntresistor = shuntresistor;
 
}

SDL_Arduino_INA3221_4::SDL_Arduino_INA3221_4(uint8_t addr, float shuntresistor) {
    
    INA3221_4_i2caddr = addr;
    INA3221_4_shuntresistor = shuntresistor;
 
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

void SDL_Arduino_INA3221_2::begin() {
  Wire.begin();    
  // Set chip to known config values to start
  INA3221_2_SetConfig();
    
    Serial.print("shunt resistor="); Serial.println(INA3221_2_shuntresistor);
        Serial.print("address="); Serial.println(INA3221_2_i2caddr);
    
}

void SDL_Arduino_INA3221_3::begin() {
  Wire.begin();    
  // Set chip to known config values to start
  INA3221_3_SetConfig();
    
    Serial.print("shunt resistor="); Serial.println(INA3221_3_shuntresistor);
        Serial.print("address="); Serial.println(INA3221_3_i2caddr);
    
}

void SDL_Arduino_INA3221_4::begin() {
  Wire.begin();    
  // Set chip to known config values to start
  INA3221_4_SetConfig();
    
    Serial.print("shunt resistor="); Serial.println(INA3221_4_shuntresistor);
        Serial.print("address="); Serial.println(INA3221_4_i2caddr);
    
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

int16_t SDL_Arduino_INA3221_2::getBusVoltage_raw(int channel) {
  uint16_t value;
  wireReadRegister(INA3221_2_REG_BUSVOLTAGE_1+(channel -1) *2, &value);
//    Serial.print("BusVoltage_raw=");
//    Serial.println(value,HEX);

  // Shift to the right 3 to drop CNVR and OVF and multiply by LSB
  return (int16_t)(value );
}

int16_t SDL_Arduino_INA3221_3::getBusVoltage_raw(int channel) {
  uint16_t value;
  wireReadRegister(INA3221_3_REG_BUSVOLTAGE_1+(channel -1) *2, &value);
//    Serial.print("BusVoltage_raw=");
//    Serial.println(value,HEX);

  // Shift to the right 3 to drop CNVR and OVF and multiply by LSB
  return (int16_t)(value );
}

int16_t SDL_Arduino_INA3221_4::getBusVoltage_raw(int channel) {
  uint16_t value;
  wireReadRegister(INA3221_4_REG_BUSVOLTAGE_1+(channel -1) *2, &value);
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

int16_t SDL_Arduino_INA3221_2::getShuntVoltage_raw(int channel) {
  uint16_t value;
  wireReadRegister(INA3221_2_REG_SHUNTVOLTAGE_1+(channel -1) *2, &value);
   // Serial.print("ShuntVoltage_raw=");
   // Serial.println(value,HEX);
  return (int16_t)value;
}

int16_t SDL_Arduino_INA3221_3::getShuntVoltage_raw(int channel) {
  uint16_t value;
  wireReadRegister(INA3221_3_REG_SHUNTVOLTAGE_1+(channel -1) *2, &value);
   // Serial.print("ShuntVoltage_raw=");
   // Serial.println(value,HEX);
  return (int16_t)value;
} 

int16_t SDL_Arduino_INA3221_4::getShuntVoltage_raw(int channel) {
  uint16_t value;
  wireReadRegister(INA3221_4_REG_SHUNTVOLTAGE_1+(channel -1) *2, &value);
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

float SDL_Arduino_INA3221_2::getShuntVoltage_mV(int channel) {
  int16_t value;
  value = getShuntVoltage_raw(channel);
  return value * 0.005;
}

float SDL_Arduino_INA3221_3::getShuntVoltage_mV(int channel) {
  int16_t value;
  value = getShuntVoltage_raw(channel);
  return value * 0.005;
}

float SDL_Arduino_INA3221_4::getShuntVoltage_mV(int channel) {
  int16_t value;
  value = getShuntVoltage_raw(channel);
  return value * 0.005;
}
/**************************************************************************/
/*! 
    @brief  Gets the shunt voltage in volts
*/
/**************************************************************************/
float SDL_Arduino_INA3221::getBusVoltage_V(int channel) {
  int16_t value = getBusVoltage_raw(channel);
  return value * 0.001;
}

float SDL_Arduino_INA3221_2::getBusVoltage_V(int channel) {
  int16_t value = getBusVoltage_raw(channel);
  return value * 0.001;
}

float SDL_Arduino_INA3221_3::getBusVoltage_V(int channel) {
  int16_t value = getBusVoltage_raw(channel);
  return value * 0.001;
}

float SDL_Arduino_INA3221_4::getBusVoltage_V(int channel) {
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
  
  return valueDec;
}

float SDL_Arduino_INA3221_2::getCurrent_mA(int channel) {
    float valueDec = getShuntVoltage_mV(channel)/INA3221_2_shuntresistor;
  
  return valueDec;
}

float SDL_Arduino_INA3221_3::getCurrent_mA(int channel) {
    float valueDec = getShuntVoltage_mV(channel)/INA3221_3_shuntresistor;
  
  return valueDec;
}

float SDL_Arduino_INA3221_4::getCurrent_mA(int channel) {
    float valueDec = getShuntVoltage_mV(channel)/INA3221_4_shuntresistor;
  
  return valueDec;
}