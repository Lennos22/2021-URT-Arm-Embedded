#ifndef CPP_CDS92TIMERS
#define CPP_CDS92TIMERS

#include "runnable.h"
#include "CDS92Timers.h"


/*
This function is to setup the Internal Timers for interfacing with the CDS
controller and providing external functions for changing the frequency, enable,
and other low level functions.
*/


void setup_CDSTimer()
{
  // Set up the generic clock (GCLK7) to clock timer TCC0
  GCLK->GENCTRL[7].reg = GCLK_GENCTRL_DIV(1) |       // Divide the 48MHz clock source by divisor 1: 48MHz/1 = 48MHz
                         GCLK_GENCTRL_IDC |          // Set the duty cycle to 50/50 HIGH/LOW
                         GCLK_GENCTRL_GENEN |        // Enable GCLK7
                         GCLK_GENCTRL_SRC_DFLL;      // Select 48MHz DFLL clock source
                         //GCLK_GENCTRL_SRC_DPLL1;     // Select 100MHz DPLL clock source
                         //GCLK_GENCTRL_SRC_DPLL0;     // Select 120MHz DPLL clock source
  while (GCLK->SYNCBUSY.bit.GENCTRL7);               // Wait for synchronization

  GCLK->PCHCTRL[25].reg = GCLK_PCHCTRL_CHEN |        // Enable the TCC0 peripheral channel PAGE 155
                          GCLK_PCHCTRL_GEN_GCLK7;    // Connect generic clock 7 to TCC0/TCC1 as per page 156

//use clock 7 and pin 10 for TCC0:0
//use clock 7 and pin 5  for TCC1:0 (ulPin = 16)

  // Enable the peripheral multiplexer on pin D10
  PORT->Group[g_APinDescription[TCC00PIN].ulPort].PINCFG[g_APinDescription[TCC00PIN].ulPin].bit.PMUXEN = 1;

  // Set the D10 (PORT_PA20) peripheral multiplexer to peripheral (even port number) E(6/G): TCC0, Channel 0 PAGE 823
  PORT->Group[g_APinDescription[TCC00PIN].ulPort].PMUX[g_APinDescription[TCC00PIN].ulPin >> 1].reg |= PORT_PMUX_PMUXE(6); //number is halved because port multiplexing is only 4 bits wide
  //is synced with group G which is 0x06

  // Enable the peripheral multiplexer on pin D5
  PORT->Group[g_APinDescription[TCC10PIN].ulPort].PINCFG[g_APinDescription[TCC10PIN].ulPin].bit.PMUXEN = 1;

  // Set the D5 (PORT_PA16) peripheral multiplexer to peripheral (even port number) E(5/F): TCC1, Channel 0 PAGE 823
  PORT->Group[g_APinDescription[TCC10PIN].ulPort].PMUX[g_APinDescription[TCC10PIN].ulPin >> 1].reg |= PORT_PMUX_PMUXE(5); //number is halved because port multiplexing is only 4 bits wide


  TCC0->CTRLA.reg = TC_CTRLA_PRESCALER_DIV8 |        // Set prescaler to 8, 48MHz/8 = 6MHz
                    TC_CTRLA_PRESCSYNC_PRESC;        // Set the reset/reload to trigger on prescaler clock

  TCC0->WAVE.reg = TC_WAVE_WAVEGEN_MFRQ;             // Set-up TCC0 timer for Matched Frequency PWM
  while (TCC0->SYNCBUSY.bit.WAVE)                    // Wait for synchronization

  TCC0->CC[0].reg = MAXVAL24INT;                     // Set-up the CC (counter compare), channel 0 register for 1Hz, MAX 24 bits
  while (TCC0->SYNCBUSY.bit.CC0);                    // Wait for synchronization

  TCC1->CTRLA.reg = TC_CTRLA_PRESCALER_DIV8 |        // Set prescaler to 8, 48MHz/8 = 6MHz
                    TC_CTRLA_PRESCSYNC_PRESC;        // Set the reset/reload to trigger on prescaler clock

  TCC1->WAVE.reg = TC_WAVE_WAVEGEN_MFRQ;             // Set-up TCC1 timer for Matched Frequency PWM
  while (TCC1->SYNCBUSY.bit.WAVE)                    // Wait for synchronization

  TCC1->CC[0].reg = MAXVAL24INT;                     // Set-up the CC (counter compare), channel 0 register for 1Hz
  while (TCC1->SYNCBUSY.bit.CC0);                    // Wait for synchronization

  //enable double buffering
  TCC0->CTRLBCLR.bit.LUPD = 1;
  TCC1->CTRLBCLR.bit.LUPD = 1;

}

status_t writeFrequency(uint8_t timer, double frequency){
  uint32_t CC0_val = 3000000.0/frequency;

  if(CC0_val > MAXVAL24INT) return FAILURE;

  switch (timer){
    case 0:
      TCC0->CCBUF[0].reg = CC0_val;
      while (TCC0->SYNCBUSY.bit.CC0);
      break;
    case 1:
      TCC1->CCBUF[0].reg = CC0_val;
      while (TCC1->SYNCBUSY.bit.CC0);
      break;
    default:
      return FAILURE;

  }
  return SUCCESS;
}

void enableTimer(uint8_t timer){
  switch (timer){
    case 0:
      TCC0->CTRLA.bit.ENABLE = 1;                        // Enable timer TCC0
      while (TCC0->SYNCBUSY.bit.ENABLE);                 // Wait for synchronization
      break;

    case 1:
      TCC1->CTRLA.bit.ENABLE = 1;                        // Enable timer TCC0
      while (TCC1->SYNCBUSY.bit.ENABLE);                 // Wait for synchronization
      break;

    default:
      break;
  }
  return;
}

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

    default:
      break;
  }
  return;
}

//TODO Check that this actually works
float readFrequency(uint8_t timer){
  uint32_t registervalue;
  switch (timer){
    case 0:
      registervalue = TCC0->CC[0].reg & 0x00FFFFFF;
      break;
    case 1:
      registervalue = TCC1->CC[0].reg & 0x00FFFFFF;
      break;
    default:
      return 0.0;
  }
  return 3000000.0/registervalue;
}

#endif
