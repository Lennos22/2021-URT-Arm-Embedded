#ifndef CPP_IMU_CLASS_H
#define CPP_IMU_CPP_H

#define PM_2_G 0
#define PM_4_G 1
#define PM_8_G 2
#define PM_16_G 3

#define GRAV 9.81
#define PM_2_G_FACT (16384)
#define PM_4_G_FACT (8192)
#define PM_8_G_FACT (4096)
#define PM_16_G_FACT (1048)

#define PM_250_DEG_SEC 0
#define PM_500_DEG_SEC 1
#define PM_1000_DEG_SEC 2
#define PM_2000_DEG_SEC 3

#define PM_250_DEG_SEC_FACT 131
#define PM_500_DEG_SEC_FACT 65.5
#define PM_1000_DEG_SEC_FACT 32.8
#define PM_2000_DEG_SEC_FACT 16.4

#define MAX_260_HZ 0
#define MAX_184_HZ 1
#define MAX_94_HZ 2
#define MAX_44_HZ 3
#define MAX_21_HZ 4
#define MAX_10_HZ 5
#define MAX_5_HZ 6

// Constant to convert raw temperature to Celsius degrees
#define MPU6050_TEMP_LINEAR_COEF (1.0/340.00)
#define MPU6050_TEMP_OFFSET       36.53

#define IMU_NAME                      (0x68)
#define MPU6050_REG_ACCEL_XOUT_H      (0x3B) // Acc measurements
#define MPU6050_REG_ACCEL_XOUT_L      (0x3C)
#define MPU6050_REG_ACCEL_YOUT_H      (0x3D)
#define MPU6050_REG_ACCEL_YOUT_L      (0x3E)
#define MPU6050_REG_ACCEL_ZOUT_H      (0x3F)
#define MPU6050_REG_ACCEL_ZOUT_L      (0x40)
#define MPU6050_REG_TEMP_OUT_H        (0x41) // Temp measurements
#define MPU6050_REG_TEMP_OUT_L        (0x42)
#define MPU6050_REG_GYRO_XOUT_H       (0x43) // Gyro measurements
#define MPU6050_REG_GYRO_XOUT_L       (0x44)
#define MPU6050_REG_GYRO_YOUT_H       (0x45)
#define MPU6050_REG_GYRO_YOUT_L       (0x46)
#define MPU6050_REG_GYRO_ZOUT_H       (0x47)
#define MPU6050_REG_GYRO_ZOUT_L       (0x48)
#define MPU6050_REG_GYRO_CONFIG       (0x1B) // Gyroscope Configuration
#define MPU6050_REG_ACCEL_CONFIG      (0x1C) // Accelerometer Configuration
#define MPU6050_REGISTER_WHO_AM_I     (0x75) // Contains address of the device (0x68)
#define MPU6050_DEFAULT_ADDRESS       (0x68) // Default MPU506 Name
#define MPU6050_REGISTER_CONFIG       (0x1A) // DLPF config
#define MPU6050_REGISTER_PWR_MGMT_1   (0x6B) // Power management

#ifndef VECTOR_STRUCT_H
#define VECTOR_STRUCT_H
struct Vector {
  flaot x;
  flaot y;
  flaot z;
};
#endif


struct Sensitivity_Pair {
  int mode;
  int factor;
};

class IMU {
  public:
    Vector ra, rg, accelG, accel, gyro;
    Sensitivity_Pair a_sens, g_sens;
    bool begin(int accSens, int  gyroSens, int bandwidth);
    bool isConnected();
    void sleepMode();
    void wakeUp();
    int16_t readRegister16(uint8_t reg);
    int8_t readRegister8(uint8_t reg);
    void writeRegister8(uint8_t reg, uint8_t value);
    void writeRegister16(uint8_t reg, uint16_t value);
    Vector readAccel();
    Vector readGyro();
    void setAccRange(int range);
    uint8_t getAccRange();
    void setGyroScale(int scale);
    uint8_t getGyroScale();
    void setDLPFBandwidth(int bandwidth);
    uint8_t getBandwidth();
    byte read8(byte registerAddr);
    void write8(byte registerAddr, byte value);
    void accelRawToG();
    int16_t readTemp();
    int16_t read16(uint8_t reg);
};


#endif
