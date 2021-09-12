#ifndef AS5048_H
#define AS5048_H

#define AS5048_PROG_CONT_REG      (0x03)
#define AS5048_I2C_ADDRESS_REG    (0x15)
#define AS5048_ZERO_POS_HIGH_REG  (0x16)
#define AS5048_ZERO_POS_LOW_REG   (0x17)
#define AS5048_AUTO_GAIN_REG      (0xFA)
#define AS5048_DIAG_REG           (0xFB)
#define AS5048_MAG_HIGH_REG       (0xFC)
#define AS5048_MAG_LOW_REG        (0xFD)
#define AS5048_ANG_HIGH_REG       (0xFE)
#define AS5048_ANG_LOW_REG        (0xFF)

#define AS5048_DEFAULT_ADDRESS    (0x40)
#define AS5048_RESOLUTION_FACTOR  (0.0219)

class AS5048 {
  public:
    bool begin(); // initialiser with the defaut name
    bool begin(uint8_t name); // initialiser with a non-defualt name
    int readZeroPosition(); // returns the zero position that is saved in the regsiter
    uint8_t readAddress(); // returns the current address of the device
    double getPositionFromZero(); // returns the value from the zero point in degrees
    double getPositionRaw(); // returns the raw reading in degrees
    uint8_t getErrorByte(); // Gets the error byte
    uint8_t getGainValue(); // Gets the gain value
    uint8_t setAddress(uint8_t newAddress); // set the 5 most significant bits of 7-bit adress

  private:
    uint16_t name, zeroPos;
    int readAngleRaw(); // returns the angle (from absolutue zero, not set point)
    int readAngleFromZero(); // reads the angle fromt he set zero position
    void programZeroPositionOTP(); // OTP to set the zero position
    uint8_t readRegister8(uint8_t reg); // reads a 8-bit register
    void writeRegister8(uint8_t reg, uint8_t value); // writes to a 8-bit register
    int setZeroPosition(bool burn); // sets the zero position, bollean to burn or not
};
#endif
