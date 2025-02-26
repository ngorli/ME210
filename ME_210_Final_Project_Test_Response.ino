extern States_t state;
extern int GAME_TIMER;
extern int TURN_TIMER;
extern int IGNITION_ON_TIMER;
extern int DISPENSING_TIMER;
extern bool TURN_COMPLETE;
extern bool IGNITION_REVERSE_COMPLETE;
extern bool DISPENSING_COMPLETE;

/************************ MAP DISTANCE DEFINITIONS ********************/
float START_ZONE_LENGTH = 40.64; // in centimeters (16 inches)
float START_ZONE_WIDTH = 40.64; // in centimeters (16 inches)
float MAP_LENGTH = 91.44;  // in centimeters (36 inches)
float ROBOT_LENGTH =  25.40; // in centimeters (10 inches)

/************************** DISTANCE THRESHOLDS **********************/
float ULTRA_SONIC_DIST_THRESHOLD = 0.1; // PLACEHOLDER +- Threshold for determing what distances are equal
float RIGHT_TAPE_SENSOR_THRESHOLD = 100; // PLACEHOLDER Threshold for determining if the right taper sensor is triggered
float LEFT_TAPE_SENSOR_THRESHOLD = 100; // PLACEHOLDERThreshold for determining if the left taper sensor is triggered
float MIDDLE_TAPE_SENSOR_THRESHOLD = 100; // PLACEHOLDER Threshold for determining if the middle taper sensor is triggered
float CUSTOMER_WINDOW_THRESHOLD = 100; // PLAECHOLDER Threshold for determing if the customer window has been reached
float POT_AT_BURNER_THRESHOLD = 100;  // PLAECHOLDER Threshold for determing if the pot has reached the burner


/*********************** STATE DEFINITIONS ************************/
bool LEFT_OF_TAPE = false; // Used to determine which side of the tape you are on
bool ORIENT = false; // used to determine if orienting is complete
bool HAVE_POT = false; // used to determine if we have the pot for get pot states
bool SEARCHING_FOR_BURNER = false; // used to determine if we should be searching for the pot


/********************* LIMIT SWITCH FUNCTIONS *********************/


/*
 * Returns true if the tail limit switch is triggered and false otherwise
 */
bool TestForTailLimitSwitchTriggered(void) {
  return (TAIL_LIMIT_SWITCH == HIGH);
}


/*
 * Returns true if the front limit switc is triggered and false otherwise
 */
bool TestForFrontLimitSwitchTriggered(void) {
  return (FRONT_LIMIT_SWITCH == HIGH);
}


bool TestForDispenserFrontLimitSwitchTriggered(void) {
  return (DISPENSER_FRONT_LIMIT_SWITCH == HIGH);
}


bool TestForDispensorBackLimitSwitchTriggered(void) {
  return (DISPENSER_BACK_LIMIT_SWITCH == HIGH);
}



/************************** SENSOR FUNCTIONS **********************/


/*
 * Returns true if the right tape sensor is triggered and false otherwise
 */
bool TestForRightTapeSensorTriggered(void) {
  // return RIGHT_TAPE_SENSOR > RIGHT_TAPE_SENSOR_THRESHOLD;
}


/*
 * Returns true if the left tape sensor is triggered and false otherwise
 */
bool TestForLeftTapeSensorTriggered(void) {
  // return LEFT_TAPE_SENSOR > LEFT_TAPE_SENSOR_THRESHOLD;
}


/*
 * Returns true if the middle tape sensor is triggered and false otherwise
 */
bool TestForMiddleTapeSensorTriggered(void) {
  // return MIDDLE_TAPE_SENSOR > MIDDLE_TAPE_SENSOR_THRESHOLD;
}



/************************** LANE DRIFT FUNCTIONS **********************/
/*
 * These Functions handle lane drifting
 */
bool TestForLaneDriftLeft(void) {
  return TestForLeftTapeSensorTriggered();
}

void RespToLaneDriftLeft(void) {
 
}



/*
 *
 */
bool TestForLaneDriftRight(void) {
  return TestForRightTapeSensorTriggered();
}

void RespToLaneDriftRight(void) {
 
}





/******************** GENERAL FUNCTIONS ***********************/

/*
 * This function returns if the front and back ultrasonics are equal to
 * to the length
 */
bool TestForUltraSonicsEqual(void) {
  float front_reading = getUltraSonicFront();
  float back_reading = getUltraSonicBack();
  return abs(MAP_LENGTH - (getUltraSonicFront() + getUltraSonicBack() + ROBOT_LENGTH)) < ULTRA_SONIC_DIST_THRESHOLD;
}


/*
 * This function returns true if the back ultrasonic reading is less than the front ultrasonic reading
 */
bool TestForBackLessThanFront(void) {
  return getUltraSonicBack() < getUltraSonicFront();
}


/*
 * This function uses the two prior functions to return true if the pot is facing forwards and
 * false otherwise
 */
