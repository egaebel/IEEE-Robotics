#include "FiniteStateMachine.h"
#include "States.h"

Movement move;

int internalState; //a sub-state used in each state, state-ception
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
    int zone[6];
    //change zone array based off the position
    if(curPos==POS_SEA_LOAD){

    }
}

void scanUpdate() {
    
    //Perform the scanning actions
    switch(internalState){
        //Move until hitting a color
        case 0:
            move.slideRight();
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
            move.slightRight();
            if(sensor.line == WHITE){
                internalState = 0:
            }
        //done
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
            //slide right until we hit a color
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
                    if (isScanning) {
                        curPos = nextPos;
                        nextPos = POS_PICK_UP_RIGHT; //after scanning, go to pick up
                        fsm->transitionTo(scanState); //start scanning

                    }
                    else{
                        curPos = nextPos;
                        nextPos = POS_PICK_UP_RIGHT; //after drop off, go to pick up
                        fsm->transitionTo(dropState); //start dropping
                    }
                }
                break;
        }
    }

    else if(curPos == POS_RAIL_LOAD && nextPos == POS_PICK_UP_RIGHT){
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
                }
                break;
        }
    }
    else if(curPos == POS_SEA_LOAD && nextPos == POS_PICK_UP){

    }
    else if(curPos == POS_PICK_UP && nextPos == POS_SEA_LOAD){

    }
    else if(curPos == POS_PICK_UP && nextPos == POS_RAIL_LOAD){

    }
    else{
        //shouldn't be here, maybe make another case?
        assert(0);
    }
}

void moveToExit() {
    
    //stop listening to sensors
}

//pickUpState
void pickUpEnter() {

    //initialize necessary variables & sensors
}

void pickUpUpdate() {

    //Perform the pick up actions
}

void pickUpExit() {
    
    //stop listening to sensors
}

//dropState
void dropEnter() {

    //initialize necessary variables & sensors
}

void dropUpdate() {

    //Perform the drop actions
}

void dropExit() {
    
    //stop listening to sensors
}
//*****END State Functions*****//

//State objects
//Put it here since it needs to know that the functions exist
State initState = State(initEnter, initUpdate, initExit);
State scanState = State(scanEnter, scanUpdate, scanExit);
State moveToState = State(moveToEnter, moveToUpdate, moveToExit);
State pickUpState = State(pickUpEnter, pickUpUpdate, pickUpExit);
State dropState = State(dropEnter, dropUpdate, dropExit);
//End State objects