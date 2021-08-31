#include "runnable.h"
#include "Pololu.h"
#include "CDS92Timers.h"
#include "PiSerial.h"

uint8_t testmessage[3];

void setup_USBserial(){
  Serial.begin(9600);
  while (!Serial);
  delay(500);
  Serial.println("Serial Connected");
}

void setup() {
<<<<<<< _Runnable/_Runnable.ino
  setup_CDSTimer();
  setup_CDS_SPI();
=======
  //setup_TCC();
  int pololouSMC_names[1] = {DEFAULT_SMC};
  int pololouServo_names[1] = {DEFAULT_SERVO};

>>>>>>> _Runnable/_Runnable.ino
  setup_PololuUart();
  //setup_PololuSMC(pololouSMC_names, 1);
  //setup_encoder();
  if(SERIALMONITER) setup_USBserial();
  timer1kztest();
  SPItest();
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
<<<<<<< _Runnable/_Runnable.ino

=======
  /**
  if(SENDSERIAL){
    SerialPol.write(testmessage, 3);
    SerialPol.flush();
    delay(2000);
  }
>>>>>>> _Runnable/_Runnable.ino
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
  /** Serial.print("The temperature is: ");
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
  delay(500); */
  Serial.println();
  Serial.print("The current error message is: ");
  Serial.println(getErrorsServo(DEFAULT_SERVO));


  setTargetServo(DEFAULT_SERVO, 1, 2000);
  delay(20);
  Serial.print("The target speed of channel 0 is: ");
  Serial.println(getPositionServo(DEFAULT_SERVO, 1));
  delay(2000);
  setTargetServo(DEFAULT_SERVO, 1, 2000);
  delay(20);
  Serial.print("The target speed of channel 1 is: ");
  Serial.println(getPositionServo(DEFAULT_SERVO, 1));
  setTargetServo(DEFAULT_SERVO, 1, 3333);
  delay(20);
  Serial.print("The target speed of channel 1 is: ");
  Serial.println(getPositionServo(DEFAULT_SERVO, 1));
  delay(2000);
  setTargetServo(DEFAULT_SERVO, 1, 4667);
  delay(20);
  Serial.print("The target speed of channel 1 is: ");
  Serial.println(getPositionServo(DEFAULT_SERVO, 1));
  delay(2000);
  setTargetServo(DEFAULT_SERVO, 1, 6000);
  delay(20);
  Serial.print("The target speed of channel 1 is: ");
  Serial.println(getPositionServo(DEFAULT_SERVO, 1));
  delay(2000);
  setTargetServo(DEFAULT_SERVO, 1, 7333);
  delay(20);
  Serial.print("The target speed of channel 1 is: ");
  Serial.println(getPositionServo(DEFAULT_SERVO, 1));
  delay(2000);
  setTargetServo(DEFAULT_SERVO, 1, 8667);
  delay(20);
  Serial.print("The target speed of channel 1 is: ");
  Serial.println(getPositionServo(DEFAULT_SERVO, 1));
  delay(2000);
  setTargetServo(DEFAULT_SERVO, 1, 10000);
  delay(20);
  Serial.print("The target speed of channel 1 is: ");
  Serial.println(getPositionServo(DEFAULT_SERVO, 1));
  delay(2000);


}
