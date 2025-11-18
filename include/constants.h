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
    GAME_OVER,
    NA
};

enum Response
{
    CORRECT,
    CORRECT_25,
    CORRECT_5,
    INCORRECT,
    NONE
};

enum Inputs
{
    LIMIT,
    CASH_BUTTON,
    START_BUTTON,
    BIG_BREAK_BEAM,
    SMALL_BREAK_BEAM,
    SHAKE
};

// GAME SETTINGS
const int INIT_LIVES = 3;
const int WIN_SCORE = 99;
// const int MAX_TIME = 
// const int MIN_TIME = 

// SPEAKER
const uint8_t SPEAKER_TX = 17; // GPIO17 (TX) -> RX
const uint8_t SPEAKER_RX = 18; // GPIO18 (RX) -> TX

// ACCELEROMETER (IMU)
const uint8_t IMU_SDA_PIN = 8;       // GPIO08 (SDA) -> SDA
const uint8_t IMU_SCL_PIN = 9;       // GPIO09 (SCL) -> SCL
const unsigned long IMU_DB = 1000;   // ms
const double SHAKE_THRESH = 15000.0; // ms

// BUTTONS & LIMIT SWITCH
const uint8_t CASH_BUTTON_PIN = 45;   // IO4
const uint8_t START_BUTTON_PIN = 21; // IO21
const uint8_t LIMIT_SWITCH_PIN = 47; // IO47
const unsigned long BUTTON_DB = 50; // ms

// BREAK BEAMS
const uint8_t BB_0_PIN = 46;     // IO46 -> BB 0
const uint8_t BB_1_PIN = 15;     // IO15 -> BB 1
const unsigned long BB_DB = 50; // ms

// STEPPERS
const uint8_t STEP_0_STEP_PIN = 1;  // IO1 -> Driver A - Step
const uint8_t STEP_0_DIR_PIN = 2;   // IO2 -> Driver A - Direction
const uint8_t STEP_1_STEP_PIN = 38; // IO38 -> Driver B - Step
const uint8_t STEP_1_DIR_PIN = 37;  // IO37 -> Driver B - Direction
const uint8_t STEP_2_STEP_PIN = 36; // IO36 -> Driver C - Step
const uint8_t STEP_2_DIR_PIN = 35;  // IO35 -> Driver C - Direction

// SERVO
const uint8_t SERVO_PWM_PIN = 7; // IO7 -> SERVO (PWM)

// LIFE LEDS
const uint8_t LIFE_0_PIN = 4; // IO45 -> Life 1
const uint8_t LIFE_1_PIN = 6;  // IO6 -> Life 2
const uint8_t LIFE_2_PIN = 5;  // IO5 -> Life 3

// DISPLAY
const uint8_t LATCH_PIN = 20; // IO20 -> LATCH
const uint8_t CLOCK_PIN = 3;  // IO3 -> CLK
const uint8_t DATA_PIN = 19;  // IO19 -> DATA

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