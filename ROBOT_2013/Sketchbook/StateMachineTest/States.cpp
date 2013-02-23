#include "FiniteStateMachine.h"
#include "States.h"

Movement move;

int internalState; //a sub-state used in each state, state-ception
int *zone;
static int blockPos; //used to keep track where we are in a zone (ie 2 would be 3rd block in a loading zone)
extern LineSensor sensor;

//*****START State Functions*****//
//initState Functions
void initEnter() {

    //initialize necessary variables & sensors
    internalState = 0;
    move.init();
}

void initUpdate() {
    //setup wall follower

    switch(internalState){
        case 0:
            move.back(0.25);
            if(sensor.line()==WHITE){
                wallFollower.lower();
                internalState++;
            }
            break;
        case 1:
            move.foward(0.25);
            if(wallFollower.isTouching()){
                curPos = POS_START;
                nextPos = POS_SEA;
                fsm->transitionTo(moveToState)
                fsm->init();
            }
        break;
    }

}

void initExit() {}

//scanState
void initScan() {
    internalState = 0;
    blockPos = 0;
    //change zone array based off the position
    if(curPos == POS_SEA){
        zone = seaZone;
    }
    else if (curPos == POS_RAIL) {
        zone = railZone;
    }
    else if (cur == POS_PICK_UP) {
        zone = loadingZone;
    }
}

