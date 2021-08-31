#ifndef H_PISERIAL
#define H_PISERIAL

#define SERIALPISPEED 19200

extern Uart SerialPi;

void setup_RaspPiUart();
void SERCOM0_0_Handler();
void SERCOM0_1_Handler();
void SERCOM0_2_Handler();
void SERCOM0_3_Handler();

#endif
