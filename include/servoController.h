#ifndef SERVO_CONTROLLER_H
#define SERVO_CONTROLLER_H

#include <Arduino.h>
#include <ESP32Servo.h>

class ServoController
{
private:
    uint8_t pwm_pin;
    Servo servo;

public:
    ServoController(uint8_t _pwm_pin);
    void fire(int delay);
};

#endif // SERVO_CONTROLLER_H