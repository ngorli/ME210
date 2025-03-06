extern States_t state;
extern bool TURN_COMPLETE;
extern bool IGNITION_REVERSE_COMPLETE;
extern bool DISPENSING_COMPLETE;

bool LINE_DETECTING = false;
float POT_DISPENSE_DIST = 30;
bool READY_TO_DISPENSE = false;


/*************************** OREINTATION HANDLING *******************/

/*
 * This function is used to play the buzzer sound
 */
void playBuzzer(void){
  // tone(BUZZER, 1000); // Play 1000Hz (1kHz) tone
  // delay(500);   // Wait 500ms
  // noTone(BUZZER);   // Stop tone
}


/*
 * This function is used to turn the robot left until it is facing
 * forward in the start zone
 */
void handleInitOrient(void)
{
  // if (TestForFrontLimitSwitchTriggered){
  //   Serial.println("LIMIT SWITCH ON");
  // } else {
  //   Serial.println("LIMIT SWITCH OFF");
  // }
  // state = DISPENSE_BALLS;

  // Serial.println(analogRead(LEFT_TAPE_SENSOR));
  rightMotorBackward();
  leftMotorForward();
  // // playBuzzer();
  // // Serial.println(getUltraSonicFront());
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
}




/*
 * This function is used to turn the bot right in the orienting state
 */
void handleOrientTurnRight(void)
{
  leftMotorForward();
  rightMotorBackward();
  Serial.println("Turn right");
  if (turnStartTime == 0) { 
    turnStartTime = millis();
  }
  if (millis() - turnStartTime >= 600) {
    turnStartTime = 0;
    state = START_TRACKING_TAPE;
  }
}


void handleStartTrackingTape(){
  TestForLaneDriftLeft();
  TestForLaneDriftRight();
  leftMotorForward();
  rightMotorForward();
    if (turnStartTime == 0) { 
    turnStartTime = millis();
  }
  if (millis() - turnStartTime >= 1750) {
    turnStartTime = 0;
    state = IGNITE_ON;
  }
}

void handleIgniteOn(){
  SPEED_R = START_SPEED;
  SPEED_L = START_SPEED;
  leftMotorBackward();
  rightMotorBackward();
  if (getUltraSonicBack() < 1){
    state = APPROACH_GET_POT;
  }
}

void handleApproachGetPot(){
  leftMotorForward();
  rightMotorForward();
  if (turnStartTime == 0) { 
    turnStartTime = millis();
  }
  if (millis() - turnStartTime >= 1000) {
    turnStartTime = 0;
    state = GET_POT_DRIVE_FORWARD;
  }
}


/********************** POT RETRIEVAL HANDLING *******************/

/*
 * This function is used to turn the robot ot the left during pot
 * retrieval states
 */
