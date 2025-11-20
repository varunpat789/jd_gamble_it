#include "game_logic.h"
#include "utils.h"
#include <Arduino.h>

State updateStateMachine(State currentState)
{
    State nextState = currentState;
    Response response = NONE;
    bool is_game_over;

    switch (currentState)
    {
    case INITIALIZED:
        if (inputs[START_BUTTON])
        {
            // RESTART GAME
            nextState = INTERMEDIATE;
            lives_remaining = 3;
            score = 0;
            credit = 0;
            action_timeout = INITIAL_ACTION_TIMEOUT;
            life_0_led.enable();
            life_1_led.enable();
            life_2_led.enable();

            Serial.println(">>> Transitioning to INTERMEDIATE");
        }
        break;

    case INTERMEDIATE:
        nextState = choose_next_action();
        switch (nextState)
        {
        case COIN_IT:
            play_sound(COIN_IT_SOUND);
            break;
        case SHAKE_IT:
            play_sound(SHAKE_IT_SOUND);
            break;
        case SPIN_IT:
            play_sound(SPIN_IT_SOUND);
            break;
        case CASH_IT:
            play_sound(CASH_IT_SOUND);
            break;
        }
        action_start_time = millis(); // Start timer for new action
        Serial.print(">>> Transitioning to ");
        Serial.println(stateToString(nextState));
        break;

    case COIN_IT:
        response = coin_it();

        if (response == NONE && (millis() - action_start_time > action_timeout))
        {
            response = INCORRECT;
            Serial.println(">>> TIMEOUT COIN IT");
        }

        switch (response)
        {
        case CORRECT_25:
            score++;
            credit += 25;
            play_sound(CORRECT_SOUND);
            Serial.println(">>> CORRECT COIN IT (25)");
            nextState = INTERMEDIATE;
            decrease_action_timeout();
            break;
        case CORRECT_5:
            score++;
            credit += 5;
            play_sound(CORRECT_SOUND);
            Serial.println(">>> CORRECT COIN IT (5)");
            nextState = INTERMEDIATE;
            decrease_action_timeout();
            break;

        case INCORRECT:
            Serial.println(">>> INCORRECT COIN IT");
            is_game_over = remove_life();

            if (is_game_over)
            {
                play_sound(GAME_OVER_SOUND);
                nextState = INITIALIZED;
            }
            else
            {
                play_sound(INCORRECT_SOUND);
                nextState = INTERMEDIATE;
            }
            break;

        case NONE:
            break;
        }
        break;

    case SPIN_IT:
        response = spin_it();
        if (response == NONE && (millis() - action_start_time > action_timeout))
        {
            response = INCORRECT;
            Serial.println(">>> TIMEOUT SPIN IT");
        }

        switch (response)
        {
        case CORRECT:
            xTaskNotifyGive(stepper_task_handle);
            score++;

            play_sound(CORRECT_SOUND);
            Serial.println(">>> CORRECT SPIN IT");

            nextState = INTERMEDIATE;
            decrease_action_timeout();

            if (credit >= 5)
            {
                credit -= 5;
            }
            else
            {
                credit = 0;
            }
            break;

        case INCORRECT:

            Serial.println(">>> INCORRECT SPIN IT");
            is_game_over = remove_life();

            if (is_game_over)
            {
                play_sound(GAME_OVER_SOUND);
                nextState = INITIALIZED;
            }
            else
            {
                play_sound(INCORRECT_SOUND);
                nextState = INTERMEDIATE;
            }

            break;

        case NONE:
            break;
        }
        break;

    case CASH_IT:
        response = cash_it();

        if (response == NONE && (millis() - action_start_time > action_timeout))
        {
            response = INCORRECT;
            Serial.println(">>> TIMEOUT CASH IT");
        }

        switch (response)
        {
        case CORRECT:
            score++;

            play_sound(CORRECT_SOUND);

            Serial.println(">>> CORRECT CASH IT");
            nextState = INTERMEDIATE;
            decrease_action_timeout();

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
                play_sound(GAME_OVER_SOUND);
                nextState = INITIALIZED;
            }
            else
            {
                play_sound(INCORRECT_SOUND);
                nextState = INTERMEDIATE;
            }

            break;

        case NONE:
            break;
        }
        break;

    case SHAKE_IT:
        response = shake_it();

        if (response == NONE && (millis() - action_start_time > action_timeout))
        {
            response = INCORRECT;
            Serial.println(">>> TIMEOUT SHAKE IT");
        }

        switch (response)
        {
        case CORRECT:
            score++;
            play_sound(CORRECT_SOUND);

            Serial.println(">>> CORRECT SHAKE IT");
            nextState = INTERMEDIATE;
            decrease_action_timeout();

            break;

        case INCORRECT:
            Serial.println(">>> INCORRECT SHAKE IT");
            is_game_over = remove_life();

            if (is_game_over)
            {
                play_sound(GAME_OVER_SOUND);
                nextState = INITIALIZED;
            }
            else
            {
                play_sound(INCORRECT_SOUND);
                nextState = INTERMEDIATE;
            }

        case NONE:
            break;
        }
        break;

    case NA:
        break;
    }

    return nextState;
}

void decrease_action_timeout()
{
    if (action_timeout - TIMEOUT_DECREASE >= MIN_ACTION_TIMEOUT)
    {
        action_timeout -= TIMEOUT_DECREASE;
    }
    else
    {
        action_timeout = MIN_ACTION_TIMEOUT;
    }
    Serial.print(">>> New timeout: ");
    Serial.println(action_timeout);
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
        life_0_led.disable();
        play_sound(GAME_OVER_SOUND);
        play_sound(REPLAY);

        return true;
    }
    return false;
}

void play_sound(Sound sound)
{
    byte track_number = static_cast<byte>(sound) + 1;
    speaker.execute_CMD(0x03, 0, track_number);
    delay(500);
}
