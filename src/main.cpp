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

// GAME VARIABLES
int lives_remaining;
State state;
volatile Response response = NONE;
volatile int inputs[5] = {0, 0, 0, 0, 0};
volatile int score = 0;
volatile int credit = 0;
volatile int counter = 1;
volatile int event_timer = 1000;
static State last_state = INITIALIZED;

// SPEAKER
Speaker speaker(SPEAKER_TX, SPEAKER_RX);

// SENSORS
IMU imu(SHAKE_THRESH, IMU_DB);
BreakBeam big_break_beam(BB_0_PIN);
BreakBeam small_break_beam(BB_1_PIN);
Button cash_button(CASH_BUTTON_PIN, BUTTON_DB);
Button start_button(START_BUTTON_PIN, BUTTON_DB);

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
Response coin_it();
Response spin_it();
Response cash_it();
Response shake_it();
State choose_next_action();
void audio();
void init_next_command();
void game_won();
void update_inputs();
void log_inputs();
bool is_start_game();

int classify_coin();
int get_next_counter_value()
{
  return 0;
}

void setup()
{
  // set up audio
  Serial.begin(115200);

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
  // speaker.play_first();

  // Serial.println("BRO");
  life_0_led.enable();
  life_1_led.enable();
  life_2_led.enable();

  update_inputs();
  // log_inputs();

  delay(10);

  // credit += 1;
  // score += 1;

  // if (score >= 100)
  // {
  //   score = 0;
  //   // game_won();
  // }
  // if (credit >= 1000)
  // {
  //   credit = 0;
  //   // game_won();
  // }

  // if (is_game_over())
  // {
  //   Serial.println("GAME OVER!");
  // }

  // GAME LOGIC
  if (state == INITIALIZED)
  {
    // only happens for one cycle
    // play start audio
    response = NONE;
    Serial.println("waitnig");
    if (inputs[START_BUTTON])
    {
      Serial.println("START GAME");
      state = INTERMEDIATE;
    }
  }
  else
  {
    Serial.println("ELSE");
  }
  // else if (state == INTERMEDIATE)
  // {
  //   // only happens for one cycle
  //   // select the next stage
  //   // play audio
  //   // start timer by reseting counter?
  //   response = NONE;
  //   state = choose_next_action();
  //   counter = get_next_counter_value();
  // }
  // else if (state == COIN_IT)
  // {
  //   response = coin_it();
  // }
  // else if (state == SPIN_IT)
  // {
  //   response = spin_it();
  // }
  // else if (state == CASH_IT)
  // {
  //   response = cash_it();
  // }
  // else if (state == SHAKE_IT)
  // {
  //   response = shake_it();
  // }
  // else if (state == GAME_OVER)
  // {
  //   // stall state until get start button input
  //   if (is_start_game())
  //   {
  //     // start button hit
  //     state = INITIALIZED;
  //   }
  // }
  // if (response == CORRECT)
  // {
  //   // increment score
  //   // set state to intermediate
  //   Serial.println("CORRECT");
  //   state = INTERMEDIATE;
  // }
  // else if (response == INCORRECT || counter <= 0)
  // {
  //   // decrement score
  //   // set state to intermediate
  //   // remove life and check for game over
  //   Serial.println("INCORRECT");
  //   state = INTERMEDIATE;
  //   score -= 1;
  // }
  if (state != last_state)
  {
    Serial.print("State changed from ");
    Serial.print(last_state);
    Serial.print(" to ");
    Serial.println(state);
    last_state = state;
  }
  // if none, increment timer/counter
  // check if alloted time has passed
  // if not, pass
  // if so, remove life and check if game over
  // counter--;
}

int classify_coin()
{
  return 0;
}
void spin_reels()
{
  int reel0_step = random(500, 3);
  int reel1_step = random(500, 3);
  int reel2_step = random(500, 3);
  for (int i = 0; i < 1000; i++)
  {
    stepper0.step();
    stepper1.step();
    stepper2.step();
  }
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
  if (!any_inputs_detected)
  {
    Serial.println("No inputs detected.");
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

int determine_coin_value()
{
  if (inputs[BIG_BREAK_BEAM])
  {
    return 25;
  }
  return 5;
}

bool is_start_game()
{
  return inputs[START_BUTTON];
}

Response coin_it()
{
  Serial.println("COIN IT");
  if (inputs[BIG_BREAK_BEAM] || inputs[SMALL_BREAK_BEAM])
  {
    // add coin classification + updating credit here
    return CORRECT;
  }
  else if (inputs[CASH_BUTTON] || inputs[LIMIT] || inputs[SHAKE])
  {
    return INCORRECT;
  }
  return NONE;
}

Response spin_it()
{
  Serial.println("SPIN IT");
  if (inputs[LIMIT])
  {
    // reduce credit here

    // fire steppers (end of steppers is when credit is added)
    return CORRECT;
  }
  else if (inputs[CASH_BUTTON] || inputs[SHAKE] || inputs[BIG_BREAK_BEAM] || inputs[SMALL_BREAK_BEAM])
  {
    return INCORRECT;
  }
  return NONE;
}

Response cash_it()
{
  Serial.println("CASH IT");
  if (inputs[CASH_BUTTON])
  {
    // reduce credit here
    // fire servo
    return CORRECT;
  }
  else if (inputs[BIG_BREAK_BEAM] || inputs[SMALL_BREAK_BEAM] || inputs[LIMIT] || inputs[SHAKE])
  {
    return INCORRECT;
  }
  return NONE;
}

Response shake_it()
{
  Serial.println("SHAKE IT");
  if (inputs[SHAKE])
  {
    // reduce credit here
    // fire servo
    return CORRECT;
  }
  else if (inputs[BIG_BREAK_BEAM] || inputs[SMALL_BREAK_BEAM] || inputs[LIMIT] || inputs[CASH_BUTTON])
  {
    return INCORRECT;
  }
  return NONE;
}

State choose_next_action()
{
  int next_action = random(4);
  if (next_action == 0)
  {
    return COIN_IT;
  }
  if (next_action == 1)
  {
    return SPIN_IT;
  }
  if (next_action == 2)
  {
    return CASH_IT;
  }
  return SHAKE_IT;
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