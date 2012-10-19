#include "FiniteStateMachine.h"
#include "States.h"

//State objects
State initState = State(initEnter, initUpdate, initExit);
State scanState = State(scanEnter, scanUpdate, scanExit);
State moveToState = State(moveToEnter, moveToUpdate, moveToExit);
State pickUpState = State(pickUpEnter, pickUpUpdate, pickUpExit);
State dropState = State(dropEnter, dropUpdate, dropExit);
//End State objects

//*****START State Functions*****//
//initState Functions
void initEnter() {

    //initialize necessary variables & sensors
}

void initUpdate() {
    
    //perform initialization actions
}

void initExit() {

    //stop listening to sensors
}

//scanState
void scanEnter() {

    //initialize necessary variables & sensors
}

void scanUpdate() {
    
    //Perform the scanning actions
}

void scanExit() {

    //stop listening to sensors
}

//moveToState
void moveToEnter() {

    //initialize necessary variables & sensors
}

void moveToUpdate() {

    //Perform the move actions
}

void moveToExit() {
    
    //stop listening to sensors
}

//pickUpState
void pickUpEnter() {

    //initialize necessary variables & sensors
}

void pickUpUpdate() {

    //Perform the drop actions
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