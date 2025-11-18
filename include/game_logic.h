#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "constants.h"
#include "imu.h"
#include "break_beam.h"
#include "button.h"
#include "stepper.h"
#include "speaker.h"
#include "servoController.h"
#include "led.h"

// State machine functions
void execute_states();
State choose_next_action();

// Game action handlers
Response coin_it();
Response spin_it();
Response cash_it();
Response shake_it();

// Game state checkers
bool is_game_won();
bool is_game_over();
bool is_start_game();

// Game state modifiers
void increment_score();
void remove_life();
void update_credit();
void init_next_command();
void game_won();

// Audio
void audio();

// External references to global variables
extern int lives_remaining;
extern State current_state;
extern State prev_state;
extern volatile Response response;
extern volatile int inputs[5];
extern volatile int score;
extern volatile int credit;
extern volatile int counter;
extern volatile int event_timer;

// External references to hardware objects
extern Speaker speaker;
extern IMU imu;
extern BreakBeam big_break_beam;
extern BreakBeam small_break_beam;
extern Button cash_button;
extern Button start_button;
extern Button limit_switch;
extern Stepper stepper0;
extern Stepper stepper1;
extern Stepper stepper2;
extern ServoController servo;
extern LED life_0_led;
extern LED life_1_led;
extern LED life_2_led;

#endif // GAME_LOGIC_H