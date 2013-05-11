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
void loop() {
    sonarLeft.update();
    sonarRight.update();
    irLeft.updateIR();
    Serial.print("LEFTSONAR ");Serial.println(sonarLeft.getDistance());
    //Serial.print("LEFTIR ");Serial.println(irLeft.getIR());
    //float bin= 27.3382*pow((analogRead(LEFT_IR)),.4464);
        float volt = analogRead(LEFT_IR)*5/1024;
    float bin =  41.543 * pow((volt + 0.30221),-1.5281);

    //Serial.print("RIGHTIR ");Serial.println(analogRead(RIGHT_IR));
    //Serial.print("LEFTIR ");Serial.println(bin);

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
                curState = 200;
            }           
            if(pickUpBlocks(LARGE)){
                blocksPickedUp += 1;
                curState = 98;
            }
        break;
        case 98:
            if(move.turn90(LEFT))
                curState = 100;
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
                nextBay++;
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
                curState = 200;           
            if(pickUpBlocks(MED))
                curState++;            
        break;
        case 666:
          if(digitalRead(BUMPER_L) && digitalRead(BUMPER_R)){
  		curState = 99;
                delay(1000);
  	  }
        break;
    }

    //fsm_debug.update();
#endif
}

bool pickUpBlocks(bSize size){
    static side curClaw = LEFT;
    static int state = 0;
    switch(state){
    case 0:
        if(goToWall())
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
            }
            else{
                curClaw = LEFT;
                return true;
            }
            return true;
        }
    }
    return false;
}

bool checkIR(side s, bSize sizee){
    int frontPin;
    int backPin;
    if(s == LEFT){
        frontPin = RIGHT_FOR_IR;
        backPin = RIGHT_BACK_IR;
    }
    else {
        frontPin = 0;
        backPin = 0;
    }
    if(sizee == MED){
        if(analogRead(backPin)>IR_BLOCK_THRES)
            return true;
    }
    else if(sizee == LARGE){
        if(analogRead(frontPin)>IR_BLOCK_THRES)// && (analogRead(backPin)-150)>IR_BLOCK_THRES)
            return true;
    }
    return false;
}
