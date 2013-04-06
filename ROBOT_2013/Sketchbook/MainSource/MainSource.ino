#define TWI_FREQ 25000L
#include "FiniteStateMachine.h"
#include "States.h"
#include "States_Test.h"
#include <Servo.h>
#include <Wire.h>
#include "IRlib.h"
#include "common.h"

#define DEBUG_FSM 0

extern State initState;
extern State moveToAirState;

FiniteStateMachine fsm(moveToAirState);

Sonar sonarLeft(SONAR_LEFT,SONAR_LEFT_INT);
Sonar sonarRight(SONAR_RIGHT,SONAR_RIGHT_INT);

IRAverager leftIR(LEFT_IR);
IRAverager rightIR(RIGHT_IR);

Movement move;

cam leftCam = cam(LCAM_PIN);
cam rightCam = cam(RCAM_PIN);

void handleSonarLeft(){
  sonarLeft.setDataReady(true);
}

void handleSonarRight(){
  sonarRight.setDataReady(true);
}

void setup() {
    delay(100); //Make sure we don't catch the PWM from POR of Sonar
    //attachInterrupt(SONAR_LEFT_INT,handleSonarLeft,FALLING);
    //attachInterrupt(SONAR_RIGHT_INT,handleSonarRight,FALLING);

    //Wire.begin();
    delay(200);
    move.init();
	Serial.begin(9600);
    Serial.println("SETUP COMPLETED!!");
	
}
void loop() {
    //sonarLeft.update();
    //sonarRight.update();
    Serial.println(sonarRight.getDistance());
#if DEBUG_FSM == 0
    fsm.update();
#else
    fsm_debug.update();
#endif
}
