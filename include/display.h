#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include "constants.h"

class Display
{
private:
    uint8_t latch_pin;
    uint8_t clock_pin;
    uint8_t data_pin;
    volatile int current_multiplexed_digit = 0;

public:
    Display(uint8_t _latch_pin, uint8_t _clock_pin, uint8_t _data_pin);
    void update_display(int score, int credit);
};

#endif // DISPLAY_H