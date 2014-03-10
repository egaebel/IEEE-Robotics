/*HUB-ee BMD Arduino Lib
Provides an object for a single motor using the BMD motor driver, includes optional standby control.
Designed for the BMD or BMD-S motor driver which uses the Toshiba TB6593FNG motor driver IC
Created by Creative Robotics Ltd in 2012.
Released into the public domain.
*/
#include "Hubee.h"

Hubee::Hubee(){}

void Hubee::setup(int In1Pin, int In2Pin, int PWMPin, unsigned int motorPower)
{
    IN1 = In1Pin;
    IN2 = In2Pin;
    PWM = PWMPin;
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);

    this->motorPower = motorPower;
    movingForward = true;
}

void Hubee::stop() {
    analogWrite(PWM, 0);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH);
}

void Hubee::forward() {
    setMotor(HIGH);
    movingForward = true;
}

void Hubee::backward() {
    setMotor(LOW);
    movingForward = false;
}

//set the motor - public function
void Hubee::setMotorPower(int motorPower) {
    if (motorPower < 0) {
        movingForward = false;
    }
    else if (motorPower > 0) {
        movingForward = true;
    }
    //write the speed value to PWM
    analogWrite(PWM, motorPower);
}

void Hubee::setMotor(bool motorDirection) {

    //Serparate case for stopping motor
    if(motorPower == 0) {
        stop();
        return;
    }

    //write the speed value to PWM
    analogWrite(PWM, motorPower);

    //Forward
    if (motorDirection) {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
    }
    //Backward
    else {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
    }
}