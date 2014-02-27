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
        Serial.print("pinInput is HIGH!\n");
    }
    else {
        pinInput = LOW;
        Serial.print("pinInput is LOW!\n");
    }

    digitalWrite(PIN_DIRECTION_LEFT, pinInput);
    analogWrite(PIN_PWM_LEFT, speed);   

    digitalWrite(PIN_DIRECTION_RIGHT, !pinInput);
    analogWrite(PIN_PWM_RIGHT, speed);

    //Delay 2 milliseconds so we don't explode....
    delay(2);
}

//Adjust the speed of each motor manually by passing a PWM 
void Motors::motorsTurn(short leftPWM, short rightPWM, Turn motorsTurn) {

    delay(2);

    bool pinInput;

    if (motorsTurn == RIGHT) {
        pinInput = LOW;
    }
    else if (motorsTurn == LEFT) {
        pinInput = HIGH;
    }
    else {
        Serial.println("WTF....");
        pinInput1 = HIGH;
        pinInput2 = LOW;
    }

    digitalWrite(PIN_DIRECTION_LEFT, pinInput1);
    digitalWrite(PIN_DIRECTION_RIGHT, pinInput2);
    analogWrite(PIN_PWM_LEFT, leftPWM);
    analogWrite(PIN_PWM_RIGHT, rightPWM);

    delay(2);
}

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

    if (motorsTurn == RIGHT) 
        pinInput = LOW;
    else
        pinInput = HIGH;

    digitalWrite(PIN_DIRECTION_LEFT, pinInput);
    analogWrite(PIN_PWM_LEFT, speed);   

    digitalWrite(PIN_DIRECTION_RIGHT, pinInput);
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