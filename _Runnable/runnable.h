#ifndef H_RUNNABLE
#define H_RUNNABLE

#include <Arduino.h>   // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function
#include "PinAssignment.h"
#include <SPI.h>
#include "PololuDeviceNames.h"

// GLOBAL FLAGS
#define SERIALMONITER 1
#define SENDSERIAL 0

#define status_t uint8_t
#define FAILURE 0
#define SUCCESS 1


#endif
