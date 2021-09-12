#include "runnable.h"
#include "Pololu.h"
#include "CDS92Timers.h"
#include "PiSerial.h"
#include "MPU6050.h"
#include "AS5048.h"
#include <Wire.h>

uint8_t testmessage[3];
Vector accel, gyro;
MPU6050 mpu;
bool mpu_connected = false;
bool encoder_connected = false;

int servoValue = 0;
VL53L1X sensor;
AS5048 encoder;

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
  setup_PololuSMC(pololouSMC_names, 0);
  //setup_encoder();
  if(SERIALMONITER) setup_USBserial();
  Wire.begin();

  Wire.setClock(400000); // use 400 kHz I2C
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


  //setup_I2C();

  // mpu = MPU6050();
  // if(mpu.begin(PM_2_G, PM_250_DEG_SEC, MAX_260_HZ)) {
  //   Serial.println("MPU Connected.");
  //   mpu_connected = true;
  // } else {
  //   Serial.println("MPU failed to connect.");
  // }
  /** Serial.println("past I2C setup");
  Serial.print("The MPU accleration range is: ");
  Serial.println(MPU.getAccRange());
  Serial.print("The MPU gyro scale is: ");
  Serial.print(MPU.getGyroScale());*/
  // timer1kztest();
  // SPItest();

  // sensor.setTimeout(500);
  // if (!sensor.init())
  // {
  //   Serial.println("Failed to detect and initialize sensor!");
  //   while (1);
  // }

  // // Use long distance mode and allow up to 50000 us (50 ms) for a measurement.
  // // You can change these settings to adjust the performance of the sensor, but
  // // the minimum timing budget is 20 ms for short distance mode and 33 ms for
  // // medium and long distance modes. See the VL53L1X datasheet for more
  // // information on range and timing limits.
  // sensor.setDistanceMode(VL53L1X::Long);
  // sensor.setMeasurementTimingBudget(50000);
  //
  // // Start continuous readings at a rate of one measurement every 50 ms (the
  // // inter-measurement period). This period should be at least as long as the
  // // timing budget.
  // sensor.startContinuous(50);
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
  // // Write to Pololu Uart
  // if(mpu_connected) {
  //   accel = mpu.readAccel();
  //   gyro = mpu.readGyro();

    // Serial.println("Accel:");
    // Serial.print("x = ");
    // Serial.print(accel.x);
    // Serial.print(",y = ");
    // Serial.print(accel.y);
    // Serial.print(",z = ");
    // Serial.println(accel.z);


    // servoValue = accel.x*4000 + 6000;
    // Serial.print("The servo setting value is: ");
    // Serial.println(servoValue);
    // setTargetServo(DEFAULT_SERVO, 0, servoValue);
    //delay(1000);
    //
    // Serial.print("The temperature is: ");
    // Serial.println(mpu.readTemp());

    // Serial.println("Gyro:");
    // Serial.print("x = ");
    // Serial.print(gyro.x);
    // Serial.print(",y = ");
    // Serial.print(gyro.y);
    // Serial.print(",z = ");
    // Serial.println(gyro.z);
  // }
  // delay(100);
  // sensor.read();
  //
  // Serial.print("range: ");
  // Serial.println(sensor.ranging_data.range_mm);
  // Serial.print("\tstatus: ");
  // Serial.println(VL53L1X::rangeStatusToString(sensor.ranging_data.range_status));

  // servoValue = (sensor.ranging_data.range_mm, 0, 3000, 2000, 10000);
  // setTargetServo(DEFAULT_SERVO, 0, servoValue);

  double angleRaw, angleFromZero;
  int zero, gain, error, name;
  if(encoder_connected) {
    // angleRaw = encoder.getPositionRaw();
    // angleFromZero = encoder.getPositionFromZero();
    // gain = encoder.getGainValue();
    // error = encoder.getErrorByte();
    // zero = (double)encoder.readZeroPosition()*AS5048_RESOLUTION_FACTOR;
    // Serial.print("The raw value was: ");
    // Serial.println(angleRaw);
    // Serial.print("The real value was: ");
    // Serial.println(angleFromZero);
    // Serial.print("The error byte was: ");
    // Serial.println(error, BIN);
    // Serial.print("The gain was: ");
    // Serial.println(gain, BIN);
    // Serial.print("The zero position is: ");
    // Serial.println(zero);
    name = encoder.setAddress(72);
    Serial.print("The returned address is: ");
    Serial.println(name);
    delay(500);
    Serial.print("The address is: ");
    Serial.println(encoder.readAddress());
    delay(3000);

    name = encoder.setAddress(64);
    Serial.print("The returned address is: ");
    Serial.println(name);
    delay(500);
    Serial.print("The address is: ");
    Serial.println(encoder.readAddress());
    delay(3000);
    //
    // Serial.print("The zero position is: ");
    // Serial.println(encoder.readZeroPosition()*AS5048_RESOLUTION_FACTOR);
  }
  delay(500);
}
