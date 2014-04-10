#include "pins.h"
#include <motors.h>
#include <linefollow.hpp>
#include <linefollow_2.hpp>
#include <colorSensor.h>
#include <Servo.h>

enum State { START = 1, MAIN_LINE, TURN_LEFT_ONTO_SIDE, STRAIGHTEN_OUT_ON_SIDE_LINE, BACKUP_ON_SIDE_LINE, 
                SIDE_LINE_START, FIRE, GO_BACK, TURN_RIGHT_ONTO_MAIN_LINE, BACKUP_ON_MAIN_LINE, RESET };

//Hardware interfaces
static Motors motors;
static ParallelLineFollower frontParallelLineFollower;
static ParallelLineFollower backParallelLineFollower;
static ColorSensor colorSensor;
static unsigned long LAST_LINE_WAIT_TIME = 4000;
static unsigned long INITIAL_WAIT_TIME = 60000;
static unsigned long BACKUP_ON_MAIN_LINE_WAIT_TIME = 2000;
static unsigned long FORWARD_ON_MAIN_LINE_WAIT_TIME = 500;
//Firing variables--------------------
//Different Servo Positions
static const int SERVO_FIRE_POSITION = 244;
static const int SERVO_REST_POSITION = 2400;
static const int DIFFERENT_SERVO_FIRE_POSITION = 2000;
static const int DIFFERENT_SERVO_REST_POSITION = 500;
static bool timerFlipped = false;
//Servo Delays
static const int TRIGGER_DELAY = 1;
static const int NOTIFY_DELAY = 1;
// The three firing barrel servos
static Servo firing_servo_1, firing_servo_2, firing_servo_3;
//Firing wait time and start time variable
static const unsigned long FIRE_WAIT_TIME = 3000;
static unsigned long startTime;
// Set by hardware interrupt, indicates whether or not the camera is aiming at the target and ready to fire
volatile boolean fire = false;
// Interrupt handler that triggers on change of the ready to fire notification pin
void fire_handler() {
  fire = ( digitalRead( READY_TO_FIRE_PIN ) == LOW );
}
//-----------------------------------

//Current state 
static State state = START;

//bytes received from line follower
static byte leftLineFollowBits;
static byte rightLineFollowBits;

static int lineCount = 0;

//Setup variables etc
void setup() {

    Serial.begin(9600);
    Serial.println("IN the beginning...");

    //Motor variables setup-----------
    motors.setup(PIN_PWM_LEFT, PIN_DIRECTION_LEFT, PIN_PWM_RIGHT, PIN_DIRECTION_RIGHT, DEFAULT_SPEED);
    motors.motorsStop();
    //-------------------------
    
    //Parallel Line Follower Setup------------------------------------------------- 
    frontParallelLineFollower.setup(PIN_SENSOR_FRONT, 
                                PIN_LF_FRONT_S0, PIN_LF_FRONT_S1, 
                                PIN_LF_FRONT_S2, PIN_LF_FRONT_S3, 
                                PIN_LF_FRONT_S4, PIN_LF_FRONT_S5, 
                                PIN_LF_FRONT_S5, PIN_LF_FRONT_S7, 
                                true);
    backParallelLineFollower.setup(PIN_SENSOR_BACK, 
                                PIN_LF_BACK_S0, PIN_LF_BACK_S1, 
                                PIN_LF_BACK_S2, PIN_LF_BACK_S3, 
                                PIN_LF_BACK_S4, PIN_LF_BACK_S5, 
                                PIN_LF_BACK_S5, PIN_LF_BACK_S7, 
                                true);

    //-----------------------------------------------------

    //Color Sensor Setup--------------------
    colorSensor.setup(PIN_CS_S0, PIN_CS_S1, PIN_CS_S2, PIN_CS_S3, PIN_CS_OUT, PIN_CS_LED);
    //--------------------------------------

    //Firing Control setup----------------------------------------------
    
    firing_servo_1.attach( FIRING_SERVO_1_PIN );
    firing_servo_1.writeMicroseconds( SERVO_REST_POSITION );
    firing_servo_2.attach( FIRING_SERVO_2_PIN );
    firing_servo_2.writeMicroseconds( SERVO_REST_POSITION );
    firing_servo_3.attach( FIRING_SERVO_3_PIN );
    firing_servo_3.writeMicroseconds( DIFFERENT_SERVO_REST_POSITION );

    pinMode( READY_TO_FIRE_PIN, INPUT_PULLUP );
    // when this pin is pulled low, the servo will fire
    attachInterrupt( READY_TO_FIRE_INTERRUPT, fire_handler, CHANGE );
    pinMode( START_AIMING_PIN, OUTPUT );
    digitalWrite( START_AIMING_PIN, LOW );
    //*///----------------------------------------------------------
  
    delay(3000);

    Serial.println("waiting to go.....");

    /*
    while (1){
        if (digitalRead(BUMPER_SWITCH) != HIGH){
            Serial.println("WALL");
        }
        else 
        {
            Serial.println("NO WALL");
        }
    }
    //*/
    

    //Loop until start
    while((digitalRead(PIN_START) != HIGH));
    
    //Delay so button push in loop below is not activated
    //REMOVE THIS BEFORE COMPETITION
    delay(500);
}

