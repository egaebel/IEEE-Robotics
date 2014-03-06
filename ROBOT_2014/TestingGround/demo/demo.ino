#include <pins.h>
#include <motors.h>
#include <linefollow.hpp>
#include <colorSensor.h>

enum State { MAIN_LINE = 1, TURN_LEFT_ONTO_SIDE, SIDE_LINE_START, FIRE, SIDE_LINE_END, TURN_LEFT_ONTO_MAIN_LINE, RESET };

//Hardware interfaces
static Motors motors;
static LineFollower lineFollower;
static ColorSensor cs;

//static j_state j_s = FOLLOW_STRAIGHT_LINE;
static State state = MAIN_LINE;
static byte leftLineFollowBits;
static byte rightLineFollowBits;
static int lineCount = 0;

//Setup variables etc
void setup() {

    Serial.begin(9600);
    Serial.println("IN the beginning...");
    
    //Initialize Hardware--------------------------------
    //Motor variables setup
    motors.setup(PIN_PWM_LEFT, PIN_DIRECTION_LEFT, PIN_PWM_RIGHT, PIN_DIRECTION_RIGHT, DEFAULT_SPEED);
    motors.motorsStop();

    //LineFollower variables setup
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV2);
    SPI.setDataMode(SPI_MODE3);
    SPI.setBitOrder(MSBFIRST);

    pinMode(PIN_LOAD, OUTPUT);
    digitalWrite(PIN_LOAD, HIGH);
    pinMode(PIN_SENSOR, OUTPUT);
    digitalWrite(PIN_SENSOR, HIGH); //activates LineFollower Kit
    lineFollower.setup(PIN_LOAD, PIN_SENSOR);

    //Color Sensor Setup
    //cs.setup(CS_S0, CS_S1, CS_S2, CS_S3, CS_OUT, CS_LED);
    delay(3000);
    Serial.println("waiting to go.....");
    //Loop until start
    while((digitalRead(PIN_START) != HIGH));
    delay(500);
    Serial.println("GO!");
}

//Loop
void loop() {

    //If we get a button press, stop
    if(digitalRead(PIN_START) == HIGH) {
        Serial.println("Stopped, state machine reset!");
        motors.motorsStop();
        state = RESET;
    }

    switch (state) {

        case MAIN_LINE:
            Serial.println("straight line");
            
            if (lineFollower.intersection(leftLineFollowBits, rightLineFollowBits)) {
                state = TURN_LEFT_ONTO_SIDE;
                break; 
            }
            else if (lineFollower.isCentered(leftLineFollowBits, rightLineFollowBits)) {
                Serial.println("forward");
                motors.motorsDrive(FORWARD);
                break;
            }
            else {

                Serial.println("isCentered?");
                Serial.print("Left bits == ");
                Serial.println(leftLineFollowBits);
                Serial.print("right bits == ");
                Serial.println(rightLineFollowBits);
                if (!lineFollower.isCentered(leftLineFollowBits, rightLineFollowBits) && (leftLineFollowBits > rightLineFollowBits)) {
                    motors.motorsTurnLeft();
                }

                if (!lineFollower.isCentered(leftLineFollowBits, rightLineFollowBits) && (leftLineFollowBits < rightLineFollowBits)) {
                    motors.motorsTurnRight(); 
                }
                break;
            }

         case TURN_LEFT_ONTO_SIDE:

            do {
                motors.motorsDrive(FORWARD);
                delay(500);
                motors.motorsTurnLeft();
                delay(500);
                lineFollower.Get_Line_Data(leftLineFollowBits, rightLineFollowBits);
            } while (leftLineFollowBits || rightLineFollowBits);
             
            do {
                motors.motorsTurnLeft();
                lineFollower.Get_Line_Data(leftLineFollowBits, rightLineFollowBits);
            } while (leftLineFollowBits || rightLineFollowBits);
             
            do {
                motors.motorsTurnLeft();
                lineFollower.Get_Line_Data(leftLineFollowBits, rightLineFollowBits);
            } while (!leftLineFollowBits && !rightLineFollowBits);
    
            //Backup to account for overturning
            motors.motorsDrive(BACKWARD);
            delay(250);

            state = SIDE_LINE_START;
            break;

        case SIDE_LINE_START:
            Serial.println("side line starter"); 
                        
            if (lineFollower.isCentered(leftLineFollowBits, rightLineFollowBits)) {
                motors.motorsDrive(FORWARD);
                //FOR DEMO ONLY--REMOVE WHEN COLOR SENSOR WORKS
                delay(2000);
                state = FIRE;
                ///////////////////////////////////////////////

                break;
            }
            else {

                if (!lineFollower.isCentered(leftLineFollowBits, rightLineFollowBits) && (leftLineFollowBits > rightLineFollowBits)) {
                    motors.motorsTurnLeft();
                }
                if (!lineFollower.isCentered(leftLineFollowBits, rightLineFollowBits) && (leftLineFollowBits < rightLineFollowBits)) {
                    motors.motorsTurnRight(); 
                }
                break;
            }
        case FIRE:
            Serial.println("FIRE STATE");

            do {
                motors.motorsTurnLeft();
                lineFollower.Get_Line_Data(leftLineFollowBits, rightLineFollowBits);
            } while (leftLineFollowBits || rightLineFollowBits);

            do {
                motors.motorsTurnLeft();
                lineFollower.Get_Line_Data(leftLineFollowBits, rightLineFollowBits);
            } while (!leftLineFollowBits && !rightLineFollowBits);

            state = SIDE_LINE_END;

            break;

        case SIDE_LINE_END:
            Serial.println("side line end state");
            
            if (lineFollower.intersection(leftLineFollowBits, rightLineFollowBits)) {
                state = TURN_LEFT_ONTO_MAIN_LINE; 
                break; 
            }
            else if (lineFollower.isCentered(leftLineFollowBits, rightLineFollowBits)) {
                motors.motorsDrive(FORWARD); 
                break;
            }
            else {

                if (!lineFollower.isCentered(leftLineFollowBits, rightLineFollowBits) && (leftLineFollowBits > rightLineFollowBits)) {
                    motors.motorsTurnLeft();
                }

                if (!lineFollower.isCentered(leftLineFollowBits, rightLineFollowBits) && (leftLineFollowBits < rightLineFollowBits)) {
                    motors.motorsTurnRight(); 
                }
                break;
            }

        case TURN_LEFT_ONTO_MAIN_LINE:
            
            do {
                motors.motorsDrive(FORWARD);
                delay(500);
                motors.motorsTurnLeft();
                delay(500);
                lineFollower.Get_Line_Data(leftLineFollowBits, rightLineFollowBits);
            } while (leftLineFollowBits || rightLineFollowBits);
             
            do {
                motors.motorsTurnLeft();
                lineFollower.Get_Line_Data(leftLineFollowBits, rightLineFollowBits);
            } while (leftLineFollowBits || rightLineFollowBits);
             
            do {
                motors.motorsTurnLeft();
                lineFollower.Get_Line_Data(leftLineFollowBits, rightLineFollowBits);
            } while (!leftLineFollowBits && !rightLineFollowBits);

            state = MAIN_LINE;
            break;

        case RESET:

            delay(2000);

            while(digitalRead(PIN_START) != HIGH);
            motors.motorsStop();
            state = MAIN_LINE;

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