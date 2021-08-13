#include "runnable.h"
#include "Pololu.h"
#include "CDS92Timers.h"

uint8_t testmessage[3];

void setup_USBserial(){
  Serial.begin(9600);
  while (!Serial);
  delay(500);
  Serial.println("Serial Connected");
}

void setup() {
  //setup_TCC();
  int polDeviceNames[1] = {DEFAULT_NAME};

  setup_PololuUart(polDeviceNames, 1);
  //setup_encoder();
  //setup_SPI();
  if(SERIALMONITER) setup_USBserial();

  if(SENDSERIAL){
    testmessage[0] = 0xAA;
    testmessage[1] = 0xAB;
    testmessage[2] = 0xAB;
  }

  Serial.print("The default brake duration is:");
  Serial.println(getVariable(DEFAULT_NAME, MAX_FORWARD_BRAKE_DURATION));
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
  /**
  if(SENDSERIAL){
    SerialPol.write(testmessage, 3);
    SerialPol.flush();
    delay(2000);
  }
  delay(100);
  **/
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
  Serial.print("The temperature is: ");
  Serial.println(getVariable(DEFAULT_NAME, TEMPERATURE));
  Serial.print("The input voltage is: ");
  Serial.println(getVariable(DEFAULT_NAME, INPUT_VOLTAGE));
  delay(500);



  setMotorLimit(DEFAULT_NAME, BOTH_DIRECT_SPEED, 3200/2);
  Serial.print("The motor speed limit is: ");
  Serial.println(getVariable(DEFAULT_NAME, MAX_FORWARD_SPEED));
  delay(500);
  setMotorLimit(DEFAULT_NAME, FORWARD_SPEED, 3200);
  Serial.print("The forward motor speed limit is: ");
  Serial.println(getVariable(DEFAULT_NAME, MAX_FORWARD_SPEED));
  delay(500);
  setMotorLimit(DEFAULT_NAME, BACKWARD_SPEED, 3200/4);
  Serial.print("The backward motor speed limit is: ");
  Serial.println(getVariable(DEFAULT_NAME, MAX_BACKWARD_SPEED));
  delay(500);
}
