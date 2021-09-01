#ifndef CPP_IMU_CLASS_CPP
#define CPP_IMU_CPP

#include <Wire.h>
#include "runnable.h"
#include "I2C.h"
#include "IMU_class.h"

bool IMU::begin(int accSens, int  gyroSens, int bandwidth) {
  Serial.println("in IMU begin");
  // Set Acc. and Gyro. Sensitivity
  wakeUp();
  Serial.println("Woke it up");
  if(!isConnected()) {
    return false;
  }
  Serial.println("Connected");

  setAccRange(accSens);
  setGyroScale(gyroSens);
  setDLPFBandwidth(bandwidth);

  Serial.println("Set sensitivity and filter");
  // Initiate the raw measurements to 0.
  ra.x = 0;
  ra.y = 0;
  ra.z = 0;
  rg.x = 0;
  rg.y = 0;
  rg.z = 0;
  return true;
}

bool IMU::isConnected() {
  Serial.println("the two values being comapred are:");
  Serial.println((uint8_t)read8(MPU6050_REGISTER_WHO_AM_I));
  Serial.println((int)MPU6050_DEFAULT_ADDRESS);
  return read8(MPU6050_REGISTER_WHO_AM_I) == MPU6050_DEFAULT_ADDRESS;
}

void IMU::sleepMode() {
    writeRegister8(MPU6050_REGISTER_PWR_MGMT_1, 0b01000000);
}

void IMU::wakeUp() {
    write8(MPU6050_REGISTER_PWR_MGMT_1, 0b00000000);
    Serial.println("Ended write");
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

byte IMU::read8(byte registerAddr) {
    Wire.beginTransmission(MPU6050_DEFAULT_ADDRESS);
    Wire.write(registerAddr);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_DEFAULT_ADDRESS,1,true);
    byte value = Wire.read();
    Wire.endTransmission();
    return value;
}

byte IMU::write8(byte registerAddr, byte value) {
    Wire.beginTransmission(MPU6050_DEFAULT_ADDRESS);
    Serial.println("Begun transmission");
    Wire.write(registerAddr);
    Serial.println("Wrote Register");
    Wire.write(value);
    Serial.println("Wrote value");
    Wire.endTransmission(true);
    Serial.println("Ended Transmission");
}

int8_t IMU::readRegister8(uint8_t reg) {
  int8_t value;
  Serial.println("trying to read what's already in register");
  Wire.beginTransmission(IMU_NAME);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(IMU_NAME, 1);
  Serial.println("Requested for value");
  Serial.println("trying to read value");
  value = Wire.read();
  Wire.endTransmission();
  Serial.println("got value, will return it");
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

Vector IMU::readRawAccel() {
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

Vector IMU::readRawGyr() {
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

void IMU::setAccRange(int range) {
  uint8_t value;
  uint8_t range_8 = (uint8_t)range;
  Serial.println("trying to set range");
  value = readRegister8(MPU6050_REG_ACCEL_CONFIG);
  value &= 0b11100111;
  value |= range_8;
  writeRegister8(MPU6050_REG_ACCEL_CONFIG, value);

  if(range == PM_250_DEG_SEC) {
    a_sens.mode = PM_250_DEG_SEC;
    a_sens.factor = PM_250_DEG_SEC_FACT;
  } else if(range == PM_500_DEG_SEC) {
    a_sens.mode = PM_500_DEG_SEC;
    a_sens.factor = PM_500_DEG_SEC_FACT;
  } else if(range == PM_1000_DEG_SEC) {
    a_sens.mode = PM_1000_DEG_SEC;
    a_sens.factor = PM_1000_DEG_SEC_FACT;
  } else {
    a_sens.mode = PM_2000_DEG_SEC;
    a_sens.factor = PM_2000_DEG_SEC_FACT;
  }
}

uint8_t IMU::getAccRange() {
  uint8_t value;
  value = readRegister8(MPU6050_REG_ACCEL_CONFIG);
  return value;
}

void IMU::setGyroScale(int scale) {
  uint8_t value;
  uint8_t scale_8 = (uint8_t)scale;

  value = readRegister8(MPU6050_REG_GYRO_CONFIG);
  value &= 0b11100111;
  value |= scale_8;
  writeRegister8(MPU6050_REG_GYRO_CONFIG, scale_8);

  if(scale == PM_2_G) {
    g_sens.mode = PM_2_G;
    g_sens.factor = PM_2_G_FACT;
  } else if(scale == PM_4_G) {
    g_sens.mode = PM_4_G;
    g_sens.factor = PM_4_G_FACT;
  } else if(scale == PM_8_G) {
    g_sens.mode = PM_8_G;
    g_sens.factor = PM_8_G_FACT;
  } else {
    g_sens.mode = PM_8_G;
    g_sens.factor = PM_8_G_FACT;
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

#endif
