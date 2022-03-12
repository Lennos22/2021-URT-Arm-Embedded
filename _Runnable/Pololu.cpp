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
void setup_PololuUart(){

  //Using Serial 2 via SERCOM 4 on pins 16/17
  SerialPol.begin(SERIALPOLSPEED);
  delay(5);

  // Assign pins 16 & 17 SERCOM functionality
  pinPeripheral(POL_TX_PIN, PIO_SERCOM_ALT);
  pinPeripheral(POL_RX_PIN, PIO_SERCOM_ALT);

  // Assign error reading pin
  pinMode(POL_ERR_PIN, INPUT);
  setPolSerialSpeedAll();
}

// set up the simple motor controllers
void setup_PololuSMC(int* deviceNumbers, int numberDevices) {
  exitSafeStartAllSMC(deviceNumbers, numberDevices);
  // More code could be added here to implement motor settinf changes and that
}

// Sets the serial communication speed in the simple motor controller
void setPolSerialSpeedAll() {
  SerialPol.write(0xAA);
}

// Exits safe start for all of the devices in deviceNumbers
void exitSafeStartAllSMC(int *deviceNumbers, int numberDevices) {
  for(int i = 0; i < numberDevices; i++) {
    exitSafeStartSMC(deviceNumbers[i]);
  }
}

// Exits saft start mode on the device indicated by device number
void exitSafeStartSMC(int deviceNumber) {
  SerialPol.write(0xAA);
  SerialPol.write(deviceNumber);
  SerialPol.write(0x03);
}

// Reads a byte from the SerialPol, returns -1 if nothing read
int readByteAll() {
  char c;
  if(SerialPol.readBytes(&c, 1) == 0) {return -1;}
  return (byte)c;
}

// Sets the motor speed of device indicated. The speed value is a int with it's
// value between -3200 and 3200. With -3200 being full speed reverse, 3200 being
// full speed forward, and 0 being stopped.
void setMotorSpeedSMC(int deviceNumber, int speed) {
  Serial.print("Setting motor ");
  Serial.print(deviceNumber);
  Serial.print(" to speed ");
  Serial.println(speed);
  if(speed<-3200 || speed>3200) return;
  SerialPol.write(0xAA);
  SerialPol.write(deviceNumber);
  if(speed < 0) {
    SerialPol.write(0x06); // Command for reverse motor direction
    speed = -speed; // Get magnitude
  } else {
    SerialPol.write(0x05); // Forward motor direction
  }
  SerialPol.write((speed & 0x1F));
  SerialPol.write((speed >> 5) /*& 0x7F*/);
}

// Sets the breaks of motor indicated. The amount is between 0 and 32, with 0 being
// a full coast and 32 being full break.
void setMotorBreakSMC(int deviceNumber, int amount) {
  SerialPol.write(0xAA);
  SerialPol.write(deviceNumber);
  SerialPol.write(0x12);
  SerialPol.write(amount);
}

// Stops the motor indicated. This does two things: sets the target speed to 0,
// thereby stopping the motor (respecting set decceleration limits) and puts the
// device into Safe-Start mode if that is enabled.
void stopMotorSMC(int deviceNumber) {
  SerialPol.write(0xAA);
  SerialPol.write(deviceNumber);
  SerialPol.write(0x60);
}

// Sets limits for the device indicated's motor parameters. See the Motor Limit IDs
// section of Polou.h for the different ID's and potential values. Returns a integer
// to signify the sucess: 0 for all good, 1 for unable to set a forward param because
// of a hard motor limit, 2 for a reverse hard motor limit, and 3 for a forward and
// reverse hard motor limit.
unsigned char setMotorLimitSMC(int deviceNumber, unsigned char limitID, unsigned int limitValue) {
  SerialPol.write(0xAA);
  SerialPol.write(deviceNumber);
  SerialPol.write(0x22);
  SerialPol.write(limitID);
  SerialPol.write(limitValue & 0x7F);
  SerialPol.write(limitValue >> 7);
  return readByteAll();
}

// Requests the a variable from the specified motor. See the Variable IDs
// section of Pololu.h for the different ID's and potential values.
unsigned int getVariableSMC(int deviceNumber, unsigned char variableID) {
  SerialPol.write(0xAA);
  SerialPol.write(deviceNumber);
  SerialPol.write(0x21);
  SerialPol.write(variableID);
  return readByteAll() + 256*readByteAll();
}

// Prints the current status information of the named device.
void printStatusInformationSMC(int deviceNumber) {
  Serial.print("The current error status byte is: ");
  Serial.println(getVariableSMC(deviceNumber, ERROR_STATUS), BIN);
  Serial.print("The current limit status byte is: ");
  Serial.println(getVariableSMC(deviceNumber, LIMIT_STATUS), BIN);
  Serial.print("The last reset casue was: ");
  Serial.println(getVariableSMC(deviceNumber, RESET_FLAGS));
}

// As these will all be in servo mode, the target represents the pulse width to
// transmit in units of quarter-microseconds. A target value of 0 tells the Maestro
// to stop sending pulses to the servo.
void setTargetServo(int deviceNumber, int channelNumber, int target) {
  if(target < 0) return;
  Serial.print("Setting Servo motor ");
  Serial.print(deviceNumber);
  Serial.print(" to target ");
  Serial.println(target);
  SerialPol.write(0xAA);
  SerialPol.write(deviceNumber);
  SerialPol.write(0x04);
  SerialPol.write(channelNumber);
  SerialPol.write(target & 0x7F);
  SerialPol.write((target >> 7) & 0x7F);
}

// Limits the speed at which the specified servo and channel's output can change.
// This is in units of 0.25 microseconds per 10 milliseconds - eg 140 means 3.5
// microsecond per millisecond. 0 means no limit. So, speed in range of (0, 40 000)
void setSpeedLimitServo(int deviceNumber, int channelNumber, uint16_t speed) {
  SerialPol.write(0xAA);
  SerialPol.write(deviceNumber);
  SerialPol.write(0x07);
  SerialPol.write(channelNumber);
  SerialPol.write(speed & 0x7F);
  SerialPol.write((speed >> 7) & 0x7F);
}

// Limits the accelration of servo's output channel. Again, 0 means no limit. The
// units are (0.25 microseconds)/(10 milliseconds)/(80 milliseconds). This is a
// value between 0 and 255.
void setAccLimitServo(int deviceNumber, int channelNumber, uint16_t accel) {
  SerialPol.write(0xAA);
  SerialPol.write(deviceNumber);
  SerialPol.write(0x09);
  SerialPol.write(channelNumber);
  SerialPol.write(accel & 0x7F);
  SerialPol.write((accel >> 7) & 0x7F);
}

// Gets the positions from a specified channel. When the channel is configured
// to servo mode this is the current pulse width on the channel
unsigned int getPositionServo(int deviceNumber, int channelNumber) {
  SerialPol.write(0xAA);
  SerialPol.write(deviceNumber);
  SerialPol.write(0x10);
  SerialPol.write(channelNumber);
  return readByteAll() + 256*readByteAll();
}

/** Gets the errors from the servo named. The bits represent the following:
0 - Serial Signal Error
1 - Serial Overrun Error
2 - Serial Buffer Full
3 - Serial CRC Error
4 - Serial Protocal Error
5 - Serial Timeout
6 - Script Stack Error
7 - Script Call Stack Error
8 - Script Program Counter Error
*/
unsigned int getErrorsServo(int deviceNumber) {
  SerialPol.write(0xAA);
  SerialPol.write(deviceNumber);
  SerialPol.write(0x21);
  return readByteAll() + 256*readByteAll();
}

#endif
