#ifndef H_CDS92
#define H_CDS92

//definitions for timer counter functions
#define MAXVAL16INT 65536
#define FREQ_CONVERSION 48000000.0/128.0/8.0/2.0
#define DEFAULT_FREQ 100
//final divide by 2 is to account for the fact that the CDS only trigger on a rising edge

//Register values
#define CDS_MSG_EMPTY     0x0000

#define CDS1_X4    0b1000000000000000
#define CDS1_DIR   0b0100000000000000
#define CDS1_EN    0b0010000000000000

#define CDS2_X4    0b0001000000000000
#define CDS2_DIR   0b0000100000000000
#define CDS2_EN    0b0000010000000000

#define CDS3_X4    0b0000000010000000
#define CDS3_DIR   0b0000000001000000
#define CDS3_EN    0b0000000000100000



void setup_CDS();
void setup_CDSTimer();
void setup_CDS_SPI();
status_t writeFrequency(uint8_t timer, double frequency);
float readFrequency(uint8_t timer);
void enableTimer(uint8_t timer);
void disableTimer(uint8_t timer);
void enableAllTimers();
void disableAllTimers();
void SPItest();
void enableRegisters();
void disableRegisters();
void CDSEnableController(int controller);
void CDSEnableControllers();
void CDSDisableController(int controller);
void CDSDisableControllers();
void CDSSetX4High(int controller);
void CDSSetX4Low(int controller);

#endif
