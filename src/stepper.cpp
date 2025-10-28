#include "stepper.h"

Stepper::Stepper(uint8_t _step_pin, uint8_t _dir_pin) : step_pin(_step_pin), dir_pin(_dir_pin)
{
    pinMode(step_pin, OUTPUT);
    pinMode(dir_pin, OUTPUT);
}

void Stepper::step()
{
    digitalWrite(dir_pin, HIGH);

    for (int x = 0; x < 200; x++)
    {
        digitalWrite(step_pin, HIGH);
        delayMicroseconds(500);
        digitalWrite(step_pin, LOW);
        delayMicroseconds(500);
    }
    delay(1000);

    digitalWrite(dir_pin, LOW);

    for (int x = 0; x < 400; x++)
    {
        digitalWrite(step_pin, HIGH);
        delayMicroseconds(500);
        digitalWrite(step_pin, LOW);
        delayMicroseconds(500);
    }
    delay(1000);
}
