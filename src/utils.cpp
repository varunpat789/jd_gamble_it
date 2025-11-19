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
}

void log_inputs()
{
    bool any_inputs_detected = false;
    if (inputs[SHAKE])
    {
        Serial.println("Shake, ");
        any_inputs_detected = true;
    }
    if (inputs[CASH_BUTTON])
    {
        Serial.println("Cash Button, ");
        any_inputs_detected = true;
    }
    if (inputs[START_BUTTON])
    {
        Serial.println("Start Button, ");
        any_inputs_detected = true;
    }
    if (inputs[LIMIT])
    {
        Serial.println("Limit, ");
        any_inputs_detected = true;
    }
    if (inputs[BIG_BREAK_BEAM])
    {
        Serial.println("Big BB, ");
        any_inputs_detected = true;
    }
    if (inputs[SMALL_BREAK_BEAM])
    {
        Serial.println("Small BB, ");
        any_inputs_detected = true;
    }
}

void spin_reels(void *parameter)
{
    while (1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        Serial.println("SPIN REELS");

        for (int i = 0; i < 1000; i++)
        {
            stepper0.step();
            stepper1.step();
            stepper2.step();
            delayMicroseconds(500);
        }

        Serial.println("DONE SPIN");
    }
}