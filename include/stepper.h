#ifndef STEPPER_H
#define STEPPER_H

#include <Arduino.h>

class Stepper
{
private:
    uint8_t step_pin;
    uint8_t dir_pin;
    double cycle_state;

public:
    Stepper(uint8_t _step_pin, uint8_t _dir_pin);
    void step();
};

#endif // STEPPER_H

// void setup() {
//   // Sets the two pins as Outputs
//   pinMode(stepPin,OUTPUT);
//   pinMode(dirPin,OUTPUT);
// }
// void loop() {
//   digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
//   // Makes 200 pulses for making one full cycle rotation
//   for(int x = 0; x < 200; x++) {
//     digitalWrite(stepPin,HIGH);
//     delayMicroseconds(500);
//     digitalWrite(stepPin,LOW);
//     delayMicroseconds(500);
//   }
//   delay(1000); // One second delay

//   digitalWrite(dirPin,LOW); //Changes the rotations direction
//   // Makes 400 pulses for making two full cycle rotation
//   for(int x = 0; x < 400; x++) {
//     digitalWrite(stepPin,HIGH);
//     delayMicroseconds(500);
//     digitalWrite(stepPin,LOW);
//     delayMicroseconds(500);
//   }
//   delay(1000);
// }
