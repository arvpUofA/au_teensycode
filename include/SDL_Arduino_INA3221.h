#ifndef SDL_ARDUINO_H
#define SDL_ARDUINO_H

//
//   SDL_Arduino_INA3221 Library
//   SDL_Arduino_INA3221.cpp Arduino code - runs in continuous mode
//   Version 1.1
//   SwitchDoc Labs   January 31, 2015
//
//

/**************************************************************************/
/*! 
    Initial code from INA219 code (Basically just a core structure left)
    @author   K. Townsend (Adafruit Industries)
  @license  BSD (see BSDlicense.txt)
  
  */
/**************************************************************************/

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include "WProgram.h"
#endif

#include <i2c_t3.h>
#include "queue.h"

/*=========================================================================
    I2C ADDRESS/BITS
    -----------------------------------------------------------------------*/
    #define INA3221_ADDRESS_1                       (0x40)    // 1000000 (A0+A1=GND)
    #define INA3221_ADDRESS_2                       (0x41)    //1000001 (A0=VS)
    #define INA3221_ADDRESS_3                       (0x42)    // 1000010 (A0=SDA)
    #define INA3221_ADDRESS_4                       (0x43)    //1000011 (A0=SCL)

/*=========================================================================*/

/*=========================================================================
    CONFIG REGISTER (R/W)
    -----------------------------------------------------------------------*/
    #define INA3221_REG_CONFIG                      (0x00)
    /*---------------------------------------------------------------------*/
    #define INA3221_CONFIG_RESET                    (0x8000)  // Reset Bit
  
    #define INA3221_CONFIG_ENABLE_CHAN1             (0x4000)  // Enable Channel 1
    #define INA3221_CONFIG_ENABLE_CHAN2             (0x2000)  // Enable Channel 2
    #define INA3221_CONFIG_ENABLE_CHAN3             (0x1000)  // Enable Channel 3
  
    #define INA3221_CONFIG_AVG2                     (0x0800)  // AVG Samples Bit 2 - See table 3 spec
    #define INA3221_CONFIG_AVG1                     (0x0400)  // AVG Samples Bit 1 - See table 3 spec
    #define INA3221_CONFIG_AVG0                     (0x0200)  // AVG Samples Bit 0 - See table 3 spec

    #define INA3221_CONFIG_VBUS_CT2                 (0x0100)  // VBUS bit 2 Conversion time - See table 4 spec
    #define INA3221_CONFIG_VBUS_CT1                 (0x0080)  // VBUS bit 1 Conversion time - See table 4 spec
    #define INA3221_CONFIG_VBUS_CT0                 (0x0040)  // VBUS bit 0 Conversion time - See table 4 spec

    #define INA3221_CONFIG_VSH_CT2                  (0x0020)  // Vshunt bit 2 Conversion time - See table 5 spec
    #define INA3221_CONFIG_VSH_CT1                  (0x0010)  // Vshunt bit 1 Conversion time - See table 5 spec
    #define INA3221_CONFIG_VSH_CT0                  (0x0008)  // Vshunt bit 0 Conversion time - See table 5 spec

    #define INA3221_CONFIG_MODE_2                   (0x0004)  // Operating Mode bit 2 - See table 6 spec
    #define INA3221_CONFIG_MODE_1                   (0x0002)  // Operating Mode bit 1 - See table 6 spec
    #define INA3221_CONFIG_MODE_0                   (0x0001)  // Operating Mode bit 0 - See table 6 spec

/*=========================================================================*/

/*=========================================================================
    SHUNT VOLTAGE REGISTER (R)
    -----------------------------------------------------------------------*/
    #define INA3221_REG_SHUNTVOLTAGE_1                (0x01)
   
/*=========================================================================*/

/*=========================================================================
    BUS VOLTAGE REGISTER (R)
    -----------------------------------------------------------------------*/
    #define INA3221_REG_BUSVOLTAGE_1                  (0x02)
   
/*=========================================================================*/

#define SHUNT_RESISTOR_VALUE_1    (0.001)   // default shunt resistor value of 0.1 Ohm
#define SHUNT_RESISTOR_VALUE_2  (0.001)   // default shunt resistor value of 0.1 Ohm
#define SHUNT_RESISTOR_VALUE_3  (0.001)   // default shunt resistor value of 0.1 Ohm
#define SHUNT_RESISTOR_VALUE_4  (0.004)   // default shunt resistor value of 0.1 Ohm

struct PowerBuffer {
private:
    int size;
    float sum;
    Queue buffer;
    
public:
    PowerBuffer();
    PowerBuffer(int size);
    void push(float power);
    float getAveragePower();
};

class SDL_Arduino_INA3221 {
public:
    SDL_Arduino_INA3221(uint8_t addr, float shuntresistor, int rate);
    void begin(void);
    void setReduction(int channel, float redc);
    float getBusVoltage_V(int channel);
    float getShuntVoltage_mV(int channel);
    float getCurrent_mA(int channel);
    void tick(int channel);
    void print(int channel);
    float getPower(int channel);
    float getAveragePower_W(int channel);

private:
    float reductions_[3];
    PowerBuffer power_buffers[3];

    uint8_t INA3221_i2caddr;
    float INA3221_shuntresistor;
    void wireWriteRegister(uint8_t reg, uint16_t value);
    void wireReadRegister(uint8_t reg, uint16_t *value);
    void INA3221SetConfig(void);
    int16_t getBusVoltage_raw(int channel);
    int16_t getShuntVoltage_raw(int channel);
};

#endif