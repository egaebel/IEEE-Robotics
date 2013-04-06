#define TWI_FREQ 25000L
#include "FiniteStateMachine.h"
#include "States.h"
#include "States_Test.h"
#include <Servo.h>
#include <Wire.h>
#include "IRlib.h"
#include "common.h"

#define DEBUG_FSM 1

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
    attachInterrupt(SONAR_LEFT_INT,handleSonarLeft,FALLING);
    attachInterrupt(SONAR_RIGHT_INT,handleSonarRight,FALLING);

    Wire.begin();
    delay(200);
    move.init();
	Serial.begin(9600);
    Serial.println("SETUP COMPLETED!!");
}

int switchInt = 0;
void loop() {
    sonarLeft.update();
    sonarRight.update();
    Serial.print("LEFT ");Serial.println(sonarLeft.getDistance());
    Serial.print("RIGHT ");Serial.println(sonarRight.getDistance());
    
    
#if DEBUG_FSM == 0
    fsm.update();
#else
    switch(switchInt){
        case 0:
        if(move.pickupClaw(RIGHT))
            switchInt++;
        break;
        case 1:
        if(move.pickupClaw(LEFT))
            switchInt++;
        break;
        case 2:
        break;
    }

    //fsm_debug.update();
#endif
}
