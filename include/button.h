#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button
{
private:
    uint8_t input_pin;
    unsigned long debouce_interval = 500; // ms
    unsigned long last_detect_time = 0;

public:
    Button(uint8_t _input_pin, unsigned long debounce);
    bool detect();
};

#endif // BUTTON_H