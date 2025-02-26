/*
 * This is the main file for the group 5 ME210 Final Project.
 * This file has some constant definitions, pin definitions for all
 * of the Arduino pins, defines states, and contains the state switching
 */

/********************* Initialize Timers ********************/
#define TIMER_INTERRUPT_DEBUG       0
#define _TIMERINTERRUPT_LOGLEVEL_     0

#define USE_TIMER_1     true
#define USE_TIMER_2     true
#include "TimerInterrupt.h"


/*--------------Constant Definitions------------------------*/

extern int GAME_TIMER;



/*----------------Pin Definitions---------------------------*/


const int START_BUTTON = 5; // PLACEHOLDER
const int TAIL_LIMIT_SWITCH = 5; // PLACEHOLDER
const int FRONT_LIMIT_SWITCH = 5; // PLACEHOLDER
const int DISPENSER_FRONT_LIMIT_SWITCH = 5; // PLACEHOLDER
const int DISPENSER_BACK_LIMIT_SWITCH = 5; // PLACEHOLDER


/*---------------State Definitions--------------------------*/
typedef enum {
  INIT_ORIENT_TURN_LEFT, ORIENT_DRIVE_FORWARD, ORIENT_TURN_RIGHT, ORIENT_TURN_LEFT,
  GET_POT_TURN_RIGHT, GET_POT_TURN_LEFT, GET_POT_DRIVE_FORWARD, TURN_ON_IGNITION_TURN_RIGHT,
  TURN_ON_IGNITION_REVERSE, DISPENSE_BALL_DRIVE_FORWARD, DISPENSE_BALL_TURN_LEFT,
  DISPENSE_BALL_OPEN_GATE, DISPENSE_BALL_CLOSE_GATE, TURN_OFF_IGNITION_REVERSE,
  TURN_OFF_IGNITION_TURN_LEFT, WAITING_FOR_GAME_END, GAME_END
} States_t;


States_t state;


void checkGlobalEvents(void) {
}






void setup()
{
  Serial.begin(9600);
 
  /*********** SET UP DIGITAL READ PINS ************/
  pinMode(START_BUTTON, INPUT);


  /*********** SET UP ANALOG WRITE PINS ************/


  /*********** BEGIN GAME ************/
  // PLAY BUZZER
  ITimer1.attachInterrupt(GAME_TIMER, GameTimerHandler);
  state = INIT_ORIENT_TURN_LEFT;


  Serial.println("Init Done");
}


