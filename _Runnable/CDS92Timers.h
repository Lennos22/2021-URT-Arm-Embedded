#ifndef H_CDSTIMERS
#define H_CDSTIMERS

#define MAXVAL24INT 16777215

void setup_CDSTimer();
status_t writeFrequency(uint8_t timer, uint32_t frequency);
float readFrequency(uint8_t timer);
void enableTimer(uint8_t timer);
void disableTimer(uint8_t timer);

#endif
