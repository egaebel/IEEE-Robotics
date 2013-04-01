#include "FiniteStateMachine.h"
#include "States.h"
#include <Servo.h>
#include <Wire.h>

extern State initState;
FiniteStateMachine fsm(initState);

void handleSonarLeft(){
  sonarLeft.setDataReady(true);
}

void handleSonarRight(){
  sonarRight.setDataReady(false);
}

void setup() {
        delay(100); //Make sure we don't catch the PWM from POR of Sonar
        attachInterrupt(SONAR_LEFT_INT,handleSonarLeft,FALLING);
        attachInterrupt(SONAR_RIGHT_INT,handleSonarRight,FALLING);
	//setup claws
	//lClaw.init(LCLAW_EXTEND_SERVO, LCLAW_SERVO);
	//rClaw.init(RCLAW_EXTEND_SERVO, RCLAW_SERVO);
	//fsm
	Serial.begin(9600);
}

void loop() {
    sonarLeft.update();
    sonarRight.update();
    fsm.update();
}
