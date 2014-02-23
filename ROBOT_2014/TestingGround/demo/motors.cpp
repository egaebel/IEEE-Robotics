#include "motors.h"

//Sets up the pin modes and the default speed.
void Motors::setup(int defaultSpeed) {

    delay(2);

    //Set the PIN Modes
    pinMode(PIN_PWM_LEFT, OUTPUT);
    pinMode(PIN_DIRECTION_LEFT, OUTPUT);

    pinMode(PIN_PWM_RIGHT, OUTPUT);
    pinMode(PIN_DIRECTION_RIGHT, OUTPUT);

    speed = defaultSpeed;

    //Delay 2 milliseconds so we don't explode....
    delay(2);
}

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
        Serial.write("pinInput is HIGH!\n");
    }
    else {
        pinInput = LOW;
        Serial.write("pinInput is LOW!\n");
    }

    digitalWrite(PIN_DIRECTION_LEFT, pinInput);
    analogWrite(PIN_PWM_LEFT, speed);   

    digitalWrite(PIN_DIRECTION_RIGHT, pinInput);
    analogWrite(PIN_PWM_RIGHT, speed);

    //Delay 2 milliseconds so we don't explode....
    delay(2);
}

//Adjust the speed of each motor manually by passing a PWM 
void Motors::motorsTurn(short leftPWM, short rightPWM) {

    digitalWrite(PIN_DIRECTION_LEFT, LOW);
    digitalWrite(PIN_DIRECTION_RIGHT, LOW);
    analogWrite(PIN_PWM_LEFT, leftPWM);
    analogWrite(PIN_PWM_RIGHT, rightPWM);
}

//
void Motors::motorsUTurn() {

    
    motorsTurn(LEFT);
}

//Turns the robot left.
void Motors::motorsTurnLeft() {
    
    //Delay 2 milliseconds so we don't explode....
    delay(2);

    motorsTurn(LEFT);

    //Delay 2 milliseconds so we don't explode....
    delay(2);
}

//Turns the robot right.
void Motors::motorsTurnRight() {
    
    //Delay 2 milliseconds so we don't explode....
    delay(2);

    motorsTurn(RIGHT);

    //Delay 2 milliseconds so we don't explode....
    delay(2);
}

//Turns the motors in the specified direction (used to make the above methods work, not to be interfaced with)
void Motors::motorsTurn(Turn motorsTurn){

    //Delay 2 milliseconds so we don't explode....
    delay(2);

    bool pinInput;

    if (motorsTurn == LEFT) 
        pinInput = HIGH;
    else
        pinInput = LOW;

    digitalWrite(PIN_DIRECTION_LEFT, pinInput);
    analogWrite(PIN_PWM_LEFT, speed);   

    digitalWrite(PIN_DIRECTION_RIGHT, !pinInput);
    analogWrite(PIN_PWM_RIGHT, speed);   

    //Delay 2 milliseconds so we don't explode....
    delay(2);
}

//Cuts power to the motors, roll to a stop
void Motors::motorsStop() {

    //Delay 2 milliseconds so we don't explode....
    delay(2);

    analogWrite(PIN_PWM_LEFT, 0);  
    analogWrite(PIN_PWM_RIGHT, 0);  

    //Delay 2 milliseconds so we don't explode....
    delay(2);
}