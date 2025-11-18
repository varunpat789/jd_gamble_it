#ifndef UTILS_H
#define UTILS_H

#include "constants.h"
#include "imu.h"
#include "break_beam.h"
#include "button.h"
#include "stepper.h"
#include "speaker.h"
#include "servoController.h"
#include "led.h"

// Utility functions
void update_inputs();
void log_inputs();

// Hardware control
void drop_coin();
void activate_reels();
void spin_reels();

// Lever and cash out
void get_lever_state();
void is_cash_out_pressed();

// Coin classification
int classify_coin();
int get_next_counter_value();

void IRAM_ATTR onTimer();

#endif // UTILS_H