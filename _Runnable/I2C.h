#ifndef H_I2C
#define H_I2C

#define I2C_CLOCK_FREQ                100000

#ifndef VECTOR_STRUCT_H
#define VECTOR_STRUCT_H
struct Vector {
  float x;
  float y;
  float z;
};
#endif

void setup_I2C();

#endif
