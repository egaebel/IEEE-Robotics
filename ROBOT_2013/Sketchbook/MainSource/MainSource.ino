#include "FiniteStateMachine.h"
#include "States.h"
#include "States_TEST.h"
#include <Servo.h>
#include <Wire.h>


#define DEBUG_FSM 1

Sonar sonarLeft(SONAR_LEFT,SONAR_LEFT_INT);
//Sonar sonarRight(SONAR_RIGHT,SONAR_RIGHT_INT);
Sonar sonarRight(SONAR_RIGHT);
extern State state1;
FiniteStateMachine fsm_debug(state1);


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
#if DEBUG_FSM ==0
    fsm.update();
#else
    fsm_debug.update();
#endif
}
