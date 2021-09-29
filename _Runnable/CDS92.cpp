#ifndef CPP_CDS92
#define CPP_CDS92

#include "runnable.h"
#include "CDS92.h"


/*
This function is to setup the Internal Timers for interfacing with the CDS
controller and providing external functions for changing the frequency, enable,
and other low level functions.
*/

void setup_CDS(){
  setup_CDSTimer();
  disableAllTimers();

  setup_CDS_SPI();

  //setup fault input pins
  pinMode(CDS_FAULT_1, INPUT);
  pinMode(CDS_FAULT_2, INPUT);
  pinMode(CDS_FAULT_3, INPUT);



  //write initial values and initialise registers
  digitalWrite(SPI_SS, LOW);
  delay(200);
  SPI.transfer16(CDS_MSG_EMPTY);
  delay(200);
  digitalWrite(SPI_SS, HIGH);
  //enable voltage level converters
  pinMode(V_LEVEL_OE, OUTPUT);
  digitalWrite(V_LEVEL_OE, HIGH);
  //enable output of shift registers
  enableRegisters();
}

void setup_CDS_SPI(){
  SPI.begin();
  SPI.beginTransaction(SPISettings(5000, LSBFIRST, SPI_MODE0));
  pinMode(SPI_SS, OUTPUT);
  pinMode(SPI_OE, OUTPUT);
  digitalWrite(SPI_OE, HIGH);
}

