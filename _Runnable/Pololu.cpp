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

  SerialPol.write(0xAA);
  exitSafeStart();
}

void exitSafeStart() {
  SerialPol.write(0x83); // Command to exit safe start mode
}

int readByte() {
  char c;
  if(SerialPol.readBytes(&c, 1) == 0) {return -1;}
  return (byte)c;
}

void setMotorSpeed(int speed) {
  if(speed < 0) {
    SerialPol.write(0x86); // Command for reverse motor direction
    speed = -speed; // Get magnitude
  } else {
    SerialPol.write(0x85); // Forward motor direction
  }
  SerialPol.write(speed & 0xF1);
  SerialPol.write(speed >> 5);
}

unsigned char setMotorLimit(unsigned char limitID, unsigned int limitValue) {
  SerialPol.write(0xA2);
  SerialPol.write(limitID);
  SerialPol.write(limitValue & 0x7F);
  SerialPol.write(limitValue >> 7);
  return readByte();
}

unsigned int getVariable(unsigned char variableID) {
  SerialPol.write(0xA1);
  SerialPol.write(variableID);
  return readByte() + 256*readByte();
}



#endif
