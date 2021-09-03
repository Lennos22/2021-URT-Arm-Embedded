#ifndef MPU6050_CPP
#define MPU6050_CPP

#include <Wire.h>
#include "runnable.h"
#include "MPU6050.h"

bool MPU6050::begin(int accSens, int  gyroSens, int bandwidth) {

  if(isConnected()) {
    return false;
  }
  wakeUp();
  setAccRange(accSens);
  setGyroScale(gyroSens);
  setDLPFBandwidth(bandwidth);
  // Initiate the raw measurements to 0.
  ra.x = 0;
  ra.y = 0;
  ra.z = 0;
  rg.x = 0;
  rg.y = 0;
  rg.z = 0;
  accel.x = 0;
  accel.y = 0;
  accel.z = 0;
  gyro.x = 0;
  gyro.y = 0;
  gyro.z = 0;
  return true;
}

bool MPU6050::isConnected() {
  uint8_t value = readRegister8(MPU6050_REGISTER_WHO_AM_I) & 0b00001111;
  value |= (0b0110000);
  return value == MPU6050_DEFAULT_ADDRESS;
}

void MPU6050::sleepMode() {
    writeRegister8(MPU6050_REGISTER_PWR_MGMT_1, 0b01000000);
}
void MPU6050::wakeUp() {
    writeRegister8(MPU6050_REGISTER_PWR_MGMT_1, 0b00000000);
}



void MPU6050::setAccRange(int range) {
  uint8_t value;
  value = readRegister8(MPU6050_REG_ACCEL_CONFIG);
  value &= 0b11100111;
  value |= range;
  writeRegister8(MPU6050_REG_ACCEL_CONFIG, value << 3);

  if(range == PM_2_G) {
    a_sens.mode = PM_2_G;
    a_sens.factor = PM_2_G_FACT;
  } else if(range == PM_4_G) {
    a_sens.mode = PM_4_G;
    a_sens.factor = PM_4_G_FACT;
  } else if(range == PM_8_G) {
    a_sens.mode = PM_8_G;
    a_sens.factor = PM_8_G_FACT;
  } else {
    a_sens.mode = PM_16_G;
    a_sens.factor = PM_16_G_FACT;
  }
}
uint8_t MPU6050::getAccRange() {
  uint8_t value;
  value = readRegister8(MPU6050_REG_ACCEL_CONFIG);
  return value >> 3;
}
void MPU6050::setGyroScale(int scale) {
  uint8_t value;
  uint8_t scale_8 = (uint8_t)scale;
  value = readRegister8(MPU6050_REG_GYRO_CONFIG);
  value &= 0b11100111;
  value |= scale_8;
  writeRegister8(MPU6050_REG_GYRO_CONFIG, value << 3);

  if(scale == PM_250_DEG_SEC) {
    g_sens.mode = PM_250_DEG_SEC;
    g_sens.factor = PM_250_DEG_SEC_FACT;
  } else if(scale == PM_500_DEG_SEC) {
    g_sens.mode = PM_500_DEG_SEC;
    g_sens.factor = PM_500_DEG_SEC_FACT;
  } else if(scale == PM_1000_DEG_SEC) {
    g_sens.mode = PM_1000_DEG_SEC;
    g_sens.factor = PM_1000_DEG_SEC_FACT;
  } else {
    g_sens.mode = PM_2000_DEG_SEC;
    g_sens.factor = PM_2000_DEG_SEC_FACT;
  }
}
uint8_t MPU6050::getGyroScale() {
  uint8_t value = readRegister8(MPU6050_REG_GYRO_CONFIG);
  return value >> 3;
}

void MPU6050::setDLPFBandwidth(int bandwidth) {
  uint8_t bandwidth_8 = (uint8_t)bandwidth;
  uint8_t value = readRegister8(MPU6050_REGISTER_CONFIG);
  value = (value & 0b11111000) | bandwidth_8;
  writeRegister8(MPU6050_REGISTER_CONFIG, value);
}
uint8_t MPU6050::getBandwidth() {
  uint8_t value = readRegister8(MPU6050_REGISTER_CONFIG);
  return value &= 0b00000111;
}

