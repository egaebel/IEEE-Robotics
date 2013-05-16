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


#include "cam.h"

#define DEBUG_FSM 1

bool checkIR(side,bSize);
bool pickUpBlocks(bSize);
int getBayNum(bColour, bPosition);

#define USE_COLOUR 1


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
bColour TEST_COLOR = GREEN;
bColour RETURN_COLOR;
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
    rightCam.init();
    leftCam.init();
    delay(200);

    Serial.println("SETUP COMPLETED!!");
}
int blocksPickedUp = 0;
bool leftClawBlock = 0;
bool rightClawBlock = 0;
int nextBay = 0;
int curState = 0;
side curClaw;

bColour railBlocks[6] = {BLUE,RED,BROWN,YELLOW,GREEN,PURPLE};
bColour seaBlocks[6] = {PURPLE,GREEN,BROWN,YELLOW,BLUE,RED};

int railBlocksExist[6] = {0,0,0,0,0,0};
int seaBlocksExist[6] = {0,0,0,0,0,0};

bColour colRight, colLeft;
void loop() {
    sonarLeft.update();
    //sonarRight.update();
    irLeft.updateIR();
    irRight.updateIR();
    //Serial.print("LEFTSONAR ");Serial.println(sonarLeft.getDistance());
    //Serial.print("RIGHTIR ");Serial.println(irRight.getIR());
    //Serial.print("LEFTIR ");Serial.println(irLeft.getIR());

    //Serial.print("RIGHT ");Serial.println(sonarRight.getDistance());
    //Serial.print("BACK ");Serial.println(analogRead(RIGHT_BACK_IR));
    //Serial.print("FRONT ");Serial.println(analogRead(RIGHT_FOR_IR));
#if DEBUG_FSM == 0
    //Serial.print("STUFF\n");

    //rightCam.getTrackingData(TEST_COLOR);
    //leftCam.getTrackingData(TEST_COLOR);
    if (digitalRead(52) || digitalRead(53)) {
        RETURN_COLOR = leftCam.getBlockColour();
        leftCam.getTrackingData(RETURN_COLOR);
        Serial.print("RETURN COLOR IS::");
        Serial.print(RETURN_COLOR);
        Serial.print("\n");
    }
    //if (RETURN_COLOR == TEST_COLOR)
        //Serial.print("YOU DID IT!\n");
    //else
        //Serial.print("YOU PHAILED!\n");
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
                curState++;
            }
        break;
        case 97:
            if(move.turnAround())
            #if USE_COLOUR
                curState++;
            #else
                curState = 102;
            #endif
        break;

        case 98:
            if(goToBay(POS_RAIL,getBayNum(colLeft,POS_RAIL),LEFT)){
                curState++;
            }
        break;
        case 99:
            if(move.dropClaw(LEFT))
                curState = 104;
        break;

        case 102:
            if(goToBay(POS_RAIL, nextBay, LEFT)){
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
            if(move.turn90(LEFT)){
                #if USE_COLOUR
                    curState = 220;
                #else
                    curState++;
                #endif
            }
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

        case 220:
            if(goToBay(POS_SEA,getBayNum(colLeft,POS_SEA),LEFT)){
                curState++;
            }
        break;
        case 221:
            if(move.dropClaw(LEFT)){
                curState++;
            }
        break;
        case 222:
            if(goToBay(POS_SEA,getBayNum(colRight,POS_SEA),RIGHT)){
                curState++;
            }
        break;
        case 223:
            if(move.dropClaw(RIGHT)){
                curState =207;
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
            move.slideWall(RIGHT,.05);
            if(checkIR(curClaw,size)){
                state++;
            }
        }
    break;
    case 1:
        move.stop();
        #if USE_COLOUR
            if(curClaw==LEFT){
                //leftCam.setWindow(POS_PICK_UP);
                colLeft = leftCam.getBlockColour();
            }
            else{
                //rightCam.setWindow(POS_PICK_UP);
                colRight = rightCam.getBlockColour();
            }
        #endif
        state++;
    break;
    case 2:
        move.stop();
        if(move.pickupClaw(curClaw)){
            if(curClaw==LEFT){
                curClaw = RIGHT;
                state = 0;
                return true;
            }
            else{
                state = 0;
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

int getBayNum(bColour col,bPosition pos){
    bColour * ptr;
    int * ptrb;
    if(pos==POS_SEA){
        ptr = seaBlocks;
        ptrb = seaBlocksExist;
    }
    else{
        ptr = railBlocks;
        ptrb = railBlocksExist;
    }

    for(int i = 0;i<6;i++){
        if(ptr[i]==col){
            /*//egads theres a block already there! lets find an empty spot
            if(ptrb[i]==1){
                for(int j =0;j<6;j++){
                    if(ptrb[j]==0){
                        ptrb[j] = 1;
                        return j;
                    }
                }
            }
            else{*/
                ptrb[i] = 1;
                return i;
            //}
        }
        else if (col == BLUE) {

            return 0;
        }
        else if (col == RED) {

            return 1;
        }
        else if (col == BROWN) {

            return 2;
        }
        else if (col == YELLOW) {

            return 3;
        }
        else if (col == GREEN) {

            return 4;
        }
        else if (col == PURPLE) {

            return 5;
        }
        else {
            return 12;
        }
    }
    return 8;
}