void handleGetPotTurnLeft(void)
{
  SPEED_R = START_SPEED;
  SPEED_L = START_SPEED;
  // Serial.println("GET POT TURN LEFT POST MOTOR");
  Serial.println(getUltraSonicFront());
  rightMotorForward();
  leftMotorBackward();
  // Serial.println("GET POT TURN LEFT AFTER MOTOR");

  if (turnStartTime == 0) { 
    turnStartTime = millis();
  }
  // Serial.print("Turn start time is ");
  // Serial.println(turnStartTime);
  if (millis() - turnStartTime >= TURN_TIMER) {
    turnStartTime = 0;
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
  Serial.print("Have pot is ");
  Serial.println(HAVE_POT);
  rightMotorForward();
  leftMotorForward();
  if(HAVE_POT){
    RespCloseToWall();
    RespFarFromWall();
  }
  if (TestForPotOnBurner()) RespToPotOnBurner();
  if (TestForAtCustomerWindowIntersection()) RespToAtCustomerWindowIntersection();
  if (TestForAtCustomerWindowWall()) RespToAtCustomerWindowWall();
  if (HAVE_POT){
    SPEED_R = 255;
    SPEED_L = 255;
  }
  TestForLaneDriftLeft();
  TestForLaneDriftRight();
}




void handleDispenseBallReverse(){
  rightMotorBackward();
  leftMotorBackward();
  if (turnStartTime == 0) { 
    turnStartTime = millis();
  }
  if (millis() - turnStartTime >= TURN_TIMER) {
    turnStartTime = 0;
    state = DISPENSE_BALL_TURN_LEFT;
  }
}

void handleDispenseBallTurnLeft(){
  rightMotorForward();
  leftMotorBackward();
  if (turnStartTime == 0) { 
    turnStartTime = millis();
  }
  if (millis() - turnStartTime >= TURN_TIMER) {
    turnStartTime = 0;
    state = DISPENSE_BALL_DRIVE_FORWARD;
  }
}

void handleDispenseBallDriveForward(){
  rightMotorForward();
  leftMotorForward();
  if(LINE_DETECTING && getUltraSonicFront() > POT_DISPENSE_DIST){
    TestForLaneDriftLeft();
    TestForLaneDriftRight();
  }
  if (TestForMiddleTapeSensorTriggered() && !LINE_DETECTING){
    LINE_DETECTING = true;
    state = DISPENSE_BALL_TURN_RIGHT;
  }
  if(TestForMiddleTapeSensorTriggered() && TestForLeftTapeSensorTriggered() && TestForRightTapeSensorTriggered()){
    READY_TO_DISPENSE = true;
    LINE_DETECTING = false;
    state = DISPENSE_BALL_TURN_RIGHT;
  }
}

void handleDispenseBallTurnRight(){
  rightMotorBackward();
  leftMotorForward();
  if (turnStartTime == 0) { 
    turnStartTime = millis();
  }
  if (millis() - turnStartTime >= TURN_TIMER) {
    turnStartTime = 0;
    if (READY_TO_DISPENSE) {
      state = FINAL_DISPENSE_BALL_FORWARD;
    } else {
      state = DISPENSE_BALL_DRIVE_FORWARD;
    }
  }
}

void handleFinalDispenseBallForward(){
  rightMotorForward();
  leftMotorForward();
  if (getUltraSonicFront() < 3){
    state = DISPENSE_BALLS;
  }
}

/*
 * This function is used to open the gate to dispense the balls
 */
 void handleDispenseBalls(void)
 {
   myServo.write(45);
   delay(2000);  // Wait for 2 seconds
   myServo.write(0);
   delay(2000);  // Wait for 2 seconds
   state = WAITING_FOR_GAME_END;
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


/******************** BALL DISPENSING HANDLING ******************/



/*
 * This function is used to turn the bot right in the ignition on
 * sequence
 */
void handleTurnOnIgnitionTurnRight(void)
{
  rightMotorBackward();
  leftMotorForward();
  if (!turnComplete) {
    if (turnStartTime == 0) { 
      turnStartTime = millis();
    }
    if (millis() - turnStartTime >= TURN_TIMER) {
      // turnComplete = true;
      turnStartTime = 0;
      state = TURN_ON_IGNITION_REVERSE;
    }
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
  if (!turnComplete) {
    if (turnStartTime == 0) { 
      turnStartTime = millis();
    }
    if (millis() - turnStartTime >= TURN_TIMER) {
      // turnComplete = true;
      turnStartTime = 0;
      state = TURN_OFF_IGNITION_REVERSE;
    }
  }
}




/*
 * This function is used to wait until the game timer to end
 */
void handleWaitingForGameEnd(void)
{
  rightMotorOff();
  leftMotorOff();
  if (gameRunning && (millis() - gameStartTime >= GAME_TIMER)) {
    gameRunning = false;
    state = GAME_END;
  }
}




/*
 * This function plays the buzzer and turns off the robot
 */
void handleGameEnd(void)
{
  // play buzzer
  // motor off
}