#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button
{
private:
    uint8_t input_pin;
    unsigned long debounce_interval;
    unsigned long lastDebounceTime;
    bool lastPinReading;
    bool debouncedState;
    bool lastDebouncedState;

public:
    Button(uint8_t _input_pin, unsigned long debounce);
    bool detect();
};

#endif // BUTTON_H