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
                nextPos = POS_SEA_LOAD;
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
    if(curPos==POS_SEA_LOAD){
        zone = seaZone;
    }
    else if (curPos == POS_RAIL_LOAD) {
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
            move.slideLeft();
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
            move.slightLeft();
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

    //Start to sea load    
    if(curPos == POS_START && nextPos == POS_SEA_LOAD){
        switch(internalState){
            case 0:
                move.slideRight();
                if(sensor.line() != WHITE && sensor.line() != BLACK){
                    internalState++;
                }
                break;
            case 1:
                move.slideLeft();
                if(sensor.line() == WHITE){
                    if (isScanning) {
                        fsm->transitionTo(scanState); //start scanning
                        curPos = nextPos;
                        nextPos = POS_RAIL_LOAD; //we want to scan rail next
                    }
                    else{
                        fsm->transitionTo(dropState); //start dropping
                        curPos = nextPos;
                        nextPos = POS_PICK_UP; //after drop off, go to pick up
                    }
                }
                break;
        }
    }
    //Sea load to rail load
    else if(curPos == POS_SEA_LOAD && nextPos == POS_RAIL_LOAD){
        switch(internalState){
            //turn and hit the wall
            case 0:
                move.turnLeft();
                internalState++;
                break;

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
                    nextPos = POS_PICK_UP; //after scanning, go to pick up
                    fsm->transitionTo(scanState); //start scanning
                }
                break;
        }
    }

    else if(curPos == POS_RAIL_LOAD && nextPos == POS_PICK_UP){
        switch(internalState){
            //turn and hit the wall
            case 0:
                move.turnAround();
                internalState++;
                break;

            case 1:
                move.forward();
                if(sensor.wall() == TRUE){
                    internalState++;
                }
                break;
            //slide right until we hit a color
            //don't care about white lines because the start position has white lines that will screw everything up
            case 2:
                move.slideLeft();
                if(sensor.line() != WHITE && sensor.line() != BLACK){
                    internalState++;
                    fsm->transitionTo(pickUpState);
                    curPos = nextPos;
                    nextPos = POS_PICK_UP;
                }
                break;
        }
    }
    //sea to pickup
    else if(curPos == POS_SEA_LOAD && nextPos == POS_PICK_UP){
        
        switch (internalState) {
        
            case 0:
                move.turnRight();
                internalState++;
                break;
            case 1:
                move.forward();
                if(sensor.wall() == TRUE){
                    internalState++;
                }
                break;
            case 2:
                move.slideRight();
                if (sensor.line() == WHITE) {
                    fsm->transitionTo(pickUpState);
                    curPos = nextPos;
                    nextPos = POS_PICK_UP;
                }
                break;
        }
    }
    //pickup to sea
    else if(curPos == POS_PICK_UP && nextPos == POS_SEA_LOAD){

        switch(internalState) {

            case 0:
                move.turnLeft();
                internalState++;
                break;
            case 1:
                move.forward();
                if(sensor.wall() == TRUE){
                    fsm->transitionTo(dropState);
                    curPos = nextPos;
                    nextPos = POS_SEA_LOAD;
                }
                break;
        }
    }
    //pickup to rail
    else if(curPos == POS_PICK_UP && nextPos == POS_RAIL_LOAD){
        
        switch(internalState) {

            case 0:
                move.turnAround();
                internalState++;
                break;
            case 1:
                move.forward();
                if(sensor.wall() == TRUE){
                    fsm->transitionTo(dropState);
                    curPos = nextPos;
                    nextPos = POS_SEA_LOAD;
                }
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
                //drop left
                lClaw.extend();
                lClaw.drop();
                lClaw.retract();
                
                //drop right
                rClaw.extend();
                rClaw.drop();
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
