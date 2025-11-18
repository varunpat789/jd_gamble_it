#include "utils.h"
#include "game_logic.h"
#include "display.h"
#include <Arduino.h>

extern volatile int current_multiplexed_digit;

void IRAM_ATTR onTimer()
{
    int scores_tens_digit = score / 10;
    int scores_ones_digit = score % 10;

    int credit_thousands_digit = (credit / 1000) % 10;
    int credit_hundreds_digit = (credit / 100) % 10;
    int credit_tens_digit = (credit / 10) % 10;
    int credit_ones_digit = credit % 10;

    digitalWrite(LATCH_PIN, LOW);

    if (current_multiplexed_digit == 0)
    {
        shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, digitToSegmentsDisplay0[credit_hundreds_digit]);
        shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, digitToSegmentsDisplay0[credit_ones_digit]);
        shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, digitToSegmentsDisplay0[scores_ones_digit]);
    }
    else
    {
        shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, digitToSegmentsDisplay1[credit_thousands_digit]);
        shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, digitToSegmentsDisplay1[credit_tens_digit]);
        shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, digitToSegmentsDisplay1[scores_tens_digit]);
    }

    digitalWrite(LATCH_PIN, HIGH);
    current_multiplexed_digit = 1 - current_multiplexed_digit;
}

void update_inputs()
{
    imu.update();
    inputs[LIMIT] = limit_switch.detect();
    inputs[START_BUTTON] = start_button.detect();
    inputs[CASH_BUTTON] = cash_button.detect();
    inputs[BIG_BREAK_BEAM] = big_break_beam.detect();
    inputs[SMALL_BREAK_BEAM] = small_break_beam.detect();
    inputs[SHAKE] = imu.is_shaken();

    if (inputs[START_BUTTON])
    {
        Serial.println("Start Button");
    }
}

void log_inputs()
{
    Serial.print("Inputs: ");
    for (int i = 0; i < 5; i++)
    {
        Serial.print(inputs[i]);
        Serial.print(" ");
    }
    Serial.println();
}

void drop_coin()
{
    // TODO: Implement coin dropping mechanism
}

void activate_reels()
{
    // TODO: Implement reel activation
    spin_reels();
}

void spin_reels()
{
    int reel0_step = random(3, 500);
    int reel1_step = random(3, 500);
    int reel2_step = random(3, 500);

    for (int i = 0; i < 1000; i++)
    {
        stepper0.step();
        stepper1.step();
        stepper2.step();
    }
}

void get_lever_state()
{
    // TODO: Implement lever state detection
}

void is_cash_out_pressed()
{
    // TODO: Implement cash out button check
}

int classify_coin()
{
    // TODO: Implement coin classification based on break beams
    if (inputs[BIG_BREAK_BEAM])
    {
        return 25; // Quarter
    }
    else if (inputs[SMALL_BREAK_BEAM])
    {
        return 10; // Dime
    }
    return 0;
}

int get_next_counter_value()
{
    // TODO: Implement timer/counter logic
    return 1000; // Default timeout value
}