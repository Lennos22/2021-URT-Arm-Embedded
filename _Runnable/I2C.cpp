#ifndef CPP_I2C
#define CPP_I2C

#include <Wire.h>
#include "runnable.h"
#include "I2C.h"


void setup_I2C() {
  Wire.begin();
  Wire.setClock(I2C_CLOCK_FREQ);
}

#endif
