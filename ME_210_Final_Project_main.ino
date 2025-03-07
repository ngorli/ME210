/*
 * This is the main file for the group 5 ME210 Final Project.
 * This file has some constant definitions, pin definitions for all
 * of the Arduino pins, defines states, and contains the state switching
 */


/**************** Library Inclusion *************************/
#include <Servo.h>
#include <NewPing.h>


/********************* Initialize Timers ********************/
const unsigned long GAME_TIMER = 130000; // 130 seconds
unsigned long gameStartTime = 0;
bool gameRunning = true;

// Timer durations
const unsigned long TURN_TIMER = 900;
const unsigned long IGNITION_ON_TIMER = 1500;
const unsigned long DISPENSING_TIMER = 1500;

// Start times for each action
unsigned long turnStartTime = 0;
unsigned long ignitionStartTime = 0;
unsigned long dispensingStartTime = 0;

// Flags to track when actions are complete
bool turnComplete = false;
bool ignitionReverseComplete = false;

/********************** Initialize Servo *******************/
Servo myServo;


/******************** Pin Definitions ***********************/

const int BUZZER = 10; // PLACEHOLDER
const int TAIL_LIMIT_SWITCH = 25; // PLACEHOLDER
const int FRONT_LIMIT_SWITCH = 50; // PLACEHOLDER

const int START_SPEED_L = 100;
const int START_SPEED_R = 108;

//Ultrasonic Sensors

const int trigBack = 36, echoBack = 37;   // 41 is trigger-green, 40 is echo-white
NewPing backSensor(trigBack, echoBack, 300);
const int trigFront = 39, echoFront = 38; // 40 is trigger-white, 41 is echo-green
NewPing frontSensor(trigFront, echoFront, 300);
const int trigLeft = 52, echoLeft = 53; // 52 is trigger-white, 53 is echo-green
NewPing leftSensor(trigLeft, echoLeft, 300);
const int trigRight = 42, echoRight = 43; // 43 is trigger-green, 27 is echo-white
NewPing rightSensor(trigRight, echoRight, 300);

// Motors
const int INPUT1_L = 46; //Left motor
const int INPUT2_L = 47;
const int ENA_L = 2; //PWM
int SPEED_L = START_SPEED_L;
int SPEED_R = START_SPEED_R;
int SPEED_DOWN_L = 77;
int SPEED_DOWN_R = 82;
const int INPUT1_R = 44; // Right Motor
const int INPUT2_R = 45; //
const int ENA_R = 5;

const int SERVO = 29;


// Tape sensors
const int RIGHT_TAPE_SENSOR = A3;
const int MIDDLE_TAPE_SENSOR = A4;
const int LEFT_TAPE_SENSOR = A5;

/*---------------State Definitions--------------------------*/
typedef enum {
  INIT_ORIENT, ORIENT_DRIVE_FORWARD, ORIENT_TURN_RIGHT,
  GET_POT_TURN_LEFT, GET_POT_DRIVE_FORWARD, TURN_ON_IGNITION_TURN_RIGHT,
  TURN_ON_IGNITION_REVERSE, DISPENSE_BALL_DRIVE_FORWARD, DISPENSE_BALL_TURN_LEFT,
  DISPENSE_BALLS, TURN_OFF_IGNITION_REVERSE, TURN_OFF_IGNITION_TURN_LEFT, 
  WAITING_FOR_GAME_END, GAME_END,
  APPROACH_GET_POT, IGNITE_ON, START_TRACKING_TAPE,
  DISPENSE_BALL_REVERSE, DISPENSE_BALL_TURN_RIGHT, FINAL_DISPENSE_BALL_FORWARD
} States_t;




States_t state;




void checkGlobalEvents(void) {
}




