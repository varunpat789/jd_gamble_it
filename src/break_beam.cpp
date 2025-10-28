#include "break_beam.h"

BreakBeam::BreakBeam(uint8_t _detect_pin, unsigned long debounce) : detect_pin(_detect_pin), debouce_interval(debounce)
{
    pinMode(detect_pin, INPUT);
}

bool BreakBeam::detect()
{
    unsigned long current_time = millis();

    if (current_time - last_detect_time < debouce_interval)
    {
        return false;
    }

    if (!digitalRead(detect_pin))
    {
        last_detect_time = current_time;
        return true;
    }

    return false;
}
