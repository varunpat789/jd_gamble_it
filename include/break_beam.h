#ifndef BREAK_BEAM_H
#define BREAK_BEAM_H

#include <Arduino.h>

class BreakBeam
{
private:
    uint8_t detect_pin;
    bool last_state = false;

public:
    BreakBeam(uint8_t _detect_pin);
    bool detect();  // Returns true once per coin passage
};

#endif // BREAK_BEAM_H