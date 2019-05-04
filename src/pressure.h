#ifndef PRESSURE_H_
#define PRESSURE_H_

//use this for reading out pressure data in two parts
typedef struct {
  uint32_t whole;
  uint8_t fractional;
} pressure_StructDef;

#endif
