#ifndef CPP_IMU_CLASS
#define CPP_IMU_CPP

#include <Wire.h>
#include "runnable.h"
#include "I2C.h"


#ifndef VECTOR_STRUCT_H
#define VECTOR_STRUCT_H
struct Vector {
  float x;
  float y;
  float z;
};
#endif

class IMU {
  public:

      int16_t readRegister16(uint8_t reg) {
          int16_t value;
          Wire.beginTransmission(IMU_NAME);
          Wire.write(reg);
          Wire.endTransmission();

          Wire.beginTransmission(IMU_NAME);
          Wire.requestFrom(IMU_NAME, 2);
          while(!Wire.available()) {};

          uint8_t vha = Wire.read();
          uint8_t vla = Wire.read();

          Wire.endTransmission();
          value = vha << 8 | vla;
          return value;
      }

      int8_t readRegister8(uint8_t reg) {
          int8_t value;
          Wire.beginTransmission(IMU_NAME);
          Wire.write(reg);
          Wire.endTransmission();

          Wire.beginTransmission(IMU_NAME);
          Wire.requestFrom(IMU_NAME, 1);
          while(!Wire.available()) {};

          value = Wire.read();
          Wire.endTransmission();

          return value;
      }

      Vector readAccel() {
          Wire.beginTransmission(IMU_NAME);
          Wire.write(MPU6050_REG_ACCEL_XOUT_H);
          Wire.endTransmission();

          Wire.beginTransmission(IMU_NAME);
          Wire.requestFrom(IMU_NAME, 6);

          while (Wire.available() < 6);

          uint8_t xha = Wire.read();
          uint8_t xla = Wire.read();
          uint8_t yha = Wire.read();
          uint8_t yla = Wire.read();
          uint8_t zha = Wire.read();
          uint8_t zla = Wire.read();

          ra.x = xha << 8 | xla;
          ra.y = yha << 8 | yla;
          ra.z = zha << 8 | zla;
          return ra;
      }

      Vector readGyr() {
          Wire.beginTransmission(IMU_NAME);
          Wire.write(MPU6050_REG_GYRO_XOUT_H);
          Wire.endTransmission();

          Wire.beginTransmission(IMU_NAME);
          Wire.requestFrom(IMU_NAME, 6);

          while (Wire.available() < 6);

          uint8_t xha = Wire.read();
	        uint8_t xla = Wire.read();
          uint8_t yha = Wire.read();
	        uint8_t yla = Wire.read();
          uint8_t zha = Wire.read();
          uint8_t zla = Wire.read();

          rg.x = xha << 8 | xla;
          rg.y = yha << 8 | yla;
          rg.z = zha << 8 | zla;

          return rg;
      }
    private:
      Vector ra, rg;

};




#endif
