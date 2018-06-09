#ifndef SENSOR_FUNCTIONS_H
#define SENSOR_FUNCTIONS_H

// humidity/temperature sensor
#define HIH7120ADDRESS 0x27

// pressure/temperature sensor
#define MPLADDRESS 0x60

#define SAMPLES_PER_SECOND 10

//use this for reading out pressure data in two parts
struct pressure_StructDef {
  uint32_t whole;
  uint8_t fractional;
};

extern float humidity(void);
extern float temp(void);
extern uint32_t pressure(void);

extern void measureHIH7120(void);
extern void setupMPL(void);
extern pressure_StructDef readPressureMPL(void);

extern float publishHumidity(void);
extern float publishTemp(void);
extern float publishPress(void);

#endif