bool TestForUltraSonicsEqualAndBackLessThanStartZone(void) {
  return (TestForUltraSonicsEqual() && TestForBackLessThanFront());
}

/*
 * This function switches the state to drive forward if the orientation test passes
 */
void RespToUltraSonicsEqualAndBackLessThanStartZone(void) {
  state = ORIENT_DRIVE_FORWARD;
}





/*
 *
 */
bool TestForBackSensorMoreThanStartZone(void) {
  return getUltraSonicBack() > START_ZONE_LENGTH;
}

/*
 *
 */
void RespToBackSensorMoreThanStartZone(void) {
  if(getUltraSonicLeft() < START_ZONE_WIDTH / 2) {    
    LEFT_OF_TAPE = true;
    ITimer2.attachInterrupt(TURN_TIMER, TurnTimerHandler);
    state = ORIENT_TURN_RIGHT;
  } else {
    //start turning timer
    LEFT_OF_TAPE = false;
    ITimer2.attachInterrupt(TURN_TIMER, TurnTimerHandler);
    state = ORIENT_TURN_LEFT;
  }
}







/*
 *
 */
bool TestForOriented(void) {
  return TestForMiddleTapeSensorTriggered();
}

void RespToOriented(void) {
  ORIENT = true;
  if(LEFT_OF_TAPE){
    ITimer2.attachInterrupt(TURN_TIMER, TurnTimerHandler);
    state = ORIENT_TURN_LEFT;
  } else {
    ITimer2.attachInterrupt(TURN_TIMER, TurnTimerHandler);
    state = ORIENT_TURN_RIGHT;
  }
}




/*
 *
 */
bool TestForGetPot(void) {
  if(ORIENT && TestForMiddleTapeSensorTriggered() && TestForRightTapeSensorTriggered()){
    return true;
  } else {
    return false;
  }
}


void RespToGetPot(void) {
  ITimer2.attachInterrupt(TURN_TIMER, TurnTimerHandler);
  state = GET_POT_TURN_RIGHT;
}







/*
 *
 */
bool TestForAtCustomerWindowIntersection(void) {
  if(TestForMiddleTapeSensorTriggered() && TestForRightTapeSensorTriggered() && TestForLeftTapeSensorTriggered()){
    return true;
  } else {
    return false;
  }
}


void RespToAtCustomerWindowIntersection(void) {
  ITimer2.attachInterrupt(TURN_TIMER, TurnTimerHandler);
  state = GET_POT_TURN_LEFT;
}






/*
 *
 */
bool TestForAtCustomerWindowWall(void) {
  if (getUltraSonicFront() < CUSTOMER_WINDOW_THRESHOLD && !HAVE_POT) {
    return true;
  } else {
    return false;
  }
}

void RespToAtCustomerWindowWall(void) {
  HAVE_POT = true;
  ITimer2.attachInterrupt(TURN_TIMER, TurnTimerHandler);
  state = GET_POT_TURN_LEFT;
}





/*
 *
 */
bool TestForPotOnBurner(void) {
  if (getUltraSonicFront() < POT_AT_BURNER_THRESHOLD && HAVE_POT) {
    return true;
  } else {
    return false;
  }
}

void RespToPotOnBurner(void) {
  ITimer2.attachInterrupt(IGNITION_ON_TIMER, IgnitionOnTimerHandler);
  state = TURN_ON_IGNITION_REVERSE;
}








/*
 *
 */
bool TestForIgnitionStopReversing(void){
  if (IGNITION_REVERSE_COMPLETE || (TestForMiddleTapeSensorTriggered() && !SEARCHING_FOR_BURNER)){
    return true;
  } else {
    return false;
  }
}

void RespToIgnitionStopReversing(void){
  ITimer2.attachInterrupt(TURN_TIMER, TurnTimerHandler);
  state = TURN_ON_IGNITION_REVERSE;
}





void StartGateTimer(){
  ITimer2.attachInterrupt(DISPENSING_TIMER, DispensingTimerHandler);
}


/*
 *
 */
bool TestForAtPotIntersectionFromBurner(void){
  if (TestForRightTapeSensorTriggered() && TestForMiddleTapeSensorTriggered() && TestForLeftTapeSensorTriggered()){
    return true;
  } else {
    return false;
  }
}

void RespToAtPotIntersectionFromBurner(void){
  ITimer2.attachInterrupt(TURN_TIMER, TurnTimerHandler);
  state = DISPENSE_BALL_TURN_LEFT;
}








/*
 *
 */
bool TestForInlineWithIgnition(void) {
  if (TestForRightTapeSensorTriggered() && TestForMiddleTapeSensorTriggered() && TestForLeftTapeSensorTriggered()){
    return true;
  } else {
    return false;
  }
}

void RespToInlineWithIgnition(void){
  ITimer2.attachInterrupt(TURN_TIMER, TurnTimerHandler);
  state = TURN_OFF_IGNITION_TURN_LEFT;
}
