//Angle Sensor

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
  uint8_t readName = setAddress(newName);

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
  return zeroPos;
}

uint16_t AS5048::readAngleRaw() {
  // uint8_t magHigh = readRegister8(AS5048_MAG_HIGH_REG);
  // uint8_t magLow = readRegister8(AS5048_MAG_LOW_REG);
  uint16_t raw = readRegister16(AS5048_ANG_HIGH_REG);
  return raw;
}

uint16_t AS5048::readAngleFromZero() {
  // uint8_t angleHigh = readRegister8(AS5048_ANG_HIGH_REG);
  // uint8_t angleLow = readRegister8(AS5048_ANG_LOW_REG);
  // Serial.print("High Byte: ");
  // Serial.print(angleHigh << 6, BIN);
  // Serial.print(", Low Byte: ");
  // Serial.print(angleLow & 0b00111111, BIN);
  // int angle = (angleHigh << 6) | (angleLow & 0b00111111);
  // Serial.print(", The total: ");
  // Serial.println(angle, BIN);
  uint16_t angle = readRegister16(AS5048_ANG_HIGH_REG);
  return angle;
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

uint16_t AS5048::readRegister16(uint8_t reg) {
  Wire.beginTransmission(name);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(name, 2);

  uint8_t highByte = Wire.read();
  uint8_t lowByte = Wire.read();

  uint16_t value = ((uint16_t)highByte << 6) | (lowByte & 0b00111111);
  return value;
}

void AS5048::writeRegister8(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(name);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission(true);
}

double AS5048::getPositionRaw() {
  return AS5048_RESOLUTION_FACTOR*((double)readAngleRaw());
}

double AS5048::getPositionFromZero() {
  return AS5048_RESOLUTION_FACTOR*((double)readAngleFromZero());
}

uint8_t AS5048::readAddress() {
  uint8_t address = readRegister8(AS5048_I2C_ADDRESS_REG);
  return (address << 2)|(0b1000000);
}

uint8_t AS5048::getErrorByte() {
  uint8_t error = readRegister8(AS5048_DIAG_REG);
  return error &= 0b00001111;
}

uint8_t AS5048::getGainValue() {
  return readRegister8(AS5048_AUTO_GAIN_REG);
}

uint8_t AS5048::setAddress(uint8_t newAddress) {
  if (newAddress > 127 || newAddress < 0) {
    Serial.println("The new adress was invalid, address is a 7-bit uint.");
    return name;
  }
  // The 2 least significant bits are from pins, so these cannot be programmed.
  // Hence, the new name should have 0's for those bits, or (equivilently) be
  // divisable by 4.
  if (newAddress % 4 != 0) {
    Serial.println("The new adress was invalid, must be divisable by 4.");
    return name;
  }

  uint8_t toWrite;
  // 7th bit is internally set
  if (newAddress > 63) {
    toWrite = newAddress & 0b0111100;
  } else {
    toWrite = newAddress | 0b1000000;
    toWrite &= 0b1111100;
  }
  toWrite = toWrite >> 2;
  Serial.print("Before: ");
  Serial.println(newAddress, BIN);
  Serial.print("After: ");
  Serial.println(toWrite, BIN);
  Serial.print("Current saved name: ");
  Serial.println(name, BIN);

  writeRegister8(AS5048_I2C_ADDRESS_REG, toWrite);
  name = newAddress;

  uint8_t readName = readAddress();
  Serial.print("Newly read name: ");
  Serial.println(readName, BIN);
  Serial.print("new saved name: ");
  Serial.println(name, BIN);
  return readName;
}

#endif
