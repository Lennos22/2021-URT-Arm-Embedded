#ifndef H_PINASSIGNMENT
#define H_PINASSIGNMENT

// CDS92 TIMERS
#define TCC00PIN 10
#define TCC10PIN 5
#define TCC20PIN 4

//CDS92 SPI for Shift Registers
#define SPI_SCK 25
#define SPI_MOSI 24
#define SPI_SS 13

// POLOLU COMMS - SERCOM4 - UART
#define POL_RX_PIN 17    // pin A3 connects to SMC RX
#define POL_TX_PIN 16    // pin A2 connects to SMC TX
<<<<<<< _Runnable/PinAssignment.h
#define POLresetPin 6    // pin 6 connects to SMC nRST
#define POLerrPin 7      // pin 7 connects to SMC ERR
=======
#define POL_RST_PIN 9    // pin 9 connects to SMC nRST
#define POL_ERR_PIN 6    // pin 6 connects to SMC ERR

// Raspberry Pi COMMS - SERCOM0 - UART
#define PI_RX_PIN 15     // pin A1 connects to SMC RX
#define PI_TX_PIN 18     // pin A4 connects to SMC TX
>>>>>>> _Runnable/PinAssignment.h

#endif
