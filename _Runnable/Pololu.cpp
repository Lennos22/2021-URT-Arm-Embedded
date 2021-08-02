#ifndef CPP_POLOLU
#define CPP_POLOLU

#include "runnable.h"
#include "Pololu.h"

uint8_t testmessage[3];

Uart SerialPol(&sercom4, POL_RX_PIN, POL_TX_PIN, SERCOM_RX_PAD_1, UART_TX_PAD_0);

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

void setup_PololuUart(){
  //Using Serial 2 via SERCOM 4 on pins 16/17

  SerialPol.begin(SERIALPOLSPEED);
  delay(5);
  // Assign pins 16 & 17 SERCOM functionality
  pinPeripheral(POL_TX_PIN, PIO_SERCOM_ALT);
  pinPeripheral(POL_RX_PIN, PIO_SERCOM_ALT);

  if(SENDSERIAL){
    testmessage[0] = 0xAA;
    testmessage[1] = 0xAB;
    testmessage[2] = 0xAC;
  }
}

#endif
