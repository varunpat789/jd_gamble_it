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
#include "game_logic.h"
#include "utils.h"

// GAME VARIABLES
int lives_remaining;
State current_state;
State prev_state;
volatile Response response = NONE;
volatile int inputs[6] = {0, 0, 0, 0, 0, 0};
volatile int score = 0;
volatile int credit = 0;
volatile int counter = 1;

// GAME TIMING
volatile unsigned long action_start_time = 0;
unsigned long action_timeout = INITIAL_ACTION_TIMEOUT;

// FREE RTOS
TaskHandle_t stepper_task_handle = NULL;
TaskHandle_t speaker_task_handle = NULL;
QueueHandle_t stepper_queue = NULL;
QueueHandle_t speaker_queue = NULL;

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

// LIFE LEDs
LED life_0_led(LIFE_0_PIN);
LED life_1_led(LIFE_1_PIN);
LED life_2_led(LIFE_2_PIN);

// DISPLAY TIMER INTERRUPT
volatile int current_multiplexed_digit = 0;
hw_timer_t *score_timer = NULL;

void setup()
{
  Serial.begin(115200);

  // display pins
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);

  // timer interrupt for display
  score_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(score_timer, &onTimer, true);
  timerAlarmWrite(score_timer, DISPLAY_INTERVAL, true);
  timerAlarmEnable(score_timer);

  if (!imu.begin())
  {
    Serial.println("Failed to connect to imu");
  }

  xTaskCreatePinnedToCore(
      spin_reels,
      "spin_reels",
      4096,
      NULL,
      1,
      &stepper_task_handle,
      0);

  Serial.println("Initialized game");

  // init game states
  score = 0;
  credit = 0;
  lives_remaining = INIT_LIVES;
  current_state = INITIALIZED;

  play_sound(INITIALIZED_SOUND);
}

void loop()
{
  current_state = updateStateMachine(current_state);

  update_inputs();
  // log_inputs();
  delay(10);
}