void setup_CDSTimer()
{
  /*
  Generic Clock 7 is used for clock timers TCC0 and TCC1
  Generic CLock 9 is used for clock timers TCC2

  TCC0:0 is on pin 10 (PA20)
  TCC1:0 is on pin 5  (PA16)
  TCC2:0 is on pin 4  (PA14)
  */
  // Set up the generic clock (GCLK7) to clock timer TCC0 and TCC1
  GCLK->GENCTRL[7].reg = GCLK_GENCTRL_DIV(128) |       // Divide the 48MHz clock source by divisor 128: 48MHz/128 = 375kHz
                         GCLK_GENCTRL_IDC |          // Set the duty cycle to 50/50 HIGH/LOW
                         GCLK_GENCTRL_GENEN |        // Enable GCLK7
                         GCLK_GENCTRL_SRC_DFLL;      // Select 48MHz DFLL clock source
                         //GCLK_GENCTRL_SRC_DPLL1;     // Select 100MHz DPLL clock source
                         //GCLK_GENCTRL_SRC_DPLL0;     // Select 120MHz DPLL clock source
  while (GCLK->SYNCBUSY.bit.GENCTRL7);               // Wait for synchronization

  GCLK->PCHCTRL[25].reg = GCLK_PCHCTRL_CHEN |        // Enable the TCC0 peripheral channel PAGE 155
                          GCLK_PCHCTRL_GEN_GCLK7;    // Connect generic clock 7 to TCC0/TCC1 as per page 156


  // Set up the generic clock (GCLK6) to clock timer TCC2
  GCLK->GENCTRL[6].reg = GCLK_GENCTRL_DIV(128) |       // Divide the 48MHz clock source by divisor 128: 48MHz/128 = 375kHz
                         GCLK_GENCTRL_IDC |          // Set the duty cycle to 50/50 HIGH/LOW
                         GCLK_GENCTRL_GENEN |        // Enable GCLK6
                         GCLK_GENCTRL_SRC_DFLL;      // Select 48MHz DFLL clock source
                         //GCLK_GENCTRL_SRC_DPLL1;     // Select 100MHz DPLL clock source
                         //GCLK_GENCTRL_SRC_DPLL0;     // Select 120MHz DPLL clock source
  while (GCLK->SYNCBUSY.bit.GENCTRL6);               // Wait for synchronization

  GCLK->PCHCTRL[29].reg = GCLK_PCHCTRL_CHEN |        // Enable the TCC2 peripheral channel PAGE 156
                          GCLK_PCHCTRL_GEN_GCLK6;    // Connect generic clock 9 to TCC2

  // Enable the peripheral multiplexer on pin 10 (PA20)
  PORT->Group[g_APinDescription[TCC00PIN].ulPort].PINCFG[g_APinDescription[TCC00PIN].ulPin].bit.PMUXEN = 1;
  // Set the D10 (PORT_PA20) peripheral multiplexer to peripheral (even port number) E(6/G): TCC0, Channel 0 PAGE 823
  PORT->Group[g_APinDescription[TCC00PIN].ulPort].PMUX[g_APinDescription[TCC00PIN].ulPin >> 1].reg |= PORT_PMUX_PMUXE(6); //number is halved because port multiplexing is only 4 bits wide
  //is synced with group G which is 0x06

  // Enable the peripheral multiplexer on pin D5 (PA16)
  PORT->Group[g_APinDescription[TCC10PIN].ulPort].PINCFG[g_APinDescription[TCC10PIN].ulPin].bit.PMUXEN = 1;
  // Set the D5 (PORT_PA16) peripheral multiplexer to peripheral (even port number) E(5/F): TCC1, Channel 0 PAGE 823
  PORT->Group[g_APinDescription[TCC10PIN].ulPort].PMUX[g_APinDescription[TCC10PIN].ulPin >> 1].reg |= PORT_PMUX_PMUXE(5); //number is halved because port multiplexing is only 4 bits wide

  // Enable the peripheral multiplexer on pin D4 (PA14)
  PORT->Group[g_APinDescription[TCC20PIN].ulPort].PINCFG[g_APinDescription[TCC20PIN].ulPin].bit.PMUXEN = 1;
  // Set the D4 (PORT_PA14) peripheral multiplexer to peripheral (even port number) E(5/F): TCC2, Channel 0 PAGE 823
  PORT->Group[g_APinDescription[TCC20PIN].ulPort].PMUX[g_APinDescription[TCC20PIN].ulPin >> 1].reg |= PORT_PMUX_PMUXE(5); //number is halved because port multiplexing is only 4 bits wide


  TCC0->CTRLA.reg = TC_CTRLA_PRESCALER_DIV8 |        // Set prescaler to 8, 375kHz/8 = 46.875kHz
                    TC_CTRLA_PRESCSYNC_PRESC;        // Set the reset/reload to trigger on prescaler clock
  TCC0->WAVE.reg = TC_WAVE_WAVEGEN_MFRQ;             // Set-up TCC0 timer for Matched Frequency PWM
  while (TCC0->SYNCBUSY.bit.WAVE)                    // Wait for synchronization
  TCC0->CC[0].reg = MAXVAL16INT;                     // Set-up the CC (counter compare), channel 0 register for 1Hz, MAX 24 bits
  while (TCC0->SYNCBUSY.bit.CC0);                    // Wait for synchronization

  TCC1->CTRLA.reg = TC_CTRLA_PRESCALER_DIV8 |        // Set prescaler to 8, 375kHz/8 = 46.875kHz
                    TC_CTRLA_PRESCSYNC_PRESC;        // Set the reset/reload to trigger on prescaler clock
  TCC1->WAVE.reg = TC_WAVE_WAVEGEN_MFRQ;             // Set-up TCC1 timer for Matched Frequency PWM
  while (TCC1->SYNCBUSY.bit.WAVE)                    // Wait for synchronization
  TCC1->CC[0].reg = MAXVAL16INT;                     // Set-up the CC (counter compare), channel 0 register for 1Hz
  while (TCC1->SYNCBUSY.bit.CC0);                    // Wait for synchronization

  TCC2->CTRLA.reg = TC_CTRLA_PRESCALER_DIV8 |        // Set prescaler to 8, 375kHz/8 = 46.875kHz
                    TC_CTRLA_PRESCSYNC_PRESC;        // Set the reset/reload to trigger on prescaler clock
  TCC2->WAVE.reg = TC_WAVE_WAVEGEN_MFRQ;             // Set-up TCC1 timer for Matched Frequency PWM
  while (TCC2->SYNCBUSY.bit.WAVE)                    // Wait for synchronization
  TCC2->CC[0].reg = MAXVAL16INT;                     // Set-up the CC (counter compare), channel 0 register for 1Hz
  while (TCC2->SYNCBUSY.bit.CC0);                    // Wait for synchronization

  //enable double buffering
  TCC0->CTRLBCLR.bit.LUPD = 1;
  TCC1->CTRLBCLR.bit.LUPD = 1;
  TCC2->CTRLBCLR.bit.LUPD = 1;

}



