#include "pins.h"
#include <motors.h>
#include <linefollow.hpp>
#include <linefollow_2.hpp>
#include <colorSensor.h>
#include <Servo.h>

enum State { START = 1, MAIN_LINE, TURN_LEFT_ONTO_SIDE, SIDE_LINE_START, FIRE, 
            GO_BACK, IGNORE_SIDE_LINE, TURN_RIGHT_ONTO_MAIN_LINE, RESET };

//Hardware interfaces
static Motors motors;
static LineFollower lineFollower;
static ParallelLineFollower frontParallelLineFollower;
static ParallelLineFollower backParallelLineFollower;
static ColorSensor colorSensor;

//Firing variables--------------------
//Different Servo Positions
const int SERVO_FIRE_POSITION = 544;
const int SERVO_REST_POSITION = 2400;
//Servo Delays
const int TRIGGER_DELAY = 1;
const int NOTIFY_DELAY = 1;
// The three firing barrel servos
static Servo firing_servo_1, firing_servo_2, firing_servo_3;
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
    Serial.println(HIGH);
    
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
    /*
    firing_servo_1.attach( FIRING_SERVO_1_PIN );
    firing_servo_1.writeMicroseconds( SERVO_REST_POSITION );
    firing_servo_2.attach( FIRING_SERVO_2_PIN );
    firing_servo_2.writeMicroseconds( SERVO_REST_POSITION );
    firing_servo_3.attach( FIRING_SERVO_3_PIN );
    firing_servo_3.writeMicroseconds( SERVO_REST_POSITION );

    pinMode( READY_TO_FIRE_PIN, INPUT_PULLUP );
    // when this pin is pulled low, the servo will fire
    attachInterrupt( READY_TO_FIRE_INTERRUPT, fire_handler, CHANGE );
    pinMode( AIM_NEXT_BARREL_PIN, OUTPUT );
    //*///----------------------------------------------------------
  
    delay(3000);

    Serial.println("waiting to go.....");

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

    switch (state) {

        case START:
            Serial.println("START");

            if (colorSensor.getColor() == GREEN || digitalRead(PIN_START) == HIGH) {
                motors.motorsDrive(FORWARD);
                delay(1000);
                state = MAIN_LINE;
            }
            break;

        case MAIN_LINE:
            Serial.println("straight line");
            
            if (frontParallelLineFollower.intersection(leftLineFollowBits, rightLineFollowBits)) {

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
                frontParallelLineFollower.Get_Line_Data(leftLineFollowBits, rightLineFollowBits);
            } while (leftLineFollowBits || rightLineFollowBits);
             
            do {
                motors.motorsTurnLeft();
                frontParallelLineFollower.Get_Line_Data(leftLineFollowBits, rightLineFollowBits);
            } while (!leftLineFollowBits && !rightLineFollowBits);
    
            //Backup to account for overturning
            motors.motorsDrive(BACKWARD);
            delay(750);

            state = SIDE_LINE_START;
            break;

        case SIDE_LINE_START:
            Serial.println("side line starter"); 
            
            static bool fuzzyIntersected = false;

            //If we hit at least 6 lights
            if (frontParallelLineFollower.fuzzyIntersection(leftLineFollowBits, rightLineFollowBits)) {
                Serial.println("fuzzy intersection has happened***************************");
                fuzzyIntersected = true;
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
            //FIRE!---------------------------------------------------------------------------
            //Wait for beagle bone to aim
            /*
            while (!fire);

            //Determine which barrel to fire based on what line we're on
                //lineCount is incremented everytime we COMPLETE a line
                //That is, when we leave a line and go back to the main line, it is incremented
            switch (lineCount) {

                // fire barrel 1
                case 0:
                    firing_servo_1.writeMicroseconds( SERVO_FIRE_POSITION );
                    delay( TRIGGER_DELAY );
                    firing_servo_1.writeMicroseconds( SERVO_REST_POSITION );
                    break;

                // fire barrel 2
                case 1:
                    firing_servo_2.writeMicroseconds( SERVO_FIRE_POSITION );
                    delay( TRIGGER_DELAY );
                    firing_servo_2.writeMicroseconds( SERVO_REST_POSITION );
                    break;

                // fire barrel 3
                case 2:
                    firing_servo_3.writeMicroseconds( SERVO_FIRE_POSITION );
                    delay( TRIGGER_DELAY );
                    firing_servo_1.writeMicroseconds( SERVO_REST_POSITION );
                    break;
            }

            // notify the BeagleBone to start aiming at the target for the next shot
            digitalWrite( AIM_NEXT_BARREL_PIN, HIGH );
            delay( NOTIFY_DELAY );
            digitalWrite( AIM_NEXT_BARREL_PIN, LOW );
            //*///--------------------------------------------------------------------------------

            motors.motorsStop();
            delay(2000);

            state = GO_BACK;
            break;
            
        //Go back to the Main line-----------------------------------------------    
        case GO_BACK:
            Serial.println("go back state!");
            if(!backParallelLineFollower.intersection(leftLineFollowBits, rightLineFollowBits))
            {
                if (backParallelLineFollower.isCentered(leftLineFollowBits, rightLineFollowBits) )
                {
                    motors.motorsDrive(BACKWARD);
                    break;
                }
                else 
                {
                    if (!backParallelLineFollower.isCentered(leftLineFollowBits, rightLineFollowBits) && (leftLineFollowBits > rightLineFollowBits)) 
                    {
                        motors.motorsTurnLeft();
                    }
                    if (!backParallelLineFollower.isCentered(leftLineFollowBits, rightLineFollowBits) && (leftLineFollowBits < rightLineFollowBits)) 
                    {
                        motors.motorsTurnRight(); 
                    }
                    break;
                }   
            }           

            state = TURN_RIGHT_ONTO_MAIN_LINE;
            break;

        //
        case TURN_RIGHT_ONTO_MAIN_LINE:
            Serial.println("turn right onto main line state");
            do {
                motors.motorsDrive(BACKWARD);
                delay(500);
                if (leftLineFollowBits || rightLineFollowBits) {
                    break;
                }
                motors.motorsTurnRight();
                delay(500);
                backParallelLineFollower.Get_Line_Data(leftLineFollowBits, rightLineFollowBits);
            } while (leftLineFollowBits || rightLineFollowBits);
            Serial.println("");
            do {
                motors.motorsTurnRight();
                backParallelLineFollower.Get_Line_Data(leftLineFollowBits, rightLineFollowBits);
            } while (leftLineFollowBits || rightLineFollowBits);
            Serial.println("");
            do {
                motors.motorsTurnRight();
                backParallelLineFollower.Get_Line_Data(leftLineFollowBits, rightLineFollowBits);
            } while (!leftLineFollowBits && !rightLineFollowBits);
            Serial.println("");
            state = MAIN_LINE;
            break;

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

/*
void followTheLine(byte leftBits, byte rightBits)
{
    short leftPWM = 0;
    short rightPWM = 0;
    Turn turnDirection;

    if (leftBits >= rightBits)
    {
        Serial.println("going left....");
        leftPWM = motors.speed / leftBits;
        rightPWM = motors.speed;
                motors.motorsTurnLeft();
    }
    else if (rightBits > leftBits)
    {
        Serial.println("going right....");
        leftPWM = motors.speed;
        rightPWM = motors.speed / rightBits;
                motors.motorsTurnRight();
    }
    else
    {
        leftPWM = motors.speed;
        rightPWM = motors.speed;
                motors.motorsDrive(FORWARD);
    }

    return;
}
*/