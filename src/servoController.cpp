#include "servoController.h"

ServoController::ServoController(u_int8_t _pwm_pin) : pwm_pin(_pwm_pin)
{
    servo.attach(pwm_pin);
    servo.write(0);
}

void ServoController::fire(int _delay = 15)
{
    // does 180 rotation, then returns
    int pos = 0;

    for (pos = 0; pos <= 180; pos += 1)
    {
        servo.write(pos);
        delay(_delay);
    }
    for (pos = 180; pos >= 0; pos -= 1)
    {
        servo.write(pos);
        delay(_delay);
    }
}
