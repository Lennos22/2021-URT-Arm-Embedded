#ifndef CPP_PISERIAL
#define CPP_PISERIAL

#include "runnable.h"
#include "PiSerial.h"

// Create a new serial Object using pins 15 and 18 (A4 is Tx, A1 is Rx)
Uart SerialPi(&sercom0, PI_RX_PIN, PI_TX_PIN, SERCOM_RX_PAD_1, UART_TX_PAD_0);

// Define Serial Interrupt Handlers (This is required for the feather M4)
void SERCOM0_0_Handler()
{
  SerialPi.IrqHandler();
}
void SERCOM0_1_Handler()
{
  SerialPi.IrqHandler();
}
void SERCOM0_2_Handler()
{
  SerialPi.IrqHandler();
}
void SERCOM0_3_Handler()
{
  SerialPi.IrqHandler();
}

//Initialises the Serial Connection
void setup_RaspPiUart(){
  //Using Serial 2 via SERCOM 4 on pins 16/17

  SerialPi.begin(SERIALPISPEED);
  delay(5);
  // Assign pins 15 & 18 SERCOM functionality
  pinPeripheral(PI_TX_PIN, PIO_SERCOM_ALT);
  pinPeripheral(PI_RX_PIN, PIO_SERCOM_ALT);
}

#endif
