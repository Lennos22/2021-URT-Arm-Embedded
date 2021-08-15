#ifndef H_CDSTIMERS
#define H_CDSTIMERS

#define MAXVAL16INT 65536
#define FREQ_CONVERSION 48000000.0/128.0/8.0/2.0 //final divide by 2 is to account for the fact that the CDS only trigger on a rising edge

void setup_CDSTimer();
status_t writeFrequency(uint8_t timer, uint32_t frequency);
float readFrequency(uint8_t timer);
void enableTimer(uint8_t timer);
void disableTimer(uint8_t timer);
void enableAllTimers();
void disableAllTimers();
void timer1kztest();

#endif
