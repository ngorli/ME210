


/************** Timer Action Definitions *******************/
bool TURN_COMPLETE = false;
bool IGNITION_REVERSE_COMPLETE = false;


/*--------------Time Duration Definitions------------------------*/
int GAME_TIMER = 130000;
int TURN_TIMER = 100; // PLACEHOLDER
int IGNITION_ON_TIMER = 1500;  // PLACEHOLDER


/*
 * This timer is used to measure the full duration of the game, 
 * and shuts plays the buzzer noise and shuts of when done
 */
void GameTimerHandler()
{
  state = GAME_END;
}



/*
 * This timer is used to measure the amount of time needed to turn
 */
void TurnTimerHandler()
{
  TURN_COMPLETE = true;
}


/*
 * This timer is used to measure the amount of needed to reverse
 * before approaching the ignition
 */
void IgnitionOnTimerHandler()
{
  IGNITION_REVERSE_COMPLETE = true;
}






