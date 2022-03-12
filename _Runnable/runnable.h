#ifndef H_RUNNABLE
#define H_RUNNABLE

#include <Arduino.h>   // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function
#include "PinAssignment.h"
#include <SPI.h>
#include "PololuDeviceNames.h"
#include "VL53L1X.h"

// GLOBAL FLAGS
#define DEBUG true

#define SERIALMONITER     true
#define MPU_CONNECTED     false
#define ENCODER_CONNECTED false
#define CDS_CONNECTED     true
#define POLOLU_CONNECTED  true
#define I2C_CONNECTED     false


#define status_t uint8_t
#define FAILURE 0
#define SUCCESS 1


//function definitions
void setup_USBserial();
void test_Encoder();
void setup_MPU();
void print_MPU();

#endif
