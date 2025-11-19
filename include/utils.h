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

void update_inputs();
void log_inputs();
void spin_reels(void *parameter);

void IRAM_ATTR onTimer();

void audio();

#endif // UTILS_H