void scanUpdate() {
    
    //Perform the scanning actions
    switch(internalState){
        //Move until hitting a color
        case 0:
            if (curPos == POS_PICK_UP) {
                move.slideLeft(0.25);
            }
            else {
                move.slideRight(0.25);
            }

            if(sensor.line() != WHITE && sensor.line() != BLACK){
                zone[blockPos] = sensor.line();
                blockPos++;
                if(blockPos>5){
                    //we are done lets moveTo the next place
                    internalState = 2;
                }
                else{
                    internalState = 1;
                }
            }
            break;
        //We already read this color, so just keep moving until white
        case 1:
            if (curPos == POS_PICK_UP) {
                move.slideLeft(0.25);
            }
            else {
                move.slideRight(0.25);
            }

            if(sensor.line == WHITE){
                internalState = 0:
            }
            break;

        case 2:
            //our nextPos and curPos should be handled for us
            fsm->transitionTo(moveToState);
            fsm->init();
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
            case 0:
                //move to line surrounding start area
                move.slideRight(0.25);
                if(line.detectRight()){
                    internalState++;
                }
                break;
            case 1:
                //move to line at start 
                move.slideRight(0.25);
                if(line.detectRight()){
                    internalState++;
                }
            case 2:
                move.slideRight(0.25);
                if(line.detectRight()){
                    if (isScanning) {
                        fsm->transitionTo(scanState); 
                        curPos = nextPos;
                        nextPos = POS_START;
                    }
                    else{
                        //INSERT LINE SENSOR CODE TO DETERMINE WHERE TO DROP
                        fsm->transitionTo(dropState);
                        curPos = nextPos;
                        nextPos = POS_START;
                        fsm->init();
                    }
                }
                break;
        }
    }
    //sea to start (2 in state diagram)
    else if (curPos == POS_SEA && nextPos == POS_START) {
        switch(internalState) {
            case 0:
                move.slideLeft(0.25);
                if (line.detectLeft()) {
                    internalState++;
                }
            break;
            case 1:
                //TODO: move backwards a certain amount (small amount)
                move.backward(0.25);
                internalState++;
            break;
            case 2:
                move.turnLeft(0.25);
                internalState++;
            break;
            case 3:
                move.forward(0.1);
                if (wallFollower.isTouching()) {
                    internalState++;
                }
            break;
            case 4:
                curPos = nextPos;
                nextPos = POS_RAIL;
            break;
        }
    }
    //start to rail (3 in state diagram)
    else if(curPos == POS_START && nextPos == POS_RAIL){
        switch(internalState){
            
            //move back and right
            case 0:
                move.backward(0.25);
                move.right(0.25);
                internalState++;
                break;
            //turn
            case 1:
                move.turnLeft(0.25);
                internalState++;
                break;
            //and hit the wall
            case 1:
                move.forward(0.1);
                if(wallFollower.isTouching()){
                    internalState++;
                }
                break;
            //slide left until we hit a color
            //don't care about white lines because the start position has white lines that will screw everything up
            case 2:
                move.slideLeft(0.25);
                if(sensor.line() != WHITE && sensor.line() != BLACK){
                    internalState++;
                }
                break;
            //go back to the first white line then change state
            case 3:
                move.slideRight(0.25);
                if(line.detectLeft()){
                    curPos = nextPos;
                    nextPos = POS_PICK_UP;
                    fsm->transitionTo(scanState);
                    fsm->init();
                }
                break;
        }
    }
    //RAIL to PICK_UP (4 in state diagram)
    else if(curPos == POS_RAIL && nextPos == POS_PICK_UP){
        
        switch(internalState){
            //turn
            case 0:
                move.turnAround(0.25);
                internalState++;
                break;
            // and hit the wall
            case 1:
                move.forward(0.1);
                if(wallFollower.isTouching()){
                    internalState++;
                }
                break;
            //slide left until we hit a color
            case 2:
                move.slideLeft(0.25);
                if(sensor.line() != WHITE && sensor.line() != BLACK) {

                    //loading area hasn't been scanned yet
                    if (loadingZone[0] == NULL) {

                        fsm->transitionTo(scanState);
                    }
                    else {
                        fsm->transitionTo(pickUpState);
                    }
                    curPos = nextPos;
                    nextPos = POS_RAIL;
                    fsm->init();
                }
                break;
        }
    }
    //pickup to rail (5 in state diagram)
    else if(curPos == POS_PICK_UP && nextPos == POS_RAIL){
        
        switch(internalState) {

            //slide to end of pickup by ramp
            case 0:
                move.slideRight(0.25);
                if (/*at last cell (rightmost if looking at stage with ramp in back) in pickup*/) {
                    internalState++;
                }
                break;
            case 1:
                move.turnAround(0.25);
                internalState++;
                break;
            case 2:
                move.forward(0.1);
                if(wallFollower.isTouching()){
                    
                    //if rail isn't full
                    if (!fullOfBlocks(railZone, RAIL_SEA_SIZE)) {
                        curPos = nextPos;
                        nextPos = POS_PICK_UP;
                        fsm->transitionTo(dropState);
                    }
                    //if sea isn't full
                    else if (!fullOfBlocks(seaZone, RAIL_SEA_SIZE)) {
                        curPos = nextPos;
                        nextPos = POS_START;
                        fsm->transitionTo(moveTo);
                    }
                    //if air isn't full
                    else {
                        //TODO: Kickstart the air states
                    }
                    fsm->init();
                }
                break;
        }
    }
    //rail to start (6 in state diagram)
    else if (curPos == POS_RAIL && nextPos == POS_START) {

        switch (internalState) {
            case 0:
                move.slideRight(0.25);
                if (line.detectRight()) {
                    internalState++;
                }
            break;
            case 1:
                //TODO: move backwards a certain amount (small amount)
                move.backward(0.2);
                internalState++;
            break;
            case 2:
                move.turnRight(0.25);
                internalState++;
            break;
            case 3:
                move.forward(0.1);
                if (wallFollower.isFollower()) {
                    internalState++;
                }
            break;
            case 4:
                curPos = nextPos;
                nextPos = POS_SEA;
            break;
        }
    }
    else{
        //shouldn't be here, maybe make another case?
        assert(0);
    }
}

void moveToExit() {}

//Handles centering the robot on a particular space, so we can properly pickup/drop the block
//mostly just reads the line array
void centerEnter() {

    //Figure out where you are in the pick up zone
    //initialize necessary variables & sensors
}

void centerUpdate() {

    //Perform the drop actions
}

void centerExit() {}

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

    internal state = 0;
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
                if (lBlock == colorSensor.detectColor()) {
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
                fsm->transitionTo(moveToState);
                nextPos = POS_PICK_UP;
                fsm->init();
                break;
        }
    }
    //we are at air
    else {
        //scan both spaces in air
        //....
        switch (internalState) {
            case 0:
                
        }
    }
}

void dropExit() {}
//*****END State Functions*****//

//State objects
//Put it here since it needs to know that the functions exist
State initState = State(initEnter, initUpdate, initExit);
State scanState = State(scanEnter, scanUpdate, scanExit);
State moveToState = State(moveToEnter, moveToUpdate, moveToExit);
State pickUpState = State(pickUpEnter, pickUpUpdate, pickUpExit);
State dropState = State(dropEnter, dropUpdate, dropExit);
State centerState = State(centerEnter, centerUpdate, centerExit);
//End State objects
