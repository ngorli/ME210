extern States_t state;

/************************ MAP DISTANCE DEFINITIONS ********************/
float START_ZONE_LENGTH = 40.64; // in centimeters (16 inches)
float START_ZONE_WIDTH = 40.64; // in centimeters (16 inches)
float MAP_LENGTH = 91.44;  // in centimeters (36 inches)
float ROBOT_LENGTH =  17.78; // in centimeters (10 inches)
float RIGHT_TURN_DISTANCE = 35; // in centimeters


/************************** DISTANCE THRESHOLDS **********************/
float ULTRA_SONIC_DIST_THRESHOLD = 1.5; // PLACEHOLDER +- Threshold for determing what distances are equal
float RIGHT_TAPE_SENSOR_THRESHOLD = 40; // PLACEHOLDER Threshold for determining if the right taper sensor is triggered
float LEFT_TAPE_SENSOR_THRESHOLD = 40; // PLACEHOLDERThreshold for determining if the left taper sensor is triggered
float MIDDLE_TAPE_SENSOR_THRESHOLD = 40; // PLACEHOLDER Threshold for determining if the middle taper sensor is triggered
float END_OF_MAP_THRESHOLD = 35;
float CUSTOMER_WINDOW_THRESHOLD = 12.24; // PLAECHOLDER Threshold for determing if the customer window has been reached
float POT_AT_BURNER_THRESHOLD = 182.88;  // PLAECHOLDER Threshold for determing if the pot has reached the burner
float RIGHT_TURN_THRESHOLD = 1; // PLACEHOLDER Threshold for determining when to turn right after exiting starting square
float WALL_CLOSE_THRESH = 2; // PLACEHOLDER Threshould for determining if we are too close to the wall when finding or pushing the pot
float WALL_FAR_THRESH = 15; // PLACEHOLDER Threshould for determining if we are too far from the wall when finding or pushing the pot


/*********************** STATE DEFINITIONS ************************/
bool LEFT_OF_TAPE = false; // Used to determine which side of the tape you are on
bool ORIENT = false; // used to determine if orienting is complete
bool HAVE_POT = false; // used to determine if we have the pot for get pot states
bool SEARCHING_FOR_BURNER = false; // used to determine if we should be searching for the pot
bool tape_hunting = false; // used to control infinite right turn loop
bool reached_window = false;

bool first_pot_left = false;
bool second_pot_left = false;
bool third_pot_left = false;
bool correction_done_l = true;
bool correction_done_r = true;
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


/************************** SENSOR FUNCTIONS **********************/
/*
 * Returns true if the right tape sensor is triggered and false otherwise
 */
bool TestForRightTapeSensorTriggered(void) {
  // Serial.print("Print right tape sensor");
  // Serial.println(analogRead(RIGHT_TAPE_SENSOR));
  return analogRead(RIGHT_TAPE_SENSOR) > RIGHT_TAPE_SENSOR_THRESHOLD;
}


/*
 * Returns true if the left tape sensor is triggered and false otherwise
 */
bool TestForLeftTapeSensorTriggered(void) {
  return analogRead(LEFT_TAPE_SENSOR) > LEFT_TAPE_SENSOR_THRESHOLD;
}


/*
 * Returns true if the middle tape sensor is triggered and false otherwise
 */
bool TestForMiddleTapeSensorTriggered(void) {
  return analogRead(MIDDLE_TAPE_SENSOR) > MIDDLE_TAPE_SENSOR_THRESHOLD;
}


/************************** LANE DRIFT FUNCTIONS **********************/
/*
 * These Functions handle lane drifting
 */
bool TestForLaneDriftLeft(void) {
  if(correction_done_l){
    Serial.println("Correction Done L");
  } else {
    Serial.println("Correction Not Done L");
    Serial.print("Left Motor Speed: ");
    Serial.println(SPEED_L);
    Serial.print("Right Motor Speed: ");
    Serial.println(SPEED_R);
  }
  if (TestForMiddleTapeSensorTriggered()){
    correction_done_l = true;
  }
  if(TestForRightTapeSensorTriggered() or !correction_done_l) {
  // if(TestForRightTapeSensorTriggered()) {
    // Serial.println("Correct Left");
    SPEED_R = 0;
    correction_done_l = false;
  } else {
    SPEED_R = START_SPEED;
    correction_done_l = true;
  }
}

/*
 *
 */