status_t writeFrequency(uint8_t timer, double frequency){
  uint16_t CC0_val = FREQ_CONVERSION/frequency;

  if(CC0_val > MAXVAL16INT) return FAILURE;

  switch (timer){
    case 0:
      if(TCC0->CTRLA.bit.ENABLE == 0){
        TCC0->CC[0].reg = CC0_val;
      } else {
        TCC0->CCBUF[0].reg = CC0_val;
        while (TCC0->SYNCBUSY.bit.CC0);
      }
      break;
    case 1:
      if(TCC1->CTRLA.bit.ENABLE == 0){
        TCC1->CC[0].reg = CC0_val;
      } else {
        TCC1->CCBUF[0].reg = CC0_val;
        while (TCC1->SYNCBUSY.bit.CC0);
      }
      break;
    case 2:
      if(TCC2->CTRLA.bit.ENABLE == 0){
        TCC2->CC[0].reg = CC0_val;
      } else {
        TCC2->CCBUF[0].reg = CC0_val;
        while (TCC2->SYNCBUSY.bit.CC0);
      }
      break;
    default:
      return FAILURE;
  }
  return SUCCESS;
}

//enabling a timer turns on the output
void enableTimer(uint8_t timer){
  switch (timer){
    case 0:
      TCC0->CTRLA.bit.ENABLE = 1;                        // Enable timer TCC0
      while (TCC0->SYNCBUSY.bit.ENABLE);                 // Wait for synchronization
      TCC0->CTRLBCLR.bit.LUPD = 1;
      break;
    case 1:
      TCC1->CTRLA.bit.ENABLE = 1;                        // Enable timer TCC0
      while (TCC1->SYNCBUSY.bit.ENABLE);                 // Wait for synchronization
      TCC0->CTRLBCLR.bit.LUPD = 1;
      break;
    case 2:
      TCC2->CTRLA.bit.ENABLE = 1;                        // Enable timer TCC0
      while (TCC2->SYNCBUSY.bit.ENABLE);                 // Wait for synchronization
      TCC0->CTRLBCLR.bit.LUPD = 1;
      break;
    default:
      break;
  }
  return;
}

void enableAllTimers(){
  enableTimer(0);
  enableTimer(1);
  enableTimer(2);
}

//disabling a timer stops the output
void disableTimer(uint8_t timer){
  switch (timer){
    case 0:
      TCC0->CTRLA.bit.ENABLE = 0;                        // Enable timer TCC0
      while (TCC0->SYNCBUSY.bit.ENABLE);                 // Wait for synchronization
      break;
    case 1:
      TCC1->CTRLA.bit.ENABLE = 0;                        // Enable timer TCC0
      while (TCC1->SYNCBUSY.bit.ENABLE);                 // Wait for synchronization
      break;
    case 2:
      TCC2->CTRLA.bit.ENABLE = 0;                        // Enable timer TCC0
      while (TCC2->SYNCBUSY.bit.ENABLE);                 // Wait for synchronization
      break;
    default:
      break;
  }
  return;
}

void disableAllTimers(){
  disableTimer(0);
  disableTimer(1);
  disableTimer(2);
}

//TODO Check that this actually works
float readFrequency(uint8_t timer){
  uint16_t registervalue;
  switch (timer){
    case 0:
      registervalue = TCC0->CC[0].reg & 0xFFFF;
      break;
    case 1:
      registervalue = TCC1->CC[0].reg & 0xFFFF;
      break;
    case 2:
      registervalue = TCC2->CC[0].reg & 0xFFFF;
      break;
    default:
      return 0.0;
  }
  return FREQ_CONVERSION/(float)registervalue;
}

void enableRegisters(){
  digitalWrite(SPI_OE, LOW);
}

void disableRegisters(){
  digitalWrite(SPI_OE, HIGH);
}


void SPItest(){
  for(int i =0; i<10; i++ ){
    digitalWrite(SPI_SS, LOW);
    delay(1);
    SPI.transfer(0xF0);
    delay(1);
    digitalWrite(SPI_SS, HIGH);
    delay(1000);
    digitalWrite(SPI_SS, LOW);
    delay(1);
    SPI.transfer(0x0F);
    delay(1);
    digitalWrite(SPI_SS, HIGH);
    delay(1000);
  }
}

void CDSEnableControllers(){
  digitalWrite(SPI_SS, LOW);
  SPI.transfer16(CDS1_EN|CDS2_EN|CDS3_EN);
  digitalWrite(SPI_SS, HIGH);
}

void CDSDisableControllers(){
  digitalWrite(SPI_SS, LOW);
  SPI.transfer16(CDS_MSG_EMPTY);
  digitalWrite(SPI_SS, HIGH);
}

#endif
