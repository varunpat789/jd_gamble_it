#include "button.h"

Button::Button(uint8_t _input_pin, unsigned long debounce)
    : input_pin(_input_pin),
      debounce_interval(debounce),
      lastDebounceTime(0),
      lastPinReading(HIGH),
      debouncedState(HIGH),
      lastDebouncedState(HIGH)
{
    pinMode(input_pin, INPUT_PULLUP);
}

bool Button::detect()
{
    // Read the input pin (inverted because of INPUT_PULLUP)
    bool pinReading = !digitalRead(input_pin);

    // Check if pin reading has changed (reset debounce timer)
    if (pinReading != lastPinReading)
    {
        lastDebounceTime = millis();
    }
    lastPinReading = pinReading;

    // If enough time has passed, update the debounced state
    if ((millis() - lastDebounceTime) > debounce_interval)
    {
        debouncedState = pinReading;
    }

    // Check for rising edge (LOW to HIGH transition = button press)
    bool edgeDetected = (debouncedState == HIGH && lastDebouncedState == LOW);

    lastDebouncedState = debouncedState;

    return edgeDetected;
}