#include "States.h"

extern FiniteStateMachine fsm;

//Hardware classes
static Movement move;
static WallFollower wallFollower;
Claw rClaw;
Claw lClaw;
cam leftCam;
cam rightCam;

//positions used for the state machine
static POSITION curPos = POS_START;
static POSITION nextPos;

//Variables for blocks currently held
static Block lBlock; //Block held by the left claw.
static Block rBlock; //Block held by the right claw.

//array of pointers to Blocks
//for reference::
    //*Air is the northmost zone
    //*Sea is the westmost zone
// Blocks in the loading zone, listed west to east.
static Block *loadingZone[14];
//Sea zone colors, listed south to north.
static Block *seaZone[6]; 
//Rail zone colors, listed west to east. 
static Block *railZone[6]; 

static const int PICKUP_SIZE = 14;
static const int RAIL_SEA_SIZE = 6;
static const int AIR_SIZE = 2;

//used to keep track where we are in a zone 
    //(ie 2 would be 3rd block in a loading zone)
static int blockPos; 
//a sub-state used in each state, state-ception
static int internalState; 
//boolean used to indicate between states if we are scanning zones
static bool isScanning;

//State objects
//Put it here since it needs to know that the functions exist
State initState = State(initEnter, initUpdate, initExit);
State scanState = State(scanEnter, scanUpdate, scanExit);
State moveToState = State(moveToEnter, moveToUpdate, moveToExit);
State pickUpState = State(pickUpEnter, pickUpUpdate, pickUpExit);
State dropState = State(dropEnter, dropUpdate, dropExit);
//End State objects

//*****START State Functions*****//
//initState Functions
void initEnter() {

    //initialize necessary variables
    internalState = 0;
    move.init();
    isScanning = true;
}

void initUpdate() {
    //setup wall follower

    switch(internalState){
        case 0:
            move.backward(0.25);
            if(line.detectFront()){
                move.dropDown();
                internalState++;
            }
            break;
        case 1:
            move.forward(0.25);
            if(wallFollower.isTouching()){
                curPos = POS_START;
                nextPos = POS_SEA;
                fsm.transitionTo(moveToState);
                    
            }
            break;
    }
}

void initExit() {}

//scanState
void initScan() {
    internalState = 0;
    blockPos = 0;
}

void scanUpdate() {
    
    //Perform the scanning actions
    switch(internalState){
        //Move until hitting a color
        case 0:
            //scanning sea (state transition 2 in state diagram)
            if (curPos == POS_SEA) {

                move.slideRight(0.25);

                //focused on bay, read color
                if(rightCam.inZone()){
             
                    //put color read code here!
                    //--------------------------
                    //--------------------------

                    blockPos++;
                    if(blockPos > 5){
                        //we are done lets moveTo the next place
                        internalState = 2;
                    }
                    else{
                        internalState = 1;
                    }
                }
            }
            //scanning rail (state transition 6)
            else if (curPos == POS_RAIL) {

                move.slideLeft(0.25);

                //if we're focused on a bay, read color
                if (leftCam.inZone()) {

                    //put color read code here!
                    //--------------------------
                    //--------------------------
                    blockPos++;
                    if(blockPos > 5){
                        //we are done lets moveTo the next place
                        internalState = 2;
                    }
                    else{
                        internalState = 1;
                    }
                }
            }
            //scanning pickup (state transition 4)
            else {

                move.slideRight(0.25);
                //TODO: change to "onBlock" or something?
                if (rightCam.inZone()) {

                    //put color read code here!
                    //--------------------------
                    //--------------------------

                    blockPos++;
                    if(blockPos > 13){
                        //we are done lets moveTo the next place
                        internalState = 2;
                    }
                    else{
                        internalState = 1;
                    }
                }
            }

            break;
        //We already read this color, so just keep moving until white
        case 1:
            //move right
            if (curPos == POS_PICK_SEA || curPos == POS_PICK_UP) {
                move.slideRight(0.25);
                //TODO: CREATE inbetweenZones
                if(rightCam.inbetweenZones()){
                   internalState = 0:
                }
            }
            //move left
            else {
                move.slideLeft(0.25);
                //TODO: CREATE inbetweenZones
                if(leftCam.inbetweenZones()){
                   internalState = 0:
                }
            }
            break;
        case 2:
            //nextPos and curPos transitions were handled in most recent moveTo call
            fsm.transitionTo(moveToState);
            fsm.init();
            break;
    }
}

void scanExit() {}

//moveToState
void moveToEnter() {

    //initialize necessary variables & sensors
    internalState = 0;
}

