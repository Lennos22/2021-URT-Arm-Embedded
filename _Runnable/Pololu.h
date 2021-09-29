#ifndef H_POLOLU
#define H_POLOLU

#define SERIALPOLSPEED 19200

/******************************** Variable IDs *********************************
*******************************************************************************/
#define ERROR_STATUS 0
/**
The set bits of this variable indicate the errors that are currently stopping the motor.
The motor can only be driven when this register has a value of 0. (See Section 3.4
for error descriptions.)
• Bit 0: Safe Start Violation
• Bit 1: Required Channel Invalid
• Bit 2: Serial Error
• Bit 3: Command Timeout
• Bit 4: Limit/Kill Switch
• Bit 5: Low VIN
• Bit 6: High VIN
• Bit 7: Over Temperature
• Bit 8: Motor Driver Error
• Bit 9: ERR Line High
• Bits 10-15: reserved */
#define ERRORS_OCCURED 1
/**
The set bits of this register indicate the errors that have occurred since this register
was last cleared. This status register has the same bit assignments as the Error
Status register documented above. Reading this variable clears all of the bits. */
#define SERIAL_ERRORS 2
/**
The set bits of this variable indicate the serial errors that have occurred since
this variable was last cleared. Reading this variable clears all of the bits. (See
Section 3.4 for serial error descriptions.)
• Bit 0: reserved
• Bit 1: Frame
• Bit 2: Noise
• Bit 3: RX Overrun
• Bit 4: Format
• Bit 5: CRC
• Bits 6-16: reserved */
#define LIMIT_STATUS 3
/**
The set bits of this variable indicate things that are currently limiting the motor
controller.
• Bit 0: Motor is not allowed to run due to an error or safe-start violation.
• Bit 1: Temperature is active reducing target speed.
• Bit 2: Max speed limit is actively reducing target speed (target speed >
max speed).
• Bit 3: Starting speed limit is actively reducing target speed to zero (target
speed < starting speed).
• Bit 4: Motor speed is not equal to target speed because of acceleration,
deceleration, or brake duration limits.
• Bit 5: RC1 is configured as a limit/kill switch and the switch is active
(scaled value ≥ 1600).
• Bit 6: RC2 limit/kill switch is active (scaled value ≥ 1600).
• Bit 7: AN1 limit/kill switch is active (scaled value ≥ 1600).
• Bit 8: AN2 limit/kill switch is active (scaled value ≥ 1600).
• Bit 9: USB kill switch is active.
• Bits 10-15: reserved */
#define TARGET_SPEED 20                 // (-3200,3200)
#define CURRENT_SPEED 21                // (-3200,3200)
#define BRAKE_AMOUNT 22                 // (0,32)
#define INPUT_VOLTAGE 23                // mV
#define TEMPERATURE 24                  // 0.1'C
#define BAU_RATE_REGISTER 27            // seconds per 7.2e7. So 7.2e7/vlaue for bps
#define MAX_FORWARD_SPEED 30            // (0,3200)
#define MAX_FORWARD_ACCEL 31            // (0,3200)
#define MAX_FORWARD_DECCEL 32           // (0,3200)
#define MAX_FORWARD_BRAKE_DURATION 33   // msec
#define MAX_BACKWARD_SPEED 36           // (0,3200)
#define MAX_BACKWARD_ACCEL 37           // (0,3200)
#define MAX_BACKWARD_DECCEL 38          // (0,3200)
#define MAX_BACKWARD_BRAKE_DURATION 39  // msec
#define RESET_FLAGS 127
/**
Flags indicating the source of the last board reset. This variable does not change
while the controller is running and is not reported under the Status Tab of the
Simple Motor Control Center. You can view it in the Device Information window of
the Control Center, which is available from the Device menu, and for the first two
seconds after start-up, the yellow status LED flashes a pattern that indicates the
last reset source (see Section 3.5).
• 0x04 (4): RST pin pulled low by external source.
• 0x0C (12): Power reset (VIN got too low or was disconnected).
• 0x14 (20): Software reset (by firmware upgrade process).
• 0x24 (38): Watchdog timer reset (should never happen; this could
indicate a firmware bug). */

/*************************** Motor limit IDs ***********************************
********************************************************************************

Speeds:           Must be between 0 and 3200 (0 to 100%).
                  Default is 3200
Accelerations:    Must be between 0 and 3200 (change in speed per update period).
                  0 is no limit, but then 1 is the greatest limit.
                  Default is 0
Deccelerations:   Same as Accelerations.
Break durations:  Must be between 0 and 16384 in units of 4ms. This value is the
                  time spent braking (at speed 0) when transitioning from
                  reverse to forward (or vice versa).
*/
#define BOTH_DIRECT_SPEED 0
#define BOTH_DIRECT_ACCEL 1
#define BOTH_DIRECT_DECCEL 2
#define BOTH_DIRECT_BRAKE_DURATION 3
#define FORWARD_SPEED 4
#define FORWARD_ACCEL 5
#define FORWARD_DECCEL 6
#define FORWARD_BRAKE_DURATION 7
#define BACKWARD_SPEED 8
#define BACKWARD_ACCEL 9
#define BACKWARD_DECCEL 10
#define BACKWARD_BRAKE_DURATION 11

extern Uart SerialPol;

void SERCOM4_0_Handler();
void SERCOM4_1_Handler();
void SERCOM4_2_Handler();
void SERCOM4_3_Handler();

//Initialises the Serial Connection
void setup_PololuUart();

// Set up the simple motor controllers
void setup_PololuSMC(int* deviceNumbers, int numberDevices);

// Exits saft start mode on the device indicated by device number
void exitSafeStartSMC(unsigned char deviceNumber);

// Exits safe start for all of the devices in deviceNumbers
void exitSafeStartAllSMC(int *deviceNumbers, int numberDevices);

// Sets the serial communication speed in the simple motor controller
void setPolSerialSpeedAll();

// Reads a byte from the SerialPol, returns -1 if nothing read
int readByteAll();

// Sets the motor speed of device indicated. The speed value is a int with it's
// value between -3200 and 3200. With -3200 being full speed reverse, 3200 being
// full speed forward, and 0 being stopped.
void setMotorSpeedSMC(int deviceNumber, int speed);

// Sets the breaks of motor indicated. The amount is between 0 and 32, with 0 being
// a full coast and 32 being full break.
void setMotorBreakSMC(int deviceNumber, int amount);

// Stops the motor indicated. This does two things: sets the target speed to 0,
// thereby stopping the motor (respecting set decceleration limits) and puts the
// device into Safe-Start mode if that is enabled.
void stopMotorSMC(int deviceNumber);

// Sets limits for the device indicated's motor parameters. See the Motor Limit IDs
// section of Polou.h for the different ID's and potential values. Returns a integer
// to signify the sucess: 0 for all good, 1 for unable to set a forward param because
// of a hard motor limit, 2 for a reverse hard motor limit, and 3 for a forward and
// reverse hard motor limit.
unsigned char setMotorLimitSMC(int deviceNumber, unsigned char limitID, unsigned int limitValue);

// Requests the a variable from teh specified motor. See the Variable IDs
// section of Polou.h for the different ID's and potential values.
unsigned int getVariableSMC(int deviceNumber, unsigned char variableID);

// Prints the current status information of the named device.
void printStatusInformationSMC(int deviceNumber);

// As these will all be in servo mode, the target represents the pulse width to
// transmit in units of quarter-microseconds. A target value of 0 tells the Maestro
// to stop sending pulses to the servo.
void setTargetServo(int deviceNumber, int channelNumber, int target);

// Limits the speed at which the specified servo and channel's output can change.
// This is in units of 0.25 microseconds per 10 milliseconds - eg 140 means 3.5
// microsecond per millisecond. 0 means no limit. Default has this set with a
// minimum pulse width of 992 microseconds and a max of 2000 microseconds (So,
// between 3968 and 8000 in our units), but this can be changes in the app thingy.
void setSpeedLimitServo(int deviceNumber, int channelNumber, uint16_t speed);

// Limits the accelration of servo's output channel. Again, 0 means no limit. The
// units are (0.25 microseconds)/(10 milliseconds)/(80 milliseconds). This is a
// value between 0 and 255.
void setAccLimitServo(int deviceNumber, int channelNumber, uint16_t accel);

// Gets the positions from a specified channel. When the channel is configured
// to servo mode this is the current pulse width on the channel. Note that the
// units here are again in 0.25 microseconds.
unsigned int getPositionServo(int deviceNumber, int channelNumber);

/** Gets the errors from the servo named. The bits represent the following:
0 - Serial Signal Error
1 - Serial Overrun Error
2 - Serial Buffer Full
3 - Serial CRC Error
4 - Serial Protocal Error
5 - Serial Timeout
6 - Script Stack Error
7 - Script Call Stack Error
8 - Script Program Counter Error
*/
unsigned int getErrorsServo(int deviceNumber);

// This can be copied to the runnable loop to printout variables from the SMC
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

// Copy into the runnable loop for 45 degree increments fror the servo
/**
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
delay(2000); */
#endif
