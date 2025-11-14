#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Arduino.h>

// GAME STATES
enum State
{
    INITIALIZED,
    INTERMEDIATE,
    COIN_IT,
    SPIN_IT,
    CASH_IT,
    SHAKE_IT,
};

enum Inputs
{
    LIMIT,
    BUTTON,
    BIG_BREAK_BEAM,
    SMALL_BREAK_BEAM,
    SHAKE
};

// GAME SETTINGS
const int INIT_LIVES = 3;
const int WIN_SCORE = 99;

// SPEAKER
const uint8_t SPEAKER_TX = 17;
const uint8_t SPEAKER_RX = 16;

// BUTTON & LIMIT SWITCH
const uint8_t BUTTON_PIN = 35;
const uint8_t LIMIT_SWITCH_PIN = 34;
const unsigned long BUTTON_DB = 500; // ms

// BREAK BEAMS
const uint8_t BB_0_PIN = 32;
const uint8_t BB_1_PIN = 33;
const unsigned long BB_DB = 500; // ms

// IMU
const unsigned long IMU_DB = 1000;   // ms
const double SHAKE_THRESH = 15000.0; // ms

// STEPPERS
const uint8_t STEP_0_STEP_PIN = 25;
const uint8_t STEP_0_DIR_PIN = 26;
const uint8_t STEP_1_STEP_PIN = 27;
const uint8_t STEP_1_DIR_PIN = 14;
const uint8_t STEP_2_STEP_PIN = 12;
const uint8_t STEP_2_DIR_PIN = 13;

// SERVO
const uint8_t SERVO_PWM_PIN = 10;

// LEDS
const uint8_t LIFE_0_PIN = 10;
const uint8_t LIFE_1_PIN = 10;
const uint8_t LIFE_2_PIN = 10;


// DISPLAY
const int LATCH_PIN = 2;
const int CLOCK_PIN = 18;
const int DATA_PIN = 19;

const int DISPLAY_INTERVAL = 5000; // 5ms

const byte digitToSegmentsDisplay0[] = {
    0b11111100, // 0
    0b01100000, // 1
    0b11011010, // 2
    0b11110010, // 3
    0b01100110, // 4
    0b10110110, // 5
    0b10111110, // 6
    0b11100000, // 7
    0b11111110, // 8
    0b11100110  // 9
};

const byte digitToSegmentsDisplay1[] = {
    0b11111101, // 0
    0b01100001, // 1
    0b11011011, // 2
    0b11110011, // 3
    0b01100111, // 4
    0b10110111, // 5
    0b10111111, // 6
    0b11100001, // 7
    0b11111111, // 8
    0b11100111  // 9
};

#endif // CONSTANTS_H