//Handles all the movement, set curPos and nextPos before entering
//Seems messy to have seperate statements for each move, but allows us to tweak each move in case of issues
void moveToUpdate() {

    //Start to sea (1 in state diagram) 
    if(curPos == POS_START && nextPos == POS_SEA){
        switch(internalState){
            //move to first bay in sea
            case 0:
                move.slideRight(0.25);
                if(rightCam.inZone()){
                    internalState++;
                }
                break;
            //transition to scanning
            case 1:
                fsm.transitionTo(scanState); 
                curPos = nextPos;
                nextPos = POS_PICK_UP;
                break;
        }
    }
    //sea to pickup (scanning) (3 in the state diagram)
    else if (isScanning && curPos == POS_SEA && nextPos == POS_PICK_UP) {
        switch(internalState) {
            //internal states 0-2 backup, turn, and move to the wall
            case 0:
                //TODO: figure out the speed needed, or get stopping condition
                move.backward(0.1);
                internalState++;
            break;
            case 1:
                //TODO: figure out the speed needed, or get stopping condition
                move.turnRight(0.1);
                internalState++;
            break;
            case 2:
                move.forward(0.1);
                if (wallFollower.isTouching())
                    internalState++;
            break;
            case 3:
                move.slideRight(0.1);
                if (rightCam.inZone())
                    internalState++;
            break;
            case 4:
                curPos = nextPos;
                nextPos = POS_RAIL;
                fsm->transitionTo(scanState);
            break;
        }
    }
    //pickup to rail (scanning) (transition 5 in the state diagram)
    else if(isScanning && curPos == POS_PICK_UP && nextPos == POS_RAIL){

        switch(internalState){    
            //backup from wall
            case 0:
                move.backward(0.25);
                internalState++;
                break;
            //turn around
            case 1:
                move.turnAround();
                internalState++;
                break;
            //and hit the wall
            case 2:
                move.forward(0.1);
                if(wallFollower.isTouching())
                    internalState++;
                break;
            //slide Right until we are past the first bay
            case 3:
                move.slideRight(0.25);
                //TODO: MAKE "IFBLACK" FUNCTION
                if(!rightCam.inZone())
                    internalState++;
                break;
            //slide left until we are IN the first bay
            case 4: 
                move.slideLeft(0.25);
                if (leftCam.inZone())
                    internalState++;
                break;
            case 5:
                //We are done scanning after this last scan!
                isScanning = false;
                curPos = nextPos;
                nextPos = POS_PICK_UP;
                fsm->transitionTo(scanState);
                break;
        }
    }
    //pickup to rail (16 & 22 in state diagram)
    else if(curPos == POS_PICK_UP && nextPos == POS_RAIL){
        
        switch(internalState) {
            //backup 
            case 0:
                move.backward(0.1);
                internalState++;
                break;
            case 1:
                move.turnAround();
                internalState++;
                break;
            case 2:
                move.forward(0.1);
                if(wallFollower.isTouching())
                    internalState++;
                break;
            //figure out whether to go left or right based on location in rail
            case 3:

                //in the middle of rail zone
                //if () {}
                    //internalState++;
                //to the right of the rail zone
                //else {}
                    //internalState += 2;

            //move left until in first rail bay
            case 4:
                move.slideLeft(0.25);
                if (leftCam.inZone())
                    internalState += 2;
                break;
            //move right until in first rail bay
            case 5:
                move.slideRight(0.25);
                if (rightCam.inZone() && blockPos == 0)
                    internalState++;
                break;
            case 6:
                //if rail isn't full
                if (!fullOfBlocks(railZone, RAIL_SEA_SIZE)) {
                    curPos = nextPos;
                    nextPos = POS_PICK_UP;
                    fsm.transitionTo(dropState);
                }
                //if air isn't full (which it won't be)
                else {
                    //TODO: Kickstart the air states
                    //curPos = nextPos;
                    //nextPos = POS_AIR;
                    //fsm->transitionTo(moveToState);
                }
                fsm->init();
                break;
        }
    }
    else{
        //shouldn't be here, maybe make another case?
        //assert(0);
    }
}

void moveToExit() {}

//pickUpState
void pickUpEnter() {

    //Figure out where you are in the pick up zone
    //initialize necessary variables & sensors
    internalState = 0;
}

void pickUpUpdate() {

    //Perform the pick up actions
}

void pickUpExit() {}

//dropState
void dropEnter() {

    internalState = 0;
    //Figure out where you are in the pick up zone
    //initialize necessary variables & sensors
}

void dropUpdate() {
    
    //we aren't at air (phew!)
    if (curPos != POS_AIR) {    
        //scan and move until the second color is encountered (main case)
        switch (internalState) {
            case 0:
                move.slideLeft(0.25);
                if (lBlock.color == colorSensor.detectColor()) {
                    internalState++;            
                }
                break;
            case 1:
                //drop blocks
                lClaw.extend();
                rClaw.extend();

                lClaw.drop();
                rClaw.drop();

                lClaw.retract();
                rClaw.retract();

                internalState++;
                break;
            case 2:
                //transition to move
                fsm.transitionTo(moveToState);
                nextPos = POS_PICK_UP;
                break;
        }
    }
    //we are at air
    else {
        //scan both spaces in air
        //....
        /*switch (internalState) {
            case 0:
        */      
        }
    }
}

bool centered(&cam theCam)
{
    if(theCam.locateZone() > 0)
    {
        move.slideRight(0.1);
    }
    else
    {
        move.slideLeft(0.1);
    }
    if (theCam.inZone())
    {
        move.stop();
        return true;
    }
    return false;
}

void dropExit() {}
//*****END State Functions*****//