bool TestForLaneDriftRight(void) {
  if(correction_done_r){
    Serial.println("Correction Done R");
  } else {
    Serial.println("Correction Not Done R");
    Serial.print("Left Motor Speed: ");
    Serial.println(SPEED_L);
    Serial.print("Right Motor Speed: ");
    Serial.println(SPEED_R);
  }
  if (TestForMiddleTapeSensorTriggered()){
    correction_done_r = true;
  }
  if(TestForLeftTapeSensorTriggered() or !correction_done_r) {
  // if(TestForLeftTapeSensorTriggered()) {
    // Serial.println("Correct Right");
    SPEED_L = 0;
    correction_done_r = false;
  } else {
    SPEED_L = START_SPEED;
    correction_done_r = true;
  }
}


// void RespToLaneDriftRight(void) {
 
// }


/******************** GENERAL FUNCTIONS ***********************/
/*
 * This function returns if the front and back ultrasonics are equal to
 * to the length
 */
bool TestForUltraSonicsEqual(void) {
  float front_reading = getUltraSonicFront();
  float back_reading = getUltraSonicBack();
  float left_reading = getUltraSonicLeft();
  // Serial.print("Front reading");
  // Serial.println(front_reading);
  // Serial.print("Back reading");
  // Serial.println(back_reading);

  // Serial.print("Thresh Value");
  // Serial.print(abs(MAP_LENGTH - (front_reading + back_reading + ROBOT_LENGTH)));

  if (abs(MAP_LENGTH - (front_reading + back_reading + ROBOT_LENGTH)) < ULTRA_SONIC_DIST_THRESHOLD) {
    return true;
  }
  return false;
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
  if (abs(getUltraSonicBack() - RIGHT_TURN_DISTANCE) < RIGHT_TURN_THRESHOLD ){
    return true;
  } else {
    return false;
  }
}

/*
 *
 */
void RespToBackSensorMoreThanStartZone(void) {
  // if(getUltraSonicLeft() < START_ZONE_WIDTH / 2) {    
  //   LEFT_OF_TAPE = true;
  //   state = ORIENT_TURN_RIGHT;
  //   Serial.println("turning right to find tape");
  // } else {
  //   LEFT_OF_TAPE = false;
  //   state = ORIENT_TURN_LEFT;
  //   Serial.println("turning left to find tape");
  // }
  state = ORIENT_TURN_RIGHT;
}


/*
 *
 */
bool TestForOriented(void) {
  Serial.println("trying to find tape");
  return TestForMiddleTapeSensorTriggered();
}

void RespToOriented(void) {
  Serial.println("found tape");
  ORIENT = true;
  if(LEFT_OF_TAPE){
    state = ORIENT_TURN_RIGHT;
  } else {
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
  // state = GET_POT_TURN_RIGHT;
}


/*
 *
 */
bool TestForAtCustomerWindowIntersection(void) {
  if ( (getUltraSonicFront() < END_OF_MAP_THRESHOLD)  && !reached_window ){
    reached_window = true;
    return true;
  } else {
    Serial.println(getUltraSonicFront());
    return false;
  }
}


void RespToAtCustomerWindowIntersection(void) {
  SPEED_R = START_SPEED;
  SPEED_L = START_SPEED;  
  state = GET_POT_TURN_LEFT;
  Serial.println("STATE SWITCH");
}


/*
 *
 */
bool TestForAtCustomerWindowWall(void) {
   if ( (getUltraSonicFront() < CUSTOMER_WINDOW_THRESHOLD)  && reached_window && !HAVE_POT) {
    return true;
  } else {
    return false;
  }
}

void RespToAtCustomerWindowWall(void) {
  HAVE_POT = true;
  SPEED_R = 200;
  SPEED_L = 200;
  state = GET_POT_TURN_LEFT;
}

void TestCloseToWall(void) {
  return (getUltraSonicRight() < WALL_CLOSE_THRESH);
}

void RespCloseToWall(void) {
  if(TestCloseToWall) {
    SPEED_L = 0;
  } else {
    SPEED_L = 255;
  }
}
void TestFarFromWall(void) {
  return (getUltraSonicRight() > WALL_FAR_THRESH);
}

void RespFarFromWall(void) {
  if(TestCloseToWall) {
    SPEED_R = 0;
  } else {
    SPEED_R = 255;
  }
}

/*
 *
 */
bool TestForPotOnBurner(void) {
  if (getUltraSonicBack() > POT_AT_BURNER_THRESHOLD && HAVE_POT) {
    return true;
  } else {
    return false;
  }
}

void RespToPotOnBurner(void) {
  state = TURN_ON_IGNITION_REVERSE;
}


/*
 *
 */
bool TestForIgnitionStopReversing(void){
  if (getUltraSonicBack() || (TestForMiddleTapeSensorTriggered() && !SEARCHING_FOR_BURNER)){
    return true;
  } else {
    return false;
  }
}

void RespToIgnitionStopReversing(void){
  state = TURN_ON_IGNITION_REVERSE;
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
  state = TURN_OFF_IGNITION_TURN_LEFT;
}