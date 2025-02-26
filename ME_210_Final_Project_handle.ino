extern States_t state;
extern bool TURN_COMPLETE;
extern bool IGNITION_REVERSE_COMPLETE;

/*************************** OREINTATION HANDLING *******************/


/*
 * This function is used to turn the robot left until it is facing
 * forward in the start zone
 */
void handleInitOrientTurnLeft(void)
{
  rightMotorForward();
  leftMotorBackward();
  if(TestForUltraSonicsEqualAndBackLessThanStartZone()) RespToUltraSonicsEqualAndBackLessThanStartZone();
}


/*
 * This function is used to drive the robot forward out of the start zone,
 * drive the robot forward towards the tape for alignment, and drive
 * the robot forward to switch into the pot retrieval sequence
 */
void handleOrientDriveForward(void)
{
  rightMotorForward();
  leftMotorForward();
  if(TestForBackSensorMoreThanStartZone()) RespToBackSensorMoreThanStartZone();
  if(TestForOriented()) RespToOriented();
  if(TestForGetPot()) RespToGetPot();
}


/*
 * This function is used to turn the bot right in the orienting state
 */
void handleOrientTurnRight(void)
{
  rightMotorForward();
  leftMotorBackward();
  if (TURN_COMPLETE){
    TURN_COMPLETE = false;
    state = ORIENT_DRIVE_FORWARD;
  }
}


/*
 * This function is used to turn the bot left in the orienting state
 */
void handleOrientTurnLeft(void)
{
  rightMotorBackward();
  leftMotorForward();
  if (TURN_COMPLETE){
    TURN_COMPLETE = false;
    state = ORIENT_DRIVE_FORWARD;
  }
}


/********************** POT RETRIEVAL HANDLING *******************/

/*
 * This function is used to turn the robot to the right during
 * pot retrieval states
 */
void handleGetPotTurnRight(void)
{
  rightMotorForward();
  leftMotorBackward();
  if (TURN_COMPLETE){
    TURN_COMPLETE = false;
    state = GET_POT_DRIVE_FORWARD;
  }
}


/*
 * This function is used to turn the robot ot the left during pot
 * retrieval states
 */
void handleGetPotTurnLeft(void)
{
  rightMotorBackward();
  leftMotorForward();
  if (TURN_COMPLETE){
    TURN_COMPLETE = false;
    state = GET_POT_DRIVE_FORWARD;
  }
}


/*
 * This function is used to drive the robot forward during pot retrieval.
 * It checks if it needs to turn left at the interesection, turn
 * right at the customer window, and stop once it has reached the burner
 */
void handleGetPotDriveForward(void)
{
  rightMotorForward();
  leftMotorForward();
  if (TestForAtCustomerWindowIntersection()) RespToAtCustomerWindowIntersection();
  if (TestForAtCustomerWindowWall()) RespToAtCustomerWindowWall();
  if (TestForPotOnBurner()) RespToPotOnBurner();
}


/********************* IGNITION ON HANDLING *********************/


/*
 * This function is used to reverse the robot during the ignition on 
 * sequence. It will reverse for a fixed amount of time to back up
 * from the burner, and will also reverse until the limit switch on
 * the tail is triggered, signifying it has hit the burner
 */
void handleTurnOnIgnitionReverse(void)
{
  rightMotorBackward();
  leftMotorBackward();
  if(TestForIgnitionStopReversing()) RespToIgnitionStopReversing();
  if(TestForTailLimitSwitchTriggered()){
    state = DISPENSE_BALL_DRIVE_FORWARD;
  }
}


/*
 * This function is used to turn the bot right in the ignition on
 * sequence
 */
void handleTurnOnIgnitionTurnRight(void)
{
  rightMotorForward();
  leftMotorBackward();
  if (TURN_COMPLETE){
    TURN_COMPLETE = false;
    state = TURN_ON_IGNITION_REVERSE;
  }
}


/******************** BALL DISPENSING HANDLING ******************/

/*
 * This function is used to turn drive the bot forward for dispensing 
 * the balls. It is used to reach the interesection from the burner
 * and it is used  to drive up to the pot
 */
void handleDispenseBallDriveForward(void)
{
  rightMotorForward();
  leftMotorForward();
  if(TestForFrontLimitSwitchTriggered()){
    state = DISPENSE_BALL_OPEN_GATE;
  }
  if (TestForAtPotIntersectionFromBurner()) RespToAtPotIntersectionFromBurner();
}


/*
 * This function is used to turn left when approaching the pot to
 * dispense balls
 */
void handleDispenseBallTurnLeft(void)
{
  rightMotorForward();
  leftMotorBackward();
  if (TURN_COMPLETE){
    TURN_COMPLETE = false;
    state = DISPENSE_BALL_DRIVE_FORWARD;
  }
}




/*
 * This function is used to open the gate to dispense the balls
 */
void handleDispenseBallOpenGate(void)
{
  rightMotorOff();
  leftMotorOff();
  dispenserMotorForward();
  if(TestForDispenserFrontLimitSwitchTriggered()){
    state = DISPENSE_BALL_CLOSE_GATE;
  }
}




/*
 * This function is used to close the gate after the balls
 * have been dispensed
 */
void handleDispenseBallCloseGate(void)
{
  rightMotorOff();
  leftMotorOff();
  dispenserMotorBackward();
  if(TestForDispensorBackLimitSwitchTriggered()){
    dispenserMotorOff();
    state = TURN_OFF_IGNITION_REVERSE;
  }
}


/*
 * This function is used to reverse when turning off the ignition.
 */
void handleTurnOffIgnitionReverse(void)
{
  rightMotorBackward();
  leftMotorBackward();
  if(TestForInlineWithIgnition()) RespToInlineWithIgnition();
  if (TestForTailLimitSwitchTriggered()){
    state = WAITING_FOR_GAME_END;
  }
}


/*
 * This function is used to turn left when turning off the ignition
 */
void handleTurnOffIgnitionTurnLeft(void)
{
  rightMotorForward();
  leftMotorBackward();
  if (TURN_COMPLETE){
    TURN_COMPLETE = false;
    state = TURN_OFF_IGNITION_REVERSE;
  }
}


/*
 * This function is used to wait until the game timer to end
 */
void handleWaitingForGameEnd(void)
{
  rightMotorOff();
  leftMotorOff();
}


/*
 * This function plays the buzzer and turns off the robot
 */
void handleGameEnd(void)
{
  // play buzzer
  // motor off
}




