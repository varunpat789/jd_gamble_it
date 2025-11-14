#include "display.h"

Display::Display(uint8_t _latch_pin, uint8_t _clock_pin, uint8_t _data_pin) : latch_pin(_latch_pin), clock_pin()
{
    pinMode(latch_pin, OUTPUT);
    pinMode(clock_pin, OUTPUT);
    pinMode(data_pin, OUTPUT);
}
void Display::update_display(volatile int score, volatile int credit)
{
    int scores_tens_digit = score / 10;
    int scores_ones_digit = score % 10;

    int credit_thousands_digit = (credit / 1000) % 10;
    int credit_hundreds_digit = (credit / 100) % 10;
    int credit_tens_digit = (credit / 10) % 10;
    int credit_ones_digit = credit % 10;

    digitalWrite(latch_pin, LOW);

    if (current_multiplexed_digit == 0)
    {
        shiftOut(data_pin, clock_pin, LSBFIRST, digitToSegmentsDisplay0[credit_ones_digit]);
        shiftOut(data_pin, clock_pin, LSBFIRST, digitToSegmentsDisplay0[credit_hundreds_digit]);
        shiftOut(data_pin, clock_pin, LSBFIRST, digitToSegmentsDisplay0[scores_ones_digit]);
    }
    else
    {
        shiftOut(data_pin, clock_pin, LSBFIRST, digitToSegmentsDisplay1[credit_tens_digit]);
        shiftOut(data_pin, clock_pin, LSBFIRST, digitToSegmentsDisplay1[credit_thousands_digit]);
        shiftOut(data_pin, clock_pin, LSBFIRST, digitToSegmentsDisplay1[scores_tens_digit]);
    }

    digitalWrite(latch_pin, HIGH);
    current_multiplexed_digit = 1 - current_multiplexed_digit;
}