void setup()
{
  Serial.begin(9600);


  Serial.println("Start Init");
 
  /*********** SET UP DIGITAL READ PINS ************/
  pinMode(trigBack, OUTPUT);
  pinMode(echoBack, INPUT);
 
  pinMode(trigFront, OUTPUT);
  pinMode(echoFront, INPUT);
 
  pinMode(trigLeft, OUTPUT);
  pinMode(echoLeft, INPUT);

  pinMode(BUZZER, OUTPUT);

  pinMode(TAIL_LIMIT_SWITCH, INPUT);
  pinMode(FRONT_LIMIT_SWITCH, INPUT);


  myServo.attach(SERVO);



  /*********** SET UP ANALOG WRITE PINS ************/




  /*********** BEGIN GAME ************/
  // playBuzzer();
  gameStartTime = millis();
  state = GET_POT_DRIVE_FORWARD;


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
    case INIT_ORIENT:
      handleInitOrient();
     Serial.println("State INIT_ORIENT");
      break;
    /*
     * This state is used to drive the robot forward until it
     * exits the start zone
     */
    case ORIENT_DRIVE_FORWARD:
      handleOrientDriveForward();
     Serial.println("State ORIENT_DRIVE_FORWARD");
      break;
    /*
     * This state is used to turn left and face the tape
     * if the robot is starting to the right of the tape
     */
    case ORIENT_TURN_RIGHT:
      handleOrientTurnRight();
     Serial.println("State ORIENT_DRIVE_RIGHT");
      break;

    case START_TRACKING_TAPE:
      handleStartTrackingTape();
     Serial.println("State START_TRACKING_TAPE");
      break;
    
    case IGNITE_ON:
      handleIgniteOn();
     Serial.println("State IGNITE_ON");
      break;
    case APPROACH_GET_POT:
      handleApproachGetPot();
     Serial.println("State APPROACH_GET_POT");
      break;


    /*
     * These states are used for getting the pot from its starting location and bringing it to
     * the burner. There is a turn right, left, and forward
     */


    /*
     * This state is used to turn the robot left within the getting pot
     * sequence
     */
    case GET_POT_TURN_LEFT:
      handleGetPotTurnLeft();
      Serial.println("State GET_POT_TURN_LEFT");
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
      Serial.println("State TURN_ON_IGNITION_TURN_RIGHT");
      break;
    /*
     * This state is used to reverse when approaching the ignition to turn it on
     */
    case TURN_ON_IGNITION_REVERSE:
      handleTurnOnIgnitionReverse();
      Serial.println("State TURN_ON_IGNITION_REVERSE");
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
      Serial.println("State DISPENSE_BALL_DRIVE_FORWARD");
      break;
    case FINAL_DISPENSE_BALL_FORWARD:
      handleFinalDispenseBallForward();
      Serial.println("State DISPENSE_BALL_DRIVE_FORWARD");
      break;
    /*
    /*
     * This state is used to turn left when approaching the pot to dispense balls
     */
    case DISPENSE_BALL_TURN_LEFT:
      handleDispenseBallTurnLeft();
      Serial.println("State DISPENSE_BALL_TURN_LEFT");
      break;
    
    
      case DISPENSE_BALL_TURN_RIGHT:
        handleDispenseBallTurnRight();
        Serial.println("State DISPENSE_BALL_TURN_RIGHT");
        break;
    
    
      case DISPENSE_BALL_REVERSE:
        handleDispenseBallReverse();
        Serial.println("State DISPENSE_BALL_REVERSE");
        break;
    /*
     * This is the state that will dispense balls by opening the gate
     * once at the pot
     */
    case DISPENSE_BALLS:
      handleDispenseBalls();
      Serial.println("State DISPENSE_BALL_OPEN_GATE");
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
      Serial.println("State TURN_OFF_IGNITION_REVERSE");
      break;
    /*
     * This state is used for turning left when approaching the ignition to
     * turn it off
     */
    case TURN_OFF_IGNITION_TURN_LEFT:
      handleTurnOffIgnitionTurnLeft();
      Serial.println("State TURN_OFF_IGNITION_TURN_LEFT");
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