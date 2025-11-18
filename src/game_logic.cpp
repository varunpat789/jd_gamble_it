#include "game_logic.h"
#include "utils.h"
#include <Arduino.h>

State updateStateMachine(State currentState)
{
    State nextState = currentState;
    Response response = NONE;
    bool is_game_over;
    // add incorrect state to play audio?
    switch (currentState)
    {
    case INITIALIZED:
        if (inputs[START_BUTTON])
        {
            nextState = INTERMEDIATE;
            lives_remaining = 3;
            score = 0;
            credit = 0;
            Serial.println(">>> Transitioning to INTERMEDIATE");
        }
        break;

    case INTERMEDIATE:
        nextState = choose_next_action();
        Serial.print(">>> Transitioning to ");
        Serial.println(stateToString(nextState));
        break;

    case COIN_IT:
        response = coin_it();
        switch (response)
        {
        case CORRECT_25:
            score++;
            credit += 25;
            Serial.println(">>> CORRECT COIN IT (25)");
            nextState = INTERMEDIATE;
            break;
        case CORRECT_5:
            score++;
            credit += 5;
            Serial.println(">>> CORRECT COIN IT (5)");
            nextState = INTERMEDIATE;
            break;

        case INCORRECT:
            Serial.println(">>> INCORRECT COIN IT");

            is_game_over = remove_life();
            if (is_game_over)
            {
                nextState = INITIALIZED;
            }
            else
            {
                nextState = INTERMEDIATE;
            }

            break;

        case NONE:
            break;
        }

        break;

    case SPIN_IT:
        response = spin_it();
        switch (response)
        {
        case CORRECT:
            score++;
            Serial.println(">>> CORRECT SPIN IT");
            nextState = INTERMEDIATE;
            // Remove money to bet
            if (credit >= 5)
            {
                credit -= 5;
            }
            else
            {
                credit = 0;
            }
            // start process to spin steppers
            break;

        case INCORRECT:
            Serial.println(">>> INCORRECT SPIN IT");

            is_game_over = remove_life();
            if (is_game_over)
            {
                nextState = INITIALIZED;
            }
            else
            {
                nextState = INTERMEDIATE;
            }

            break;

        case NONE:
            break;
        }
        break;

    case CASH_IT:
        response = cash_it();
        switch (response)
        {
        case CORRECT:
            score++;
            Serial.println(">>> CORRECT CASH IT");
            nextState = INTERMEDIATE;
            // Remove money to cash out
            if (credit >= 5)
            {
                credit -= 5;
            }
            else
            {
                credit = 0;
            }
            servo.fire(1);
            break;

        case INCORRECT:
            Serial.println(">>> INCORRECT CASH IT");

            is_game_over = remove_life();
            if (is_game_over)
            {
                nextState = INITIALIZED;
            }
            else
            {
                nextState = INTERMEDIATE;
            }

            break;

        case NONE:
            break;
        }
        break;

    case SHAKE_IT:
        response = shake_it();
        switch (response)
        {
        case CORRECT:
            score++;
            Serial.println(">>> CORRECT SHAKE IT");
            nextState = INTERMEDIATE;
            break;

        case INCORRECT:
            Serial.println(">>> INCORRECT SHAKE IT");

            is_game_over = remove_life();
            if (is_game_over)
            {
                nextState = INITIALIZED;
            }
            else
            {
                nextState = INTERMEDIATE;
            }

            break;

        case NONE:
            break;
        }
        break;

    case NA:
        // Do nothing in NA state
        break;
    }

    return nextState;
}

const char *stateToString(State state)
{
    switch (state)
    {
    case INITIALIZED:
        return "INITIALIZED";
    case INTERMEDIATE:
        return "INTERMEDIATE";
    case COIN_IT:
        return "COIN_IT";
    case SPIN_IT:
        return "SPIN_IT";
    case CASH_IT:
        return "CASH_IT";
    case SHAKE_IT:
        return "SHAKE_IT";
    case GAME_OVER:
        return "GAME_OVER";
    case NA:
        return "NA";
    default:
        return "UNKNOWN";
    }
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

Response coin_it()
{
    // Serial.println("COIN IT");
    if (inputs[BIG_BREAK_BEAM])
    {
        // add coin classification + updating credit here
        return CORRECT_25;
    }
    else if (inputs[SMALL_BREAK_BEAM])
    {
        return CORRECT_5;
    }
    else if (inputs[CASH_BUTTON] || inputs[LIMIT] || inputs[SHAKE])
    {
        return INCORRECT;
    }
    return NONE;
}

Response spin_it()
{
    // Serial.println("SPIN IT");
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
    // Serial.println("CASH IT");
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
    // Serial.println("SHAKE IT");
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

bool remove_life()
{
    if (lives_remaining == 3)
    {
        lives_remaining = 2;
        Serial.println("TWO lives remaining");
        life_2_led.disable();
        return false;
    }
    else if (lives_remaining == 2)
    {
        lives_remaining = 1;
        Serial.println("ONE lives remaining");
        life_1_led.disable();
        return false;
    }
    else if (lives_remaining == 1)
    {
        lives_remaining = 0;
        Serial.println("ZERO lives remaining");
        Serial.println(">>> GAME OVER, BACK TO INITIALIZED");
        // PLAY GAME OVER AUDIO
        life_0_led.disable();
        return true;
    }
    return false;
}

void audio()
{
    // TODO: Implement audio feedback
}