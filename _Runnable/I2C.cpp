#ifndef CPP_I2C
#define CPP_I2C

#include <Wire.h>
#include "runnable.h"
#include "I2C.h"
#include "IMU_class.h"


void setup_I2C() {
  Serial.println("in I2C setup");
  //Wire.begin();
  //Wire.setClock(I2C_CLOCK_FREQ);
}

#endif
