#include "runnable.h"
#include "Pololu.h"
#include "CDS92Timers.h"
#include "PiSerial.h"
#include "MPU6050.h"
#include <Wire.h>

uint8_t testmessage[3];
Vector accel, gyro;
MPU6050 mpu;
bool mpu_connected = false;

void setup_USBserial(){
  Serial.begin(9600);
  while (!Serial);
  delay(500);
  Serial.println("Serial Connected");
}

void setup() {

  setup_CDSTimer();
  setup_CDS_SPI();

  //setup_TCC();
  int pololouSMC_names[1] = {DEFAULT_SMC};
  int pololouServo_names[1] = {DEFAULT_SERVO};


  setup_PololuUart();
  //setup_PololuSMC(pololouSMC_names, 1);
  //setup_encoder();
  if(SERIALMONITER) setup_USBserial();
  Wire.begin();
  //setup_I2C();

  mpu = MPU6050();
  if(mpu.begin(PM_2_G, PM_250_DEG_SEC, MAX_260_HZ)) {
    Serial.println("MPU Connected.");
    mpu_connected = true;
  } else {
    Serial.println("MPU failed to connect.");
  }
  /** Serial.println("past I2C setup");
  Serial.print("The MPU accleration range is: ");
  Serial.println(MPU.getAccRange());
  Serial.print("The MPU gyro scale is: ");
  Serial.print(MPU.getGyroScale());*/
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
  if(mpu_connected) {
    accel = mpu.readAccel();
    gyro = mpu.readGyro();

    Serial.println("Accel:");
    Serial.print("x = ");
    Serial.print(accel.x);
    Serial.print(",y = ");
    Serial.print(accel.y);
    Serial.print(",z = ");
    Serial.println(accel.z);
    delay(1500);

    Serial.print("The temperature is: ");
    Serial.println(mpu.readTemp());

    Serial.println("Gyro:");
    Serial.print("x = ");
    Serial.print(gyro.x);
    Serial.print(",y = ");
    Serial.print(gyro.y);
    Serial.print(",z = ");
    Serial.println(gyro.z);
  }
  delay(10);
}
