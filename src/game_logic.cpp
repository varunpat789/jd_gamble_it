#include "game_logic.h"
#include "utils.h"
#include <Arduino.h>

void execute_states()
{
  switch (current_state)
  {
  case INITIALIZED:
    // Only happens for one cycle
    // Play start audio
    response = NONE;
    if (inputs[START_BUTTON])
    {
      Serial.println("START GAME");
      current_state = INTERMEDIATE;
    }
    break;

  case INTERMEDIATE:
    // Only happens for one cycle
    // Select the next stage
    // Play audio
    // Start timer by resetting counter
    response = NONE;
    current_state = choose_next_action();
    counter = get_next_counter_value();
    break;

  case COIN_IT:
    response = coin_it();
    break;

  case SPIN_IT:
    response = spin_it();
    break;

  case CASH_IT:
    response = cash_it();
    break;

  case SHAKE_IT:
    response = shake_it();
    break;

  case GAME_OVER:
    // Stall current_state until start button input
    if (is_start_game())
    {
      current_state = INITIALIZED;
    }
    break;

  default:
    Serial.println("NONE");
    break;
  }

  // Handle responses
  if (response == CORRECT)
  {
    increment_score();
    current_state = INTERMEDIATE;
  }
  else if (response == INCORRECT || counter <= 0)
  {
    score -= 1;
    remove_life();
    if (is_game_over())
    {
      current_state = GAME_OVER;
    }
    else
    {
      current_state = INTERMEDIATE;
    }
  }
}

State choose_next_action()
{
  // TODO: Implement logic to choose next action
  return COIN_IT;
}

Response coin_it()
{
  // TODO: Implement coin insertion logic
  return NONE;
}

Response spin_it()
{
  // TODO: Implement spin logic
  return NONE;
}

Response cash_it()
{
  // TODO: Implement cash out logic
  return NONE;
}

Response shake_it()
{
  // TODO: Implement shake detection logic
  if (inputs[SHAKE])
  {
    return CORRECT;
  }
  return NONE;
}

bool is_game_won()
{
  return score >= 100;
}

bool is_game_over()
{
  return lives_remaining <= 0;
}

bool is_start_game()
{
  return inputs[START_BUTTON];
}

void increment_score()
{
  score += 10; // Or whatever increment is appropriate
}

void remove_life()
{
  if (lives_remaining > 0)
  {
    lives_remaining--;
    Serial.print("Lives remaining: ");
    Serial.println(lives_remaining);
  }
}

void update_credit()
{
  // TODO: Implement credit update logic
}

void init_next_command()
{
  // TODO: Implement next command initialization
}

void game_won()
{
  Serial.println("GAME WON!");
  // TODO: Implement win celebration
}

void audio()
{
  // TODO: Implement audio feedback
}