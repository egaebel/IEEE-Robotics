#include "FiniteStateMachine.h"
#include "States.h"

Movement move;

int internalState; //a sub-state used in each state, state-ception
int *zone;
static int blockPos; //used to keep track where we are in a zone (ie 2 would be 3rd block in a loading zone)

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
            move.back();
            if(sensor.line()==WHITE){
                wallFollower.lower();
                internalState++;
            }
            break;
        case 1:
            move.foward();
            if(wallFollower.isTouching()){
                curPos = POS_START;
                nextPos = POS_SEA;
                fsm->transitionTo(moveToState)
            }
        break;
    }

}

void initExit() {

    //stop listening to sensors
    internalState = 0;
}

//scanState
void scanEnter() {
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
                move.slideLeft();
            }
            else {
                move.slideRight();
            }

            if(sensor.line != WHITE && sensor.line != BLACK){
                zone[blockPos] = sensor.line;
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
                move.slideLeft();
            }
            else {
                move.slideRight();
            }

            if(sensor.line == WHITE){
                internalState = 0:
            }
            break;

        case 2:
            //our nextPos and curPos should be handled for us
            fsm->transitionTo(moveToState);
            break;
    }
}

void scanExit() {

    //stop listening to sensors
    internalState = 0;
}

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
                move.slideRight();
                if(sensor.line() != WHITE && sensor.line() != BLACK){
                    internalState++;
                }
                break;
            case 1:
                move.slideRight();
                if(sensor.line() == WHITE){
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
                    }
                }
                break;
        }
    }
    //sea to start (2 in state diagram)
    else if (curPos == POS_SEA && nextPos == POS_START) {
        switch(internalState) {
            //go left until wall is close (sonar?)
            case 0:
                //TODO: use sonar code somehow
                internalState++;
                break;
            case 1:
                curPos = nextPos;
                nextPos = POS_RAIL;
                fsm->transitionTo(moveTo);
                break;
        }
    }
    //start to rail (3 in state diagram)
    else if(curPos == POS_START && nextPos == POS_RAIL){
        switch(internalState){
            
            //move back and right
            case 0:
                move.backward();
                move.right();
                internalState++;
                break;
            //turn
            case 1:
                move.turnLeft();
                internalState++;
                break;
            //and hit the wall
            case 1:
                move.forward();
                if(sensor.wall() == TRUE){
                    internalState++;
                }
                break;
            //slide left until we hit a color
            //don't care about white lines because the start position has white lines that will screw everything up
            case 2:
                move.slideLeft();
                if(sensor.line() != WHITE && sensor.line() != BLACK){
                    internalState++;
                }
                break;
            //go back to the first white line then change state
            case 3:
                move.slideRight();
                if(sensor.line() == WHITE){
                    curPos = nextPos;
                    nextPos = POS_PICK_UP;
                    fsm->transitionTo(scanState);
                }
                break;
        }
    }
    //RAIL to PICK_UP (4 in state diagram)
    else if(curPos == POS_RAIL && nextPos == POS_PICK_UP){
        switch(internalState){
            //turn
            case 0:
                move.turnAround();
                internalState++;
                break;
            // and hit the wall
            case 1:
                move.forward();
                if(sensor.wall() == TRUE){
                    internalState++;
                }
                break;
            //slide left until we hit a color
            case 2:
                move.slideLeft();
                if(sensor.line() != WHITE && sensor.line() != BLACK) {

                    if (/*the pickup position hasn't been filled*/) {

                        fsm->transitionTo(scanState);
                    }
                    else {
                        fsm->transitionTo(pickUpState);
                    }
                    curPos = nextPos;
                    nextPos = POS_RAIL;
                }
                break;
        }
    }
    //pickup to rail (5 in state diagram)
    else if(curPos == POS_PICK_UP && nextPos == POS_RAIL){
        
        switch(internalState) {

            //slide to end of pickup by ramp
            case 0:
                //TODO slide to the "end" of the PICK_UP
                break;
            case 1:
                move.turnAround();
                internalState++;
                break;
            case 2:
                move.forward();
                if(sensor.wall() == TRUE){
                    
                    if (/*RAIL ISN'T FULL*/) {
                        curPos = nextPos;
                        nextPos = POS_PICK_UP;
                        fsm->transitionTo(dropState);
                    }
                    else if (/*SEA ISN'T FULL*/) {
                        curPos = nextPos;
                        nextPos = POS_START;
                        fsm->transitionTo(moveTo);
                    }
                    else {
                        //TODO: Kickstart the air states
                    }
                }
                break;
        }
    }
    //rail to start (6 in state diagram)
    else if (curPos == POS_RAIL && nextPos == POS_START) {

        switch (internalState) {
            case 0:
            break;
            case 1:
            break;
            case 2:
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
                move.slideLeft();
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