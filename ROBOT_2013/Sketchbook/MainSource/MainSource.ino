#define TWI_FREQ 25000L
#include "FiniteStateMachine.h"
#include "States.h"
#include "States_Test.h"
#include <Servo.h>
#include <Wire.h>
#include "IRlib.h"
#include "common.h"
#include "RSonar.h"
#include "math.h"
#define DEBUG_FSM 1

bool checkIR(side,bSize);
bool pickUpBlocks(bSize);


extern State initState;
extern State moveToAirState;

FiniteStateMachine fsm(moveToAirState);

Sonar sonarLeft(SONAR_LEFT,SONAR_LEFT_INT);
RSonar sonarRight(1);

IRAverager irLeft(LEFT_IR);
IRAverager irRight(RIGHT_IR);

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
    Serial.begin(9600);
    Serial.println("serial started");
    delay(100); //Make sure we don't catch the PWM from POR of Sonar
    attachInterrupt(SONAR_LEFT_INT,handleSonarLeft,FALLING);
    //attachInterrupt(SONAR_RIGHT_INT,handleSonarRight,FALLING);

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
int curState = 0;
side curClaw;


void loop() {
    sonarLeft.update();
    sonarRight.update();
    irLeft.updateIR();
    irRight.updateIR();
    Serial.print("LEFTSONAR ");Serial.println(sonarLeft.getDistance());
    Serial.print("RIGHTIR ");Serial.println(irRight.getIR());
    Serial.print("LEFTIR ");Serial.println(irLeft.getIR());

    //Serial.print("RIGHT ");Serial.println(sonarRight.getDistance());
    //Serial.print("BACK ");Serial.println(analogRead(RIGHT_BACK_IR));
    //Serial.print("FRONT ");Serial.println(analogRead(RIGHT_FOR_IR));
#if DEBUG_FSM == 0
    fsm.update();
#else
    switch(curState){
        case 0:
            if(digitalRead(BUMPER_L)&&digitalRead(BUMPER_R)){
                curState = 95;
            }
            break;
        //case 98:
            //if(goToWall())
            //    curState++;
        //break;
        case 95:
            if(goToBay(POS_PICK_UP, 0, RIGHT))
                curState++;
            //skip this
            curState++;
            break;
        case 96:
            if(blocksPickedUp == 6){
                blocksPickedUp = 0;
                nextBay = 0;
                curState = 200;
            }
            else
                curClaw = LEFT;
             //force it to skip the right claw           
            if(pickUpBlocks(LARGE)){
                blocksPickedUp += 1;
                if(sonarLeft.getDistance()>80)
                    curState++;
                else
                    curState = 98;
            }
        break;
        case 97:
            if(move.turnAround())
                curState = 102;
        break;
        case 98:
            if(move.turn90(LEFT))
                curState++;
        break;
        case 99:
            if(goToBay(POS_SEA,0,RIGHT))
                curState++;
        break;
        case 100:
            if(goToWall())
                curState++;
        break;
        case 101:
            if(move.turn90(LEFT)){
                curState++;
            }
        break;
        case 102:
            if(goToBay(POS_RAIL, nextBay, RIGHT)){
                curState++;
            }
        break;
        case 103:
            if(move.dropClaw(LEFT)){
                nextBay++;
                curState++;
            }
        break;

        case 104:
            if(goToBay(POS_RAIL,7,RIGHT))
                curState++;
        break;

        case 105:
            if(move.turn90(RIGHT))
                curState++;
        break;

        case 106:
            if(goToWall()){
                curState++;
            }
        break;

        case 107:
            if(move.turn90(RIGHT)){
                curState = 96; 
            }
        break;
        case 200:
            if(blocksPickedUp == 6)
                curState = 666;           
            if(pickUpBlocks(MED)){
                blocksPickedUp += 2;
                curState = 201;   
            }         
        break;
        case 201:
            if(goToBay(POS_PICK_UP,0,LEFT))
                curState++;
        break;
        case 202:
            if(move.turn90(LEFT))
                curState++;
        break;
        case 203:
            if(goToBay(POS_SEA,nextBay,LEFT))
                curState++;
        break;
        case 204:
            if(move.dropClaw(LEFT)){
                curState++;
                nextBay++;
            }
        break;
        case 205:
            if(goToBay(POS_SEA,nextBay,RIGHT))
                curState++;
        break;
        case 206:
            if(move.dropClaw(RIGHT)){
                curState++;
                nextBay++;
            }
        break;
        case 207:
            if(goToBay(POS_SEA,2,RIGHT)){
                curState++;
            }
        break;
        case 208:
            if(move.turn90(RIGHT)){
                curState = 200;
            }
        break;
        case 666:
            move.stop();
        break;
    }

    //fsm_debug.update();
#endif
}

bool pickUpBlocks(bSize size){
    static int state = 0;
    switch(state){
    case 0:
        if(goToWall()){
            move.slideWall(RIGHT);
            if(checkIR(curClaw,size)){
                state++;
            }
        }
    break;
    case 1:
        move.stop();
        if(move.pickupClaw(curClaw)){
            state = 0;
            if(curClaw==LEFT){
                curClaw = RIGHT;
                return true;
            }
            else{
                curClaw = LEFT;
            }
        }
    }
    return false;
}

bool checkIR(side s, bSize sizee){
    int frontPin;
    int backPin;
    if(s == LEFT){
        frontPin = LEFT_FOR_IR;
        backPin = LEFT_BACK_IR;
    }
    else {
        frontPin = RIGHT_FOR_IR;
        backPin = RIGHT_BACK_IR;
    }
    if(sizee == MED){
        if(analogRead(backPin)>IR_BLOCK_THRES+150)
            return true;
    }
    else if(sizee == LARGE){
        if(analogRead(frontPin)>IR_BLOCK_THRES)// && (analogRead(backPin)-150)>IR_BLOCK_THRES)
            return true;
    }
    return false;
}