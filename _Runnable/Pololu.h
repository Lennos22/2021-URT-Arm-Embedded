#ifndef H_POLOLU
#define H_POLOLU

#define SERIALPOLSPEED 19200

extern Uart SerialPol;

void setup_PololuUart();
void SERCOM4_0_Handler();
void SERCOM4_1_Handler();
void SERCOM4_2_Handler();
void SERCOM4_3_Handler();

#endif
