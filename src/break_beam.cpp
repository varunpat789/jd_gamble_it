#include "break_beam.h"

BreakBeam::BreakBeam(uint8_t _detect_pin)
    : detect_pin(_detect_pin)
{
    pinMode(detect_pin, INPUT);
}

bool BreakBeam::detect()
{
    bool current_state = !digitalRead(detect_pin); // true when beam is broken

    // Detect rising edge (beam just broke)
    if (current_state && !last_state)
    {
        last_state = true;
        return true; // Coin detected!
    }

    last_state = current_state;
    return false;
}