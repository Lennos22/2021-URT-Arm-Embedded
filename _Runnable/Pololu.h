#ifndef H_POLOLU
#define H_POLOLU

#define SERIALPOLSPEED 19200

// Variable IDs
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
#define TARGET_SPEED 20
#define CURRENT_SPEED 21
#define BRAKE_AMOUNT 22
#define INPUT_VOLTAGE 23
#define TEMPERATURE 24
#define BAU_RATE_REGISTER 27
#define MAX_FORWARD_SPEED 30
#define MAX_FORWARD_ACCEL 31
#define MAX_FORWARD_DECCEL 32
#define MAX_FORWARD_BRAKE_DURATION 33
#define MAX_BACKWARD_SPEED 36
#define MAX_BACKWARD_ACCEL 37
#define MAX_BACKWARD_DECCEL 38
#define MAX_BACKWARD_BRAKE_DURATION 39
#define RESET_FLAGS 127

/** Motor limit IDs
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

// Device Names
#define DEFAULT_NAME 13

extern Uart SerialPol;

void SERCOM4_0_Handler();
void SERCOM4_1_Handler();
void SERCOM4_2_Handler();
void SERCOM4_3_Handler();

//Initialises the Serial Connection
void setup_PololuUart(int* deviceNumbers, int numberDevices);
// Exits saft start mode on the device indicated by device number
void exitSafeStart(unsigned char deviceNumber);
// Exits safe start for all of the devices in deviceNumbers
void exitSafeStartAll(int *deviceNumbers, int numberDevices);
// Sets the serial communication speed in the simple motor controller
void setPolSerialSpeed();
// Reads a byte from the SerialPol, returns -1 if nothing read
int readByte();
// Sets the motor speed of device indicated. The speed value is a int with it's
// value between -3200 and 3200. With -3200 being full speed reverse, 3200 being
// full speed forward, and 0 being stopped.
void setMotorSpeed(int deviceNumber, int speed);
// Sets the breaks of motor indicated. The amount is between 0 and 32, with 0 being
// a full coast and 32 being full break.
void setMotorBreak(int deviceNumber, int amount);
// Stops the motor indicated. This does two things: sets the target speed to 0,
// thereby stopping the motor (respecting set decceleration limits) and puts the
// device into Safe-Start mode if that is enabled.
void stopMotor(int deviceNumber);
// Sets limits for the device indicated's motor parameters. See the Motor Limit IDs
// section of Polou.h for the different ID's and potential values. Returns a integer
// to signify the sucess: 0 for all good, 1 for unable to set a forward param because
// of a hard motor limit, 2 for a reverse hard motor limit, and 3 for a forward and
// reverse hard motor limit.
unsigned char setMotorLimit(int deviceNumber, unsigned char limitID, unsigned int limitValue);
// Requests the a variable from teh specified motor. See the Variable IDs
// section of Polou.h for the different ID's and potential values.
unsigned int getVariable(int deviceNumber, unsigned char variableID);


/**
Other things to note about the Pololu Simple Motor Controller:

*/
#endif
