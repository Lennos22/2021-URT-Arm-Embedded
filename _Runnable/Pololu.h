#ifndef H_POLOLU
#define H_POLOLU

#define SERIALPOLSPEED 19200

// Variable IDs
#define ERROR_STATUS 0
#define LIMIT_STATUS 3
#define TARGET_SPEED 20
#define CURRENT_SPEED 21
#define BRAKE_AMOUNT 22
#define INPUT_VOLTAGE 23
#define TEMPERATURE 24
#define BAU_RATE_REGISTER 27
#define MAX_FORWARD_SPEED 30
#define MAX_BACKWARD_SPEED 36



// Motor limit IDs
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

void setup_PololuUart();
void SERCOM4_0_Handler();
void SERCOM4_1_Handler();
void SERCOM4_2_Handler();
void SERCOM4_3_Handler();

void exitSafeStart();
int readByte();
void setMotorSpeed(int speed);
unsigned char setMotorLimit(unsigned char limitID, unsigned int limitValue);
unsigned int getVariable(unsigned char variableID);

#endif
