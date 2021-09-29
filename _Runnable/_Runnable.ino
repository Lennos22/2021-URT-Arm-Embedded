#include "runnable.h"
#include "Pololu.h"
#include "CDS92.h"
#include "PiSerial.h"
#include "MPU6050.h"
#include "AS5048.h"
#include <Wire.h>


MPU6050 mpu;
bool mpu_active = false;

VL53L1X sensor;

AS5048 encoder;

void setup() {

  if(SERIALMONITER) setup_USBserial();

  if(CDS_CONNECTED){
    if(DEBUG) Serial.println("CDS Setup Starting");
    setup_CDS();
    if(DEBUG) Serial.println("CDS Setup Finished");
  }

  if(MPU_CONNECTED) setup_MPU();

  if(I2C_CONNECTED){
    Wire.begin();
    Wire.setClock(400000); // use 400 kHz I2C
  }
  if(ENCODER_CONNECTED) test_Encoder();

  if(POLOLU_CONNECTED){
    int pololouSMC_names[1] = {DEFAULT_SMC};
    int pololouServo_names[1] = {DEFAULT_SERVO};

    setup_PololuUart();
    setup_PololuSMC(pololouSMC_names, 0);
  }
}

void loop() {
  CDSEnableControllers();
  delay(2000);
  CDSDisableControllers();
  delay(2000); 
}


void setup_USBserial(){
  Serial.begin(9600);
  delay(500);
  Serial.println("Serial Connected");
}

void test_Encoder(){
  bool encoder_connected = false;
  if(!encoder.begin()) {
    Serial.println("The encoder failed to connect properly");
  } else {
    Serial.println("The encoder was connected");
    encoder_connected = true;
    delay(100);
    Serial.print("The zero position is: ");
    Serial.println(encoder.readZeroPosition()*AS5048_RESOLUTION_FACTOR);
    Serial.print("The address is: ");
    Serial.println(encoder.readAddress());
  }
  double angleRaw, angleFromZero;
  int zero, gain, error, name;
  if(encoder_connected) {
    // angleRaw = encoder.getPositionRaw();
    angleFromZero = encoder.getPositionFromZero();
    gain = encoder.getGainValue();
    error = encoder.getErrorByte();
    zero = (double)encoder.readZeroPosition()*AS5048_RESOLUTION_FACTOR;
    // Serial.print("The raw value was: ");
    // Serial.println(angleRaw);
    Serial.print("The real value was: ");
    Serial.println(angleFromZero);
    // Serial.print("The error byte was: ");
    // Serial.println(error, BIN);
    // Serial.print("The gain was: ");
    // Serial.println(gain, BIN);
    // Serial.print("The zero position is: ");
    // Serial.println(zero);
    // name = encoder.setAddress(72);
    // Serial.print("The returned address is: ");
    // Serial.println(name);
    // delay(500);
    // Serial.print("The address is: ");
    // Serial.println(encoder.readAddress());
    // delay(3000);
    //
    // name = encoder.setAddress(64);
    // Serial.print("The returned address is: ");
    // Serial.println(name);
    // delay(500);
    // Serial.print("The address is: ");
    // Serial.println(encoder.readAddress());
    // delay(3000);
    //
    // Serial.print("The zero position is: ");
    // Serial.println(encoder.readZeroPosition()*AS5048_RESOLUTION_FACTOR);
  }
}

void setup_MPU(){
  mpu = MPU6050();
  if(mpu.begin(PM_2_G, PM_250_DEG_SEC, MAX_260_HZ)) {
    Serial.println("MPU Connected.");
    mpu_active = true;
  } else {
    Serial.println("MPU failed to connect.");
  }
  /** Serial.println("past I2C setup");
  Serial.print("The MPU accleration range is: ");
  Serial.println(MPU.getAccRange());
  Serial.print("The MPU gyro scale is: ");
  Serial.print(MPU.getGyroScale());*/
}

void print_MPU(){
  if(mpu_active) {
    Vector accel, gyro;
    accel = mpu.readAccel();
    gyro = mpu.readGyro();

    Serial.println("Accel:");
    Serial.print("x = ");
    Serial.print(accel.x);
    Serial.print(",y = ");
    Serial.print(accel.y);
    Serial.print(",z = ");
    Serial.println(accel.z);

    //the following code moves the servo
    //int servoValue = 0;
    //servoValue = accel.x*4000 + 6000;
    //Serial.print("The servo setting value is: ");
    //Serial.println(servoValue);
    //setTargetServo(DEFAULT_SERVO, 0, servoValue);
    //delay(1000);

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
}
