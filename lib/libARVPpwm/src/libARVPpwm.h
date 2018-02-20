/*************************************************** 
  This is a library for our Adafruit 16-channel PWM & Servo driver

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/815

  These displays use I2C to communicate, 2 pins are required to  
  interface. For Arduino UNOs, thats SCL -> Analog 5, SDA -> Analog 4

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/
#ifndef LIB_ARVP_PWM_H
#define LIB_ARVP_PWM_H

#include <Wire.h>

#define PCA9685_BASEADDR 0X40
#define PCA9685_SUBADDR1 0x1
#define PCA9685_SUBADDR2 0x2
#define PCA9685_SUBADDR3 0x3

#define PCA9685_MODE1 0x0
#define PCA9685_PRESCALE 0xFE

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD

#define PWM_FEQUENCY 100 //Hz



enum angleUnits {UNIT_RADIANS, UNIT_DEGREES};

/**************************************************************************/
/*! 
    @brief  Class that stores state and functions for interacting with PCA9685 PWM chip
*/
/**************************************************************************/
class Adafruit_PWMServoDriver {
 private:
  uint8_t _i2caddr;

  //default channels for REG LED strip
  uint8_t redChannel = 13;
  uint8_t greenChannel = 14;
  uint8_t blueChannel = 15;
  uint8_t whiteChannel = 12;

  float pwmFreq;
  TwoWire *_i2c;

  uint8_t read8(uint8_t addr);
  void write8(uint8_t addr, uint8_t d);

 public:
  Adafruit_PWMServoDriver(uint8_t addr);
  Adafruit_PWMServoDriver(TwoWire *I2C, uint8_t addr);
  void begin(void);
  void reset(void);
  void setPWMFreq(float freq);
  void setPWM(uint8_t num, uint16_t on, uint16_t off);
  void setPin(uint8_t num, uint16_t val, bool invert=false);
  void setServoPulse(uint8_t n, double pulse);
  void setServoAngle(uint8_t pwmChannel, float angle, uint16_t minPulse, uint16_t maxPulse, angleUnits angleUnit);
  void setRGBChannels(uint8_t red, uint8_t green, uint8_t blue, uint8_t white);
  void setRGB(float red, float green, float blue, float white, float brightness); 
};

#endif