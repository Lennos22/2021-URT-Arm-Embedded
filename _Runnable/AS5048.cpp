#ifndef AS5048_CPP
#define AS5048_CPP

#include <Wire.h>
#include "AS5048.h"
#include "runnable.h"

bool AS5048::begin() {
  name = AS5048_DEFAULT_ADDRESS;
  uint8_t readName = readRegister8(AS5048_I2C_ADDRESS_REG);
  readName << 2;
  readName |= 0b1000000;
  if(readName != name) {
    Serial.print("The name from the register is: ");
    Serial.println(readName);
    Serial.print("The default name is: ");
    Serial.println(name);
    Serial.println("These should match!!");
    return false;
  }
  return true;
}

bool AS5048::begin(uint8_t newName) {
  name = AS5048_DEFAULT_ADDRESS;
  Serial.println("The device name will be set to: ");
  Serial.println(newName);
  uint8_t nameToWrite = newName;
  nameToWrite &= 0b01111111; // the 8th bit is inverted
  nameToWrite >> 2; // the 2 least significant bits are from pin values A2/1
  writeRegister8(AS5048_I2C_ADDRESS_REG, nameToWrite);
  name = newName;
  uint8_t readName = readRegister8(AS5048_I2C_ADDRESS_REG);
  readName << 2;
  readName |= 0b1000000;
  if(readName == newName) {
    Serial.print("The device name has been set to: ");
    Serial.println(newName);
    return true;
  }
  return false;
}

void AS5048::programZeroPositionOTP() {
  int error = setZeroPosition(true);
  if(error == -1) {
    Serial.println("There was an error reading the existing value. Burn was not begun");
  } else if (error == 1) {
    Serial.println("The angle read to varify the zero-position was incorrect, Burn aborted");
  } else {
    Serial.println("Zero position sucessfully programmed.");
  }
}

int AS5048::setZeroPosition(bool burn) {
  writeRegister8(AS5048_ZERO_POS_HIGH_REG, 0);
  writeRegister8(AS5048_ZERO_POS_LOW_REG, 0);

  int error = readRegister8(AS5048_DIAG_REG) & 0x0F;
  if(error == 1) { // NO error
    uint8_t angleHigh = readRegister8(AS5048_ANG_HIGH_REG);
    uint8_t angleLow = readRegister8(AS5048_ANG_LOW_REG);
    zeroPos = (angleHigh << 6) + (angleLow & 0x3F);
    writeRegister8(AS5048_ZERO_POS_HIGH_REG, angleHigh);
    writeRegister8(AS5048_ZERO_POS_LOW_REG, angleLow);
  } else {
    return -1;
  }

  if(burn == true) { // OTP Proceedure
    writeRegister8(AS5048_PROG_CONT_REG, 0b00000001); // Allow Porgramming
    writeRegister8(AS5048_PROG_CONT_REG, 0b00001000); // Begin Burn
    int angle = readAngleFromZero();
    if(angle == 0) {
      writeRegister8(AS5048_PROG_CONT_REG, 0b01000000); // Verify
    } else {
      return 1;
    }
  }
  zeroPos = readAngleRaw();
  return 0;
}

int AS5048::readZeroPosition() {
  uint8_t angleHigh = readRegister8(AS5048_ZERO_POS_HIGH_REG);
  uint8_t angleLow = readRegister8(AS5048_ZERO_POS_LOW_REG);
  zeroPos = (angleHigh << 6) + (angleLow & 0x3F);
}

int  AS5048::readAngleRaw() {
  uint8_t magHigh = readRegister8(AS5048_MAG_HIGH_REG);
  uint8_t magLow = readRegister8(AS5048_MAG_LOW_REG);
  return (magHigh << 6) + (magLow & 0x3F);
}

int  AS5048::readAngleFromZero() {
  uint8_t angleHigh = readRegister8(AS5048_ANG_HIGH_REG);
  uint8_t angleLow = readRegister8(AS5048_ANG_LOW_REG);
  return (angleHigh << 6) + (angleLow & 0x3F);
}

uint8_t AS5048::readRegister8(uint8_t reg) {
  uint8_t value;
  Wire.beginTransmission(name);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(name, 1);
  value = Wire.read();
  Wire.endTransmission();
  return value;
}

void AS5048::writeRegister8(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(name);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission(true);
}
#endif
