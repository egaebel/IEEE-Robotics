#include "motors.h"

//Sets up the pin modes and the default speed.
void Motors::setup(unsigned short pwmPinLeft, unsigned short directionPinLeft, 
                unsigned short pwmPinRight, unsigned short directionPinRight, 
                unsigned int defaultSpeed) {

    delay(2);

    pinMode(PWM_LEFT, OUTPUT);
    pinMode(DIRECTION_LEFT, OUTPUT);

    pinMode(PWM_RIGHT, OUTPUT);
    pinMode(DIRECTION_RIGHT, OUTPUT);

    PWM_LEFT = pwmPinLeft;
    DIRECTION_LEFT = directionPinLeft;
    PWM_RIGHT = pwmPinRight;
    DIRECTION_RIGHT = directionPinRight;

    speed = defaultSpeed;
    flipDirection = false;

    delay(2);
}

//Sets up the pin modes and the default speed.
/*void Motors::setup(int defaultSpeed) {

    delay(2);

    //Set the PIN Modes
    pinMode(PWM_LEFT, OUTPUT);
    pinMode(DIRECTION_LEFT, OUTPUT);

    pinMode(PWM_RIGHT, OUTPUT);
    pinMode(DIRECTION_RIGHT, OUTPUT);

    speed = defaultSpeed;

    //Delay 2 milliseconds so we don't explode....
    delay(2);
}*/

/*
 * Drives the motors in the direction passed in. The Enum Direction has FORWARD, and BACKWARD as values.
 *
 */
void Motors::motorsDrive(Direction motorsDirection) {
    
    //Delay 2 milliseconds so we don't explode....
    delay(2);

    bool pinInput;
 
    if (motorsDirection == FORWARD) {
        pinInput = HIGH;
    }
    else {
        pinInput = LOW;
    }

    digitalWrite(DIRECTION_LEFT, pinInput);
    analogWrite(PWM_LEFT, speed);   

    digitalWrite(DIRECTION_RIGHT, !pinInput);
    analogWrite(PWM_RIGHT, speed);

    //Delay 2 milliseconds so we don't explode....
    delay(2);
}

//Adjust the speed of each motor manually by passing a PWM 
/*void Motors::motorsTurn(short leftPWM, short rightPWM, Turn motorsTurn) {

    delay(2);

    bool pinInput;

    if (motorsTurn == RIGHT) {
        pinInput = LOW;
    }
    else if (motorsTurn == LEFT) {
        pinInput = HIGH;
    }

    digitalWrite(DIRECTION_LEFT, pinInput);
    digitalWrite(DIRECTION_RIGHT, pinInput);
    analogWrite(PWM_LEFT, leftPWM);
    analogWrite(PWM_RIGHT, rightPWM);

    delay(2);
}*/

//
void Motors::motorsUTurn() {

    delay(2);

    this->motorsTurn(RIGHT);
    delay(3000);
    this->motorsStop();

    delay(2);
}

//Turns the robot left.
void Motors::motorsTurnLeft() {
    
    //Delay 2 milliseconds so we don't explode....
    delay(2);

    this->motorsTurn(LEFT);

    //Delay 2 milliseconds so we don't explode....
    delay(2);
}

//Turns the robot right.
void Motors::motorsTurnRight() {
    
    //Delay 2 milliseconds so we don't explode....
    delay(2);

    this->motorsTurn(RIGHT);

    //Delay 2 milliseconds so we don't explode....
    delay(2);
}

//Turns the motors in the specified direction (used to make the above methods work, not to be interfaced with)
void Motors::motorsTurn(Turn motorsTurn){
    //Delay 2 milliseconds so we don't explode....
    delay(2);

    bool pinInput;

    if (motorsTurn == RIGHT) {
        pinInput = LOW;
        //Serial.println("motorsTurnLeft");
    }
    else {
        pinInput = HIGH;
        //Serial.println("motorsTurnRight");
    }

    digitalWrite(DIRECTION_LEFT, pinInput);
    analogWrite(PWM_LEFT, speed);   

    digitalWrite(DIRECTION_RIGHT, pinInput);
    analogWrite(PWM_RIGHT, speed);   

    //Delay 2 milliseconds so we don't explode....
    delay(2);
}

//Cuts power to the motors, roll to a stop
void Motors::motorsStop() {

    //Delay 2 milliseconds so we don't explode....
    delay(2);

    analogWrite(PWM_LEFT, 0);  
    analogWrite(PWM_RIGHT, 0);  

    //Delay 2 milliseconds so we don't explode....
    delay(2);
}