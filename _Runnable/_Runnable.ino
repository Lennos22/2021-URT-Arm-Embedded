#include "runnable.h"
#include "Pololu.h"
#include "CDS92Timers.h"



void setup_USBserial(){
  Serial.begin(9600);
  while (!Serial);
  delay(500);
  Serial.println("Serial Connected");
}

void setup() {
  //setup_TCC();
  setup_PololuUart();
  //setup_encoder();
  //setup_SPI();
  if(SERIALMONITER) setup_USBserial();
}

void loop() {
  if(SERIALMONITER){
    if (SerialPol.available() > 0) {
      delay(100);
      Serial.print(" -> 0x");
      size_t readsize = SerialPol.available();
      size_t i;
      for(i = 0; i<readsize; i = i+1){
        Serial.print(SerialPol.read(), HEX);
      }
      Serial.println();
    }
  }

  if(SENDSERIAL){
    SerialPol.write(testmessage, 3);
    SerialPol.flush();
    delay(1000);
  }
  delay(100);

  // put your main code here, to run repeatedly:

  // CYCLICAL 100Hz or higher
  // Read Encoders - calculate vel
  // If needed - Calculate PID and Apply Commands

  // ON SERIAL REPORT_DATA_CMD
  // Current Position and Velocity of 6 motors
  // Current Acceletomoeter Data via

  // ON SERIAL MOTOR_CMD
  // Write to CDS SPI
  // Change CDS Frequencies
  // Write to Pololu Uart
}
