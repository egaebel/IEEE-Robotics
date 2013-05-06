#define TWI_FREQ 25000L
#include "FiniteStateMachine.h"
#include "States.h"
#include "States_Test.h"
#include <Servo.h>
#include <Wire.h>
#include "IRlib.h"
#include "common.h"
#include "RSonar.h"

#define DEBUG_FSM 1

extern State initState;
extern State moveToAirState;

FiniteStateMachine fsm(moveToAirState);

Sonar sonarLeft(SONAR_LEFT,SONAR_LEFT_INT);
RSonar sonarRight(1);

IRAverager leftIR(LEFT_IR);
IRAverager rightIR(RIGHT_IR);

Movement move;

cam leftCam = cam(LCAM_PIN);
cam rightCam = cam(RCAM_PIN);

void handleSonarLeft(){
  sonarLeft.setDataReady(true);
}

//void handleSonarRight(){
//  sonarRight.setDataReady(true);
//}

void setup() {
    delay(100); //Make sure we don't catch the PWM from POR of Sonar
    attachInterrupt(SONAR_LEFT_INT,handleSonarLeft,FALLING);
    //attachInterrupt(SONAR_RIGHT_INT,handleSonarRight,FALLING);

    Wire.begin();
    delay(200);
    move.init();

    //TODO:
    rightCam.init();

	Serial.begin(9600);
    Serial.println("SETUP COMPLETED!!");
}

int switchInt = 5;
void loop() {
    sonarLeft.update();
    sonarRight.update();
    //Serial.print("LEFT ");Serial.println(sonarLeft.getDistance());
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
        if(move.dropClaw(RIGHT))
            switchInt++;
        break;
        case 3:
        if(move.dropClaw(LEFT))
            switchInt++;
        break;
        case 4:
        if(move.turn90(RIGHT)){
            switchInt++;
        }
        break;
        case 5:
        if(digitalRead(53))
            switchInt = 123;
        break;
        case 6:
            if(move.liftUp())
                switchInt++;
        break;
        case 7:
            if(digitalRead(52))
                switchInt++;
        break;
        case 8:
            if(move.setDown())
                switchInt++;
        break;
        case 123:
            Serial.print("colour: ");Serial.println(rightCam.getBlockColour());
            switchInt = 5;
        break;
        case 666:
        break;
    }

    //fsm_debug.update();
#endif
}
