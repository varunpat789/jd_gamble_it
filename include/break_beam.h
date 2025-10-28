#ifndef BREAK_BEAM_H
#define BREAK_BEAM_H

#include <Arduino.h>

class BreakBeam
{
private:
    uint8_t detect_pin;
    unsigned long debouce_interval = 500; // ms
    unsigned long last_detect_time = 0;

public:
    BreakBeam(uint8_t _detect_pin, unsigned long debounce);
    bool detect();
};

#endif // BREAK_BEAM_H