double MPU6050::readTemp() {
  int16_t rawTemp ;
  Wire.beginTransmission(MPU6050_NAME);
  Wire.write(MPU6050_REG_TEMP_OUT_H);
  Wire.endTransmission();

  Wire.beginTransmission(MPU6050_NAME);
  Wire.requestFrom(MPU6050_NAME, 2);

  while(Wire.available() < 2);

  uint8_t tha = Wire.read();
  uint8_t tla = Wire.read();

  rawTemp = (int16_t)(tha << 8 | tla);
  return rawTemp/340 + 36.53;
}
Vector MPU6050::readAccel() {
  Wire.beginTransmission(MPU6050_NAME);
  Wire.write(MPU6050_REG_ACCEL_XOUT_H);
  Wire.endTransmission();

  Wire.beginTransmission(MPU6050_NAME);
  Wire.requestFrom(MPU6050_NAME, 6);

  while (Wire.available() < 6);

  uint8_t xha = Wire.read();
  uint8_t xla = Wire.read();
  uint8_t yha = Wire.read();
  uint8_t yla = Wire.read();
  uint8_t zha = Wire.read();
  uint8_t zla = Wire.read();

  /**Serial.print("Accel: \nx \ n HIGH:");
  Serial.print(xha, HEX);
  Serial.print(", LOW: ");
  Serial.println(xla, HEX);

  Serial.print("y \ n HIGH:");
  Serial.print(yha, HEX);
  Serial.print(", LOW: ");
  Serial.println(yla, HEX);
  Serial.print("z \ n HIGH:");
  Serial.print(zha, HEX);
  Serial.print(", LOW: ");
  Serial.println(zla, HEX);

  Serial.print("Full x in hex: ");
  Serial.println( xha << 8 | xla, HEX);
  Serial.println();
  Serial.print("As a int16_t : ");
  Serial.println((int16_t)(xha << 8 | xla));*/

  ra.x = (int16_t)(xha << 8 | xla);
  ra.y = (int16_t)(yha << 8 | yla);
  ra.z = (int16_t)(zha << 8 | zla);
  accelRawToG();
  return accel;
}
Vector MPU6050::readGyro() {
  Wire.beginTransmission(MPU6050_NAME);
  Wire.write(MPU6050_REG_GYRO_XOUT_H);
  Wire.endTransmission();

  Wire.beginTransmission(MPU6050_NAME);
  Wire.requestFrom(MPU6050_NAME, 6);

  while (Wire.available() < 6);

  uint8_t xha = Wire.read();
  uint8_t xla = Wire.read();
  uint8_t yha = Wire.read();
  uint8_t yla = Wire.read();
  uint8_t zha = Wire.read();
  uint8_t zla = Wire.read();

  rg.x = (int16_t)(xha << 8 | xla);
  rg.y = (int16_t)(yha << 8 | yla);
  rg.z = (int16_t)(zha << 8 | zla);
  gyroRawtoDPS();
  return gyro;
}

void MPU6050:: accelRawToG() {
  accel.x = ra.x/a_sens.factor;
  accel.y = ra.y/a_sens.factor;
  accel.z = ra.z/a_sens.factor;
}
void MPU6050::gyroRawtoDPS() {
  gyro.x = rg.x/g_sens.factor;
  gyro.y = rg.y/g_sens.factor;
  gyro.z = rg.z/g_sens.factor;
}

uint8_t MPU6050::readRegister8(uint8_t reg) {
  uint8_t value;
  Wire.beginTransmission(MPU6050_NAME);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(MPU6050_NAME, 1);
  value = Wire.read();
  Wire.endTransmission();
  return value;
}
void MPU6050::writeRegister16(uint8_t reg, uint16_t value) {
  Wire.beginTransmission(MPU6050_NAME);
  Wire.write(reg);
  Wire.write(value >> 8);
  Wire.write(value);
  Wire.endTransmission(true);
}
void MPU6050::writeRegister8(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(MPU6050_NAME);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission(true);
}

#endif
