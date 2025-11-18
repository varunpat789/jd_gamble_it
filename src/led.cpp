#include "led.h"

LED::LED(uint8_t _pin) : pin(_pin)
{
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
}

void LED::disable()
{
    digitalWrite(pin, LOW);
}

void LED::enable()
{
    digitalWrite(pin, HIGH);
}