void loop()
{
  checkGlobalEvents();
  switch (state) {
    /*
     * These states are used for orienting the robot. There is an initial turn
     * left as well as driving forward, left and right to orient and leave the
     * start zone aligned on the tape.
     */    
   
    /*
     * This state is used to turn the robot left until it is face
     * forward in the start zone
     */
    case INIT_ORIENT_TURN_LEFT:
      handleInitOrientTurnLeft();
      // Serial.println("State INIT_ORIENT_TURN_LEFT");
      break;
    /*
     * This state is used to drive the robot forward until it
     * exits the start zone
     */
    case ORIENT_DRIVE_FORWARD:
      handleOrientDriveForward();
      // Serial.println("State ORIENT_DRIVE_FORWARD");
      break;
    /*
     * This state is used to turn right and face the tape
     * if the robot is starting to the left of the tape
     */
    case ORIENT_TURN_RIGHT:
      handleOrientTurnRight();
      // Serial.println("State ORIENT_DRIVE_RIGHT");
      break;
    /*
     * This state is used to turn left and face the tape
     * if the robot is starting to the right of the tape
     */
    case ORIENT_TURN_LEFT:
      handleOrientTurnLeft();
      // Serial.println("State ORIENT_DRIVE_RIGHT");
      break;


    /*
     * These states are used for getting the pot from its starting location and bringing it to
     * the burner. There is a turn right, left, and forward
     */


    /*
     * This state is used to turn the robot right within the getting pot
     * sequence
     */
    case GET_POT_TURN_RIGHT:
      handleGetPotTurnRight();
      // Serial.println("State GET_POT_TURN_RIGHT");
      break;
    /*
     * This state is used to turn the robot left within the getting pot
     * sequence
     */
    case GET_POT_TURN_LEFT:
      handleGetPotTurnLeft();
      // Serial.println("State GET_POT_TURN_LEFT");
      break;
    /*
     * This state is used to drive the robot forward during the
     * getting pot sequence
     */
    case GET_POT_DRIVE_FORWARD:
      handleGetPotDriveForward();
      // Serial.println("State GET_POT_DRIVE_FORWARD");
      break;


    /*
     * These states are used for turning on the ignition
     * There is a turn right and a reverse state.
     */
   
    /*
     * This state is used to turn right when approaching the ignition to
     * turn it on
     */
    case TURN_ON_IGNITION_TURN_RIGHT:
      handleTurnOnIgnitionTurnRight();
      // Serial.println("State TURN_ON_IGNITION_TURN_RIGHT");
      break;
    /*
     * This state is used to reverse when approaching the ignition to turn it on
     */
    case TURN_ON_IGNITION_REVERSE:
      handleGetPotDriveForward();
      // Serial.println("State TURN_ON_IGNITION_REVERSE");
      break;
    /*
     * These states are used for dispensing the balls, after coming from the ignition.
     * There is a drive forward, turn fight, disepnse, and retract/reset.
     */
   
    /*
     * This state is used to drive forward when approaching the pot to dispense balls
     */
    case DISPENSE_BALL_DRIVE_FORWARD:
      handleDispenseBallDriveForward();
      // Serial.println("State DISPENSE_BALL_DRIVE_FORWARD");
      break;
    /*
     * This state is used to turn left when approaching the pot to dispense balls
     */
    case DISPENSE_BALL_TURN_LEFT:
      handleDispenseBallDTurnLeft();
      // Serial.println("State DISPENSE_BALL_TURN_LEFT");
      break;
    /*
     * This is the state that will dispense balls by opening the gate
     * once at the pot
     */
    case DISPENSE_BALL_OPEN_GATE:
      handleDispenseBallOpenGate();
      // Serial.println("State DISPENSE_BALL_OPEN_GATE");
      break;
    /*
     * This is the state that will dispense balls by closing the gate
     * once at the pot
     */
    case DISPENSE_BALL_CLOSE_GATE:
      handleDispenseBallCloseGate();
      // Serial.println("State DISPENSE_BALL_CLOSE_GATE");
      break;




    /*
     * These states are used for turning off the ignition near the end of the game.
     * There is a reverse state and a turn left state
     */




    /*
     * This state is used for reversing towards the ignition to turn it off
     */
    case TURN_OFF_IGNITION_REVERSE:
      handleTurnOffIgnitionReverse();
      // Serial.println("State TURN_OFF_IGNITION_REVERSE");
      break;
    /*
     * This state is used for turning left when approaching the ignition to
     * turn it off
     */
    case TURN_OFF_IGNITION_TURN_LEFT:
      handleTurnOffIgnitionTurnLeft();
      // Serial.println("State TURN_OFF_IGNITION_TURN_LEFT");
      break;


    /*
     * These states are the end states of the game. There is one state for waiting to end
     * and there is a state for turning off the bot
     */
   
    /*
     * This state has the robot wait until the game is over after turning
     * off the ignition
     */
    case WAITING_FOR_GAME_END:
      handleWaitingForGameEnd();
      // Serial.println("State WAITING_FOR_GAME_END");
      break;
    /*
     * This state plays the buzzer when the game ends and turns off the robot
     */
    case GAME_END:
      handleGameEnd();
      // Serial.println("State GAME_END");
      break;
    default:    // Should never get into an unhandled state
      Serial.println("SOMETHING IS WRONG, ROBOT IS COOKED :(");
  }
}







