#include <Arduino.h>
#include "constants.h"
#include "imu.h"
#include "break_beam.h"
#include "button.h"
#include "stepper.h"
#include "speaker.h"
#include "servoController.h"
#include "led.h"
#include "display.h"

// ESP32-DevKitC V4

// GAME VARIABLES
int lives_remaining;
volatile State state = INITIALIZED;
volatile int inputs[5] = {0, 0, 0, 0, 0};
volatile int score = 0;
volatile int credit = 0;
volatile int event_timer = 1000;

// SPEAKER
Speaker speaker(SPEAKER_TX, SPEAKER_RX);

// SENSORS
IMU imu(SHAKE_THRESH, IMU_DB);
BreakBeam big_break_beam(BB_0_PIN, BB_DB);
BreakBeam small_break_beam(BB_1_PIN, BB_DB);
Button button(BUTTON_PIN, BUTTON_DB);
Button limit_switch(LIMIT_SWITCH_PIN, BUTTON_DB);

// STEPPERS
Stepper stepper0(STEP_0_STEP_PIN, STEP_0_DIR_PIN);
Stepper stepper1(STEP_1_STEP_PIN, STEP_1_DIR_PIN);
Stepper stepper2(STEP_2_STEP_PIN, STEP_2_DIR_PIN);

// SERVO
ServoController servo(SERVO_PWM_PIN);

// LED - LIVES
LED life_0_led(LIFE_0_PIN);
LED life_1_led(LIFE_1_PIN);
LED life_2_led(LIFE_2_PIN);

// DISPLAY TIMER INTURRUPT
volatile int current_multiplexed_digit = 0;
hw_timer_t *score_timer = NULL;

void IRAM_ATTR onTimer();
void increment_score();
bool is_game_won();
bool is_game_over();
void remove_life();
void update_credit();
void drop_coin();
void activate_reels();
void get_lever_state();
void is_cash_out_pressed();
void coin_it();
void spin_it();
void cash_it();
State choose_next_action();
void audio();
void init_next_command();
void game_won();
void update_inputs();

void setup()
{
  Serial.begin(9600);

  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);

  score_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(score_timer, &onTimer, true);
  timerAlarmWrite(score_timer, DISPLAY_INTERVAL, true);
  timerAlarmEnable(score_timer);

  // INITIALIZE IMU
  if (!imu.begin())
  {
    Serial.println("Failed to connect to imu");
  }

  Serial.println("Initialized game");

  score = 0;
  credit = 0;
  lives_remaining = INIT_LIVES;
  state = INITIALIZED;
}

void loop()
{
  // stepper0.step();
  // stepper1.step();
  // stepper2.step();

  delay(100);

  credit += 1;
  score += 1;

  if (score >= 100)
  {
    score = 0;
    // game_won();
  }
  if (credit >= 1000)
  {
    credit = 0;
    // game_won();
  }

  if (is_game_over())
  {
    Serial.println("GAME OVER!");
  }
}

void update_inputs()
{
  imu.update();

  inputs[LIMIT] = limit_switch.detect();
  inputs[BUTTON] = button.detect();
  inputs[BIG_BREAK_BEAM] = big_break_beam.detect();
  inputs[SMALL_BREAK_BEAM] = small_break_beam.detect();
  inputs[SHAKE] = imu.is_shaken();
}

void log_inputs()
{
  bool any_inputs_detected = false;
  if (inputs[SHAKE])
  {
    Serial.print("Shake, ");
    any_inputs_detected = true;
  }
  if (inputs[BUTTON])
  {
    Serial.print("Button, ");
    any_inputs_detected = true;
  }
  if (inputs[LIMIT])
  {
    Serial.print("Limit, ");
    any_inputs_detected = true;
  }
  if (inputs[BIG_BREAK_BEAM])
  {
    Serial.print("Big BB, ");
    any_inputs_detected = true;
  }
  if (inputs[SMALL_BREAK_BEAM])
  {
    Serial.print("Small BB, ");
    any_inputs_detected = true;
  }
  if (!any_inputs_detected)
  {
    Serial.print("No inputs detected.");
  }
  Serial.println("");
}

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
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, digitToSegmentsDisplay0[credit_ones_digit]);
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, digitToSegmentsDisplay0[credit_hundreds_digit]);
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, digitToSegmentsDisplay0[scores_ones_digit]);
  }
  else
  {
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, digitToSegmentsDisplay1[credit_tens_digit]);
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, digitToSegmentsDisplay1[credit_thousands_digit]);
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, digitToSegmentsDisplay1[scores_tens_digit]);
  }

  digitalWrite(LATCH_PIN, HIGH);
  current_multiplexed_digit = 1 - current_multiplexed_digit;
}

void increment_score()
{
  score += 1;
}

bool is_game_won()
{
  return score >= WIN_SCORE;
}

bool is_game_over()
{
  return lives_remaining <= 0;
}

void remove_life()
{
  lives_remaining -= 1;
}

void update_credit()
{
  credit += 1;
}

void drop_coin() {}
void activate_reels() {}
void is_cash_out_pressed() {}
void is_coin_inserted() {}
int determine_coin_value() { return 1; }

void coin_it()
{
  Serial.println("COIN IT");
}

void spin_it()
{
  Serial.println("SPIN IT");
}

void cash_it()
{
  Serial.println("CASH IT");
}

State choose_next_action()
{
  int next_action = random(3);
  if (next_action == 0)
  {
    return COIN_IT;
  }
  if (next_action == 1)
  {
    return SPIN_IT;
  }
  return CASH_IT;
}

void audio() {}

void init_next_command()
{
  state = choose_next_action();
  if (state == COIN_IT)
  {
    Serial.println("Coin it!");
    coin_it();
  }
  else if (state == CASH_IT)
  {
    Serial.println("Cash it!");
    cash_it();
  }
  else if (state == SPIN_IT)
  {
    Serial.println("Spin it!");
    spin_it();
  }
}

void game_won()
{
  Serial.println("game won");
}