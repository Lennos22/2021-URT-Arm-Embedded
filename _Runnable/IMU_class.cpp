#ifndef CPP_IMU_CLASS_CPP
#define CPP_IMU_CPP

#include <Wire.h>
#include "runnable.h"
#include "I2C.h"
#include "IMU_class.h"

bool IMU::begin(int accSens, int  gyroSens, int bandwidth) {
  // Set Acc. and Gyro. Sensitivity
  /** byte error;

  Wire.beginTransmission(0x68);
  Wire.write(0x75);
  error = Wire.endTransmission();
  if(error == 0) {
     Serial.println("\n acknowledged \n ");
  } else {
    Serial.println("\n not acknowledged \n ");
  }
  Wire.beginTransmission(0x68);
  Wire.requestFrom(0x68, 1);

  byte value = Wire.read();
  error = Wire.endTransmission();

  Serial.print("The error value was: ");
  Serial.println(error);
  Serial.print("The register value was: 0x");
  Serial.println(value, HEX);*/

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
  accelG.x = 0;
  accelG.y = 0;
  accelG.z = 0;
  gyro.x = 0;
  gyro.y = 0;
  gyro.z = 0;

  isConnected();
  return true;
}

bool IMU::isConnected() {
  return read8(MPU6050_REGISTER_WHO_AM_I) == MPU6050_DEFAULT_ADDRESS;
}

void IMU::sleepMode() {
    writeRegister8(MPU6050_REGISTER_PWR_MGMT_1, 0b01000000);
}

void IMU::wakeUp() {
    write8(MPU6050_REGISTER_PWR_MGMT_1, 0b00000000);
}



Vector IMU::readAccel() {
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

  /**
  Serial.print("Accel: \nx \ n HIGH:");
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
  Serial.println((int16_t)(xha << 8 | xla)); */

  ra.x = (int16_t)(xha << 8 | xla);
  ra.y = (int16_t)(yha << 8 | yla);
  ra.z = (int16_t)(zha << 8 | zla);
  accelRawToG();
  return accelG;
}

void IMU:: accelRawToG() {
  accelG.x = ra.x/a_sens.factor;
  accelG.y = ra.y/a_sens.factor;
  accelG.z = ra.z/a_sens.factor;
}

Vector IMU::readGyro() {
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

  rg.x = (int16_t)(xha << 8 | xla);
  rg.y = (int16_t)(yha << 8 | yla);
  rg.z = (int16_t)(zha << 8 | zla);

  return rg;
}

int16_t IMU::readTemp() {
  int16_t rawTemp = read16(MPU6050_REG_TEMP_OUT_H);
  return (int16_t)(rawTemp/340 + 36.53);
}



void IMU::setGyroScale(int scale) {
  uint8_t value;
  uint8_t scale_8 = (uint8_t)scale;
  value = readRegister8(MPU6050_REG_GYRO_CONFIG);
  value &= 0b11100111;
  value |= scale_8;
  writeRegister8(MPU6050_REG_GYRO_CONFIG, value);

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

uint8_t IMU::getAccRange() {
  uint8_t value;
  value = readRegister8(MPU6050_REG_ACCEL_CONFIG);
  return value;
}

void IMU::setAccRange(int range) {
  uint8_t value;
  uint8_t range_8 = (uint8_t)range;

  value = readRegister8(MPU6050_REG_ACCEL_CONFIG);
  value &= 0b11100111;
  value |= range_8;
  writeRegister8(MPU6050_REG_ACCEL_CONFIG, range_8);

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
    a_sens.mode = PM_8_G;
    a_sens.factor = PM_8_G_FACT;
  }
}

uint8_t IMU::getGyroScale() {
  uint8_t value = readRegister8(MPU6050_REG_GYRO_CONFIG);
  return value;
}

void IMU::setDLPFBandwidth(int bandwidth) {
  bandwidth = (int8_t)bandwidth;
  uint8_t value = readRegister8(MPU6050_REGISTER_CONFIG);
  value = (value & 0b11111000) | bandwidth;
  writeRegister8(MPU6050_REGISTER_CONFIG, value);
}

uint8_t IMU::getBandwidth() {
  uint8_t value = readRegister8(MPU6050_REGISTER_CONFIG);
  return value &= 0b00000111;
}

int16_t IMU::readRegister16(uint8_t reg) {
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

int16_t IMU::read16(uint8_t reg) {
  Wire.beginTransmission(0x68);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 2);

  return int16_t((int8_t )Wire.read()<<8 | (int8_t)Wire.read());
}

byte IMU::read8(byte registerAddr) {
    Wire.beginTransmission(0x68);
    Wire.write(registerAddr);
    Serial.println(Wire.endTransmission());
    Wire.beginTransmission(0x68);
    Wire.requestFrom(0x68,1);
    byte value = Wire.read();
    Serial.print(Wire.endTransmission());
    return value;
}

void IMU::write8(byte registerAddr, byte value) {
    Wire.beginTransmission(MPU6050_DEFAULT_ADDRESS);
    Wire.write(registerAddr);
    Wire.write(value);
    Wire.endTransmission(true);
}

int8_t IMU::readRegister8(uint8_t reg) {
  int8_t value;
  Wire.beginTransmission(MPU6050_DEFAULT_ADDRESS);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(IMU_NAME, 1);
  value = Wire.read();
  Wire.endTransmission();
  return value;
}

void IMU::writeRegister16(uint8_t reg, uint16_t value) {
  Wire.beginTransmission(IMU_NAME);
  Wire.write(reg);
  Wire.write(value >> 8);
  Wire.write(value);
  Wire.endTransmission(true);
}

void IMU::writeRegister8(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(IMU_NAME);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission(true);
}

#endif
