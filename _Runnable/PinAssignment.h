#ifndef H_PINASSIGNMENT
#define H_PINASSIGNMENT

// CDS92 TIMERS
#define TCC00PIN 10
#define TCC10PIN 5
#define TCC20PIN 4

#define CDS_STEP_1 10
#define CDS_STEP_2 5
#define CDS_STEP_3 4

#define CDS_FAULT_1 6
#define CDS_FAULT_2 9
#define CDS_FAULT_3 11

#define V_LEVEL_OE 12 //pulled high enables voltage converters

//CDS92 SPI for Shift Registers
#define SPI_SCK 25
#define SPI_MOSI 24
#define SPI_OE 23 //pulled low enables output of shift registers
#define SPI_SS 13

// POLOLU COMMS - SERCOM4 - UART
#define POL_RX_PIN 17    // pin A3 connects to SMC RX
#define POL_TX_PIN 16    // pin A2 connects to SMC TX
#define POL_RST_PIN 19    // pin 9 connects to SMC nRST
#define POL_ERR_PIN 14    // pin 6 connects to SMC ERR

// Raspberry Pi COMMS - SERCOM0 - UART
#define PI_RX_PIN 15     // pin A1 connects to SMC RX
#define PI_TX_PIN 18     // pin A4 connects to SMC TX

// I2C Comms
#define SDA 21
#define SCL 22

#endif
