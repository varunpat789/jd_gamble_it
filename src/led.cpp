#include "led.h"

LED::LED(uint8_t _pin) : pin(_pin)
{
    digitalWrite(pin, HIGH);
}

void LED::disable()
{
    digitalWrite(pin, LOW);
}