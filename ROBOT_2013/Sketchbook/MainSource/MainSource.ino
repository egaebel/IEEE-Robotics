#define TWI_FREQ 25000L
#include "FiniteStateMachine.h"
#include "States.h"
#include "States_Test.h"
#include <Servo.h>
#include <Wire.h>
#include "IRlib.h"
#include "common.h"

#define DEBUG_FSM 1

bool checkIR(side,bSize);
bool pickUpBlocks(bSize);


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
    Serial.begin(9600);
    Serial.println("serial started");
    delay(100); //Make sure we don't catch the PWM from POR of Sonar
    //attachInterrupt(SONAR_LEFT_INT,handleSonarLeft,FALLING);
    attachInterrupt(SONAR_RIGHT_INT,handleSonarRight,FALLING);

    Wire.begin();
    delay(200);
    move.init();

    //TODO:
    //rightCam.init();

    Serial.println("SETUP COMPLETED!!");
}
int blocksPickedUp = 0;
bool leftClawBlock = 0;
bool rightClawBlock = 0;
int nextBay = 0;
int curState = 666;
void loop() {
    sonarLeft.update();
    sonarRight.update();
    //Serial.print("LEFT ");Serial.println(sonarLeft.getDistance());
    //Serial.print("RIGHT ");Serial.println(sonarRight.getDistance());
    
    Serial.println(analogRead(RIGHT_FOR_IR));
#if DEBUG_FSM == 0
    fsm.update();
#else
    switch(curState){

        case 99:
            if(goToWall())
                curState++;
        break;

        case 100:
            if(blocksPickedUp == 6){
                blocksPickedUp = 0;
                curState = 200;
            }           
            if(pickUpBlocks(LARGE)){
                blocksPickedUp += 2;
                curState++;
            }
        break;
        case 101:
            if(move.turnAround(LEFT)){
                curState++;
            }
        break;
        case 102:
            if(goToBay(POS_RAIL, nextBay, RIGHT)){
                nextBay++;
                curState++;
            }
        break;
        case 103:
            if(move.dropClaw(RIGHT)){
                curState++;
            }
        break;

        case 104:
            if(goToBay(POS_RAIL, nextBay, LEFT)){
                nextBay++;
                curState++;
            }
        break;

        case 105:
            if(move.dropClaw(LEFT)){
                curState++;
            }
        break;

        case 106:
            if(move.turnAround(LEFT)){
                curState = 99;
            }
        break;

        case 200:
            if(blocksPickedUp == 6)
                curState = 200;           
            if(pickUpBlocks(LARGE))
                curState++;            
        break;
        case 666:
        break;
    }

    //fsm_debug.update();
#endif
}

bool pickUpBlocks(bSize size){
    static side curClaw = RIGHT;
    static int state = 0;
    switch(state){
    case 0:
        move.slideWall(RIGHT);
        if(checkIR(curClaw,size)){
            state++;
        }
    break;
    case 1:
        move.stop();
        if(move.pickupClaw(curClaw)){
            state = 0;
            if(curClaw==LEFT){
                curClaw==RIGHT;
                return true;
            }
            else
                curClaw = LEFT;
        }
    }
}

bool checkIR(side s, bSize sizee){
    int frontPin;
    int backPin;
    if(s == RIGHT){
        frontPin = RIGHT_FOR_IR;
        backPin = RIGHT_BACK_IR;
    }
    else {
        frontPin = 0;
        backPin = 0;
    }
    if(sizee == MED){
        if(analogRead(backPin)>0)
            return true;
    }
    else if(sizee == LARGE){
        if(analogRead(frontPin)>0 && analogRead(backPin)>0)
            return true;
    }
}