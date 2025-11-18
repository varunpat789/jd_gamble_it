#ifndef STEPPER_H
#define STEPPER_H

#include <Arduino.h>

class Stepper
{
private:
    uint8_t step_pin;
    uint8_t dir_pin;
    double cycle_state;

public:
    Stepper(uint8_t _step_pin, uint8_t _dir_pin);
    void step();
    void reverse_step();
};

#endif // STEPPER_H