//Loop
void loop() {

    //If we get a button press, stop
    //REMOVE THIS BEFORE COMPETITION
    if(digitalRead(PIN_START) == HIGH) {
        Serial.println("Stopped, state machine reset!");
        motors.motorsStop();
        state = RESET;
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    //~~~Dat State Machine~~~
    switch (state) {

        case START:
            Serial.println("START");

            if (!timerFlipped) {
                startTime = millis();
            }

            if (colorSensor.getColor() == GREEN || (startTime + INITIAL_WAIT_TIME <= millis())) {
                motors.motorsDrive(FORWARD);
                delay(1000);
                state = MAIN_LINE;
                timerFlipped = false;
            }
            break;

        case MAIN_LINE:
            Serial.println("straight line");
            
            //run to the end on a timer
            if (lineCount == 3) {
                if (!timerFlipped) {
                    startTime = millis();
                }
                timerFlipped = true;
                if ((startTime + LAST_LINE_WAIT_TIME) <= millis()) {

                    motors.motorsStop();
                }
            }
            ///-------------


            if (lineCount != 3 && frontParallelLineFollower.intersection(leftLineFollowBits, rightLineFollowBits)) {

                state = TURN_LEFT_ONTO_SIDE;
                break; 
            }
            else if (frontParallelLineFollower.isCentered(leftLineFollowBits, rightLineFollowBits)) {
                Serial.println("FORWARD!");
                motors.motorsDrive(FORWARD);
                break;
            }
            else {

                if (!frontParallelLineFollower.isCentered(leftLineFollowBits, rightLineFollowBits) 
                    && (leftLineFollowBits > rightLineFollowBits)) {
                    
                    motors.motorsTurnLeft();
                }

                if (!frontParallelLineFollower.isCentered(leftLineFollowBits, rightLineFollowBits) 
                    && (leftLineFollowBits < rightLineFollowBits)) {

                    motors.motorsTurnRight(); 
                }
                break;
            }

         case TURN_LEFT_ONTO_SIDE:
            Serial.println("turning left onto side!!");
            do {
                motors.motorsDrive(FORWARD);
                delay(500);
                motors.motorsTurnLeft();
                delay(500);
                frontParallelLineFollower.Get_Line_Data(leftLineFollowBits, rightLineFollowBits);
            } while (leftLineFollowBits || rightLineFollowBits);

            do {
                motors.motorsTurnLeft();
                //motors.motorsDrive(FORWARD);
                //delay(100);
                frontParallelLineFollower.Get_Line_Data(leftLineFollowBits, rightLineFollowBits);
            } while (leftLineFollowBits || rightLineFollowBits);

            do {
                motors.motorsTurnLeft();
                //motors.motorsDrive(FORWARD);
                //delay(100);
                frontParallelLineFollower.Get_Line_Data(leftLineFollowBits, rightLineFollowBits);
            } while (!frontParallelLineFollower.isCentered(leftLineFollowBits, rightLineFollowBits)); //!leftLineFollowBits && !rightLineFollowBits);
            
            motors.motorsStop();
            delay(4000);
            state = STRAIGHTEN_OUT_ON_SIDE_LINE;
            break;

        case STRAIGHTEN_OUT_ON_SIDE_LINE:
            Serial.println("STRAIGHTEN_OUT_ON_SIDE_LINE");
            //Straighten out
            backParallelLineFollower.Get_Line_Data(leftLineFollowBits, rightLineFollowBits);
            if (leftLineFollowBits == 0 && rightLineFollowBits == 0) {
                Serial.println("motorsTurnLeft, bits all 00s");

                motors.motorsTurnLeft();                
            }
            else if (backParallelLineFollower.isCentered(leftLineFollowBits, rightLineFollowBits) 
                && leftLineFollowBits < 4 && rightLineFollowBits < 4) {

                state = BACKUP_ON_SIDE_LINE;
                break;
            }
            else {
                Serial.println("elsed");
                if (!backParallelLineFollower.isCentered(leftLineFollowBits, rightLineFollowBits) && (leftLineFollowBits > rightLineFollowBits)) {
                    Serial.println("Correct to left");
                    motors.motorsTurnLeft();
                    motors.motorsDrive(BACKWARD);
                }
                else {//if (!backParallelLineFollower.isCentered(leftLineFollowBits, rightLineFollowBits) && (leftLineFollowBits <= rightLineFollowBits)) {
                    Serial.println("Correct to right");
                    motors.motorsTurnRight(); 
                    motors.motorsDrive(BACKWARD);
                }
            }
            break;
        case BACKUP_ON_SIDE_LINE:

            if (lineCount == 0 || lineCount == 2) {
                if (digitalRead(BUMPER_SWITCH) != HIGH) {

                    motors.motorsStop();
                    state = SIDE_LINE_START;
                    break;
                }
                else if (backParallelLineFollower.isCentered(leftLineFollowBits, rightLineFollowBits) 
                    && leftLineFollowBits < 4 && rightLineFollowBits < 4) {

                    motors.motorsDrive(BACKWARD);
                }
                else {

                    if (!backParallelLineFollower.isCentered(leftLineFollowBits, rightLineFollowBits) && (leftLineFollowBits > rightLineFollowBits)) {
                        Serial.println("Correct to left");
                        motors.motorsTurnLeft();
                        motors.motorsDrive(BACKWARD);
                    }
                    if (!backParallelLineFollower.isCentered(leftLineFollowBits, rightLineFollowBits) && (leftLineFollowBits < rightLineFollowBits)) {
                        Serial.println("Correct to right");
                        motors.motorsTurnRight(); 
                        motors.motorsDrive(BACKWARD);
                    }
                }
            }
            else {

                if (digitalRead(BUMPER_SWITCH) != HIGH) {

                    motors.motorsStop();
                    state = SIDE_LINE_START;
                    break;
                }
                else {

                    motors.motorsDrive(BACKWARD);
                    break;
                }
            }
            break;

        case SIDE_LINE_START:
            Serial.println("side line starter"); 

            //Start aiming at the target
            digitalWrite(START_AIMING_PIN, HIGH);
            
            static bool fuzzyIntersected = false;

            //If we hit at least 6 lights
            if (frontParallelLineFollower.fuzzyIntersection(leftLineFollowBits, rightLineFollowBits)) {
                //Serial.println("fuzzy intersection has happened***************************");
                //fuzzyIntersected = true;
                motors.motorsStop();
                motors.motorsDrive(FORWARD);
                delay(500);
                state = FIRE;
                break;
            }
            //If we're centered, OR we've encountered the blue block 
            else if (frontParallelLineFollower.isCentered(leftLineFollowBits, rightLineFollowBits) 
                || (leftLineFollowBits >= 2 && rightLineFollowBits >= 2)) {

                motors.motorsDrive(FORWARD);
                break;
            }
            else {

                if (!frontParallelLineFollower.isCentered(leftLineFollowBits, rightLineFollowBits) && (leftLineFollowBits > rightLineFollowBits)) {
                    motors.motorsTurnLeft();
                }
                if (!frontParallelLineFollower.isCentered(leftLineFollowBits, rightLineFollowBits) && (leftLineFollowBits < rightLineFollowBits)) {
                    motors.motorsTurnRight(); 
                }
                break;
            }
        case FIRE:
            Serial.println("FIRE STATE");
            /*
            motors.motorsStop();
            //FIRE!---------------------------------------------------------------------------
            //Wait for beagle bone to aim

            startTime = millis();
            while (!fire && ((startTime + FIRE_WAIT_TIME) >= millis()));

            //Determine which barrel to fire based on what line we're on
                //lineCount is incremented everytime we COMPLETE a line
                //That is, when we leave a line and go back to the main line, it is incremented
            switch (lineCount) {

                // fire barrel 1
                case 0:
                    firing_servo_1.writeMicroseconds( SERVO_FIRE_POSITION );
                    delay( TRIGGER_DELAY );
                    firing_servo_1.writeMicroseconds( SERVO_REST_POSITION );

                    //switch off servo 1
                    pinMode(FIRING_SERVO_1_PIN, INPUT);
                    break;

                // fire barrel 2
                case 1:
                    firing_servo_2.writeMicroseconds( SERVO_FIRE_POSITION );
                    delay( TRIGGER_DELAY );
                    firing_servo_2.writeMicroseconds( SERVO_REST_POSITION );

                    //switch off servo 2
                    pinMode(FIRING_SERVO_2_PIN, INPUT);
                    break;

                // fire barrel 3
                case 2:
                    firing_servo_3.writeMicroseconds( DIFFERENT_SERVO_FIRE_POSITION );
                    delay( TRIGGER_DELAY );
                    firing_servo_3.writeMicroseconds( DIFFERENT_SERVO_REST_POSITION );

                    //switch off servo 3
                    pinMode(FIRING_SERVO_3_PIN, INPUT);
                    break;
            }

            //Stop aiming
            digitalWrite(START_AIMING_PIN, LOW);
            */

            //increment our count
            lineCount++;

            motors.motorsStop();
            delay(2000);
            state = GO_BACK;
            break;
            
        //Go back to the Main line-----------------------------------------------    
        case GO_BACK:
            Serial.println("go back state!");
            /*if (rightLineFollowBits == 0 && leftLineFollowBits == 0){

                Serial.println("turn left...lol");
                Serial.print("leftLineFollowBits == ");
                Serial.println(leftLineFollowBits);
                Serial.print("rightLineFollowBits == ");
                Serial.println(rightLineFollowBits);
                motors.motorsTurnLeft();
                break;
            }
            else */
            if (backParallelLineFollower.intersection(leftLineFollowBits, rightLineFollowBits)) {

                Serial.print("leftLineFollowBits == ");
                Serial.println(leftLineFollowBits);
                Serial.print("rightLineFollowBits == ");
                Serial.println(rightLineFollowBits);
                motors.motorsDrive(BACKWARD);
                delay(250);         
                motors.motorsStop();
                state = TURN_RIGHT_ONTO_MAIN_LINE;
                break;
            }
            else if(!backParallelLineFollower.intersection(leftLineFollowBits, rightLineFollowBits)) {
                Serial.println("NOT an intersection...fixing");
                if (backParallelLineFollower.isCentered(leftLineFollowBits, rightLineFollowBits)) {
                    motors.motorsDrive(BACKWARD);
                    break;
                }
                else {

                    if (!backParallelLineFollower.isCentered(leftLineFollowBits, rightLineFollowBits) 
                        && (leftLineFollowBits > rightLineFollowBits)) {

                        motors.motorsTurnLeft();
                    }
                    if (!backParallelLineFollower.isCentered(leftLineFollowBits, rightLineFollowBits) 
                        && (leftLineFollowBits < rightLineFollowBits)) {
                        
                        motors.motorsTurnRight(); 
                    }
                    break;
                }   
            }  
            else {
                Serial.print("leftLineFollowBits == ");
                Serial.println(leftLineFollowBits);
                Serial.print("rightLineFollowBits == ");
                Serial.println(rightLineFollowBits);
                motors.motorsDrive(BACKWARD);
                delay(250);         
                motors.motorsStop();
                state = TURN_RIGHT_ONTO_MAIN_LINE;
                break;
            }
        //
        case TURN_RIGHT_ONTO_MAIN_LINE:
            Serial.println("turn right onto main line state");
            do {
                motors.motorsDrive(BACKWARD);
                delay(500);
                motors.motorsTurnRight();
                delay(500);
                backParallelLineFollower.Get_Line_Data(leftLineFollowBits, rightLineFollowBits);
            } while (leftLineFollowBits || rightLineFollowBits);
            Serial.println("after first do while");
            do {
                motors.motorsTurnRight();
                backParallelLineFollower.Get_Line_Data(leftLineFollowBits, rightLineFollowBits);
            } while (leftLineFollowBits || rightLineFollowBits);
            Serial.println("after second do while");
            do {
                motors.motorsTurnRight();
                frontParallelLineFollower.Get_Line_Data(leftLineFollowBits, rightLineFollowBits);
            } while (!leftLineFollowBits && !rightLineFollowBits);
            Serial.println("after third do while");

            state = BACKUP_ON_MAIN_LINE;
            break;

        case BACKUP_ON_MAIN_LINE:
        {
            unsigned long initial_millis = millis();
            do {

                if (!frontParallelLineFollower.isCentered(leftLineFollowBits, rightLineFollowBits)) {
                 
                    if (leftLineFollowBits > rightLineFollowBits) {

                        motors.motorsTurnLeft();
                    }
                    if (leftLineFollowBits < rightLineFollowBits) {

                        motors.motorsTurnRight();
                    }
                }
                else {

                    motors.motorsDrive(BACKWARD);
                }

            } while((initial_millis + FORWARD_ON_MAIN_LINE_WAIT_TIME) > millis());

            initial_millis = millis();
            do {

                if (!backParallelLineFollower.isCentered(leftLineFollowBits, rightLineFollowBits)) {
                 
                    if (leftLineFollowBits > rightLineFollowBits) {

                        motors.motorsTurnLeft();
                    }
                    if (leftLineFollowBits < rightLineFollowBits) {

                        motors.motorsTurnRight();
                    }
                }
                else {

                    motors.motorsDrive(BACKWARD);
                }

            } while((initial_millis + BACKUP_ON_MAIN_LINE_WAIT_TIME) > millis());
            break;
        }
        /////////////////////////////////////////////////////////
        //State entered when button pushed while in loop
            //Reset's state to beginning
        case RESET:

            //delay so initial button push is not double read
            delay(2000);

            //Wait for button push again
            while(digitalRead(PIN_START) != HIGH);
            motors.motorsStop();
            state = START;

            //delay so initial button push is not double read
            delay(2000);
            break;
    }
    //*/
}