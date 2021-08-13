#ifndef CPP_POLOLU
#define CPP_POLOLU

#include "runnable.h"
#include "Pololu.h"

// Create a new serial Object using pins 16 and 17 (A2 is Tx, A3 is Rx)
Uart SerialPol(&sercom4, POL_RX_PIN, POL_TX_PIN, SERCOM_RX_PAD_1, UART_TX_PAD_0);

// Define Serial Interrupt Handlers (This is required for the feather M4)
void SERCOM4_0_Handler()
{
  SerialPol.IrqHandler();
}
void SERCOM4_1_Handler()
{
  SerialPol.IrqHandler();
}
void SERCOM4_2_Handler()
{
  SerialPol.IrqHandler();
}
void SERCOM4_3_Handler()
{
  SerialPol.IrqHandler();
}

//Initialises the Serial Connection
void setup_PololuUart(int* deviceNumbers, int numberDevices){
  //Using Serial 2 via SERCOM 4 on pins 16/17

  SerialPol.begin(SERIALPOLSPEED);
  delay(5);

  // Assign pins 16 & 17 SERCOM functionality
  pinPeripheral(POL_TX_PIN, PIO_SERCOM_ALT);
  pinPeripheral(POL_RX_PIN, PIO_SERCOM_ALT);

  setPolSerialSpeed();
  exitSafeStartAll(deviceNumbers, numberDevices);
}

// Sets the serial communication speed in the simple motor controller
void setPolSerialSpeed() {
  SerialPol.write(0xAA);
}

// Exits safe start for all of the devices in deviceNumbers
void exitSafeStartAll(int *deviceNumbers, int numberDevices) {
  for(int i = 0; i < numberDevices; i++) {
    exitSafeStart(deviceNumbers[i]);
  }
}

// Exits saft start mode on the device indicated by device number
void exitSafeStart(unsigned char deviceNumber) {
  SerialPol.write(0xAA);
  SerialPol.write(deviceNumber);
  SerialPol.write(0x03);
}

// Reads a byte from the SerialPol, returns -1 if nothing read
int readByte() {
  char c;
  if(SerialPol.readBytes(&c, 1) == 0) {return -1;}
  return (byte)c;
}

// Sets the motor speed of device indicated. The speed value is a int with it's
// value between -3200 and 3200. With -3200 being full speed reverse, 3200 being
// full speed forward, and 0 being stopped.
void setMotorSpeed(int deviceNumber, int speed) {
  SerialPol.write(0xAA);
  SerialPol.write(deviceNumber);
  if(speed < 0) {
    SerialPol.write(0x06); // Command for reverse motor direction
    speed = -speed; // Get magnitude
  } else {
    SerialPol.write(0x05); // Forward motor direction
  }
  SerialPol.write(speed & 0xF1);
  SerialPol.write(speed >> 5);
}

// Sets the breaks of motor indicated. The amount is between 0 and 32, with 0 being
// a full coast and 32 being full break.
void setMotorBreak(int deviceNumber, int amount) {
  SerialPol.write(0xAA);
  SerialPol.write(deviceNumber);
  SerialPol.write(0x12);
  SerialPol.write(amount);
}

// Stops the motor indicated. This does two things: sets the target speed to 0,
// thereby stopping the motor (respecting set decceleration limits) and puts the
// device into Safe-Start mode if that is enabled.
void stopMotor(int deviceNumber) {
  SerialPol.write(0xAA);
  SerialPol.write(deviceNumber);
  SerialPol.write(0x60);
}

// Sets limits for the device indicated's motor parameters. See the Motor Limit IDs
// section of Polou.h for the different ID's and potential values. Returns a integer
// to signify the sucess: 0 for all good, 1 for unable to set a forward param because
// of a hard motor limit, 2 for a reverse hard motor limit, and 3 for a forward and
// reverse hard motor limit.
unsigned char setMotorLimit(int deviceNumber, unsigned char limitID, unsigned int limitValue) {
  SerialPol.write(0xAA);
  SerialPol.write(deviceNumber);
  SerialPol.write(0x22);
  SerialPol.write(limitID);
  SerialPol.write(limitValue & 0x7F);
  SerialPol.write(limitValue >> 7);
  return readByte();
}

// Requests the a variable from teh specified motor. See the Variable IDs
// section of Polou.h for the different ID's and potential values.
unsigned int getVariable(int deviceNumber, unsigned char variableID) {
  SerialPol.write(0xAA);
  SerialPol.write(deviceNumber);
  SerialPol.write(0x21);
  SerialPol.write(variableID);
  return readByte() + 256*readByte();
}

#endif
