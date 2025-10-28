#include "button.h"

Button::Button(uint8_t _input_pin, unsigned long debounce) : input_pin(_input_pin), debouce_interval(debounce)
{
    pinMode(input_pin, INPUT);
}

bool Button::detect()
{
    unsigned long current_time = millis();

    if (current_time - last_detect_time < debouce_interval)
    {
        return false;
    }

    if (digitalRead(input_pin))
    {
        last_detect_time = current_time;
        return true;
    }

    return false;
}
