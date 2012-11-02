#include "FiniteStateMachine.h"
#include "States.h"

Movement move;
/**
 * int used to keep track of which substate we are on in a state
 * 
  * This int is used in init, scan, 
  * This int is NOT used in move, 
  */
static int internalState;

//*****START State Functions*****//
//initState Functions
void initEnter() {

    //initialize necessary variables & sensors
    internalState = 0;
    move.init();
}

//Uses intern
void initUpdate() {
    //setup wall follower

    /* psuedo code
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
    */
}

void initExit() {

    //stop listening to sensors
    internalState = 0;
}

//scanState
void scanEnter() {

    //initialize necessary variables & sensors
    internalState = 0;
}

void scanUpdate() {
    
    //Perform the scanning actions
    
    //if on the last spot  at the pickup
    if (curPos == POS_PICKUP && internalState == PICKUP_SIZE) {
        
        //Move to pickup the first block as determined by rail or sea spaces that are open
        internalState = 0;
    }
    //if on the last spot at the sea or rail
    else if ((curPos == POS_SEA_LOAD || curPos == POS_RAIL_LOAD) 
        && internalState == MAIN_DROPOFF_SIZE) {
        
        fsm->transitionTo(moveToState);
        toState = POS_PICK_UP
    }
    //read color
    else if ((subState % 2) == 0) {
    
        //save color to array
        internalState++;
    }
    //move until read white (again)
    else {
        
        //move to next white
        internalState++;
    }
}

void scanExit() {

    //stop listening to sensors
    internalState = 0;
}

//moveToState
void moveToEnter() {

    //initialize necessary variables & sensors
}

//Does not use internalState
void moveToUpdate() {

    //Perform the move actions
    /*
        if (curPos is to the left of nextPos) {
            move.slideRight()
        }
        else if (curPos is to the right of nextPos) {
            move.slideLeft();
        }
        else if (curPos is in front of nextPos) {
            move.forward();
        }
        else if (curPos is behind nextPos) {
            move.turnAround();
            move.forward()
        }
        
        if (//wall is hit) {
            if (isScanning) {
                fsm->transitionTo(scanState);
                curPos = nextPos;
            }
            else {
                if (curPos is equivalent to PICKUP) {
                    fsm->transitionTo(pickUpState);
                }
                else {
                    fsm->transitionTo(dropOffState);
                }
                curPos = nextPos;
                nextPos = NULL;
            }
        }
    */
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