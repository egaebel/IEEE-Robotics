#include "FiniteStateMachine.h"

//List of states
State initState = State(NULL);
State moveToState = State(NULL);
State scanState = State(NULL);
State pickUpState = State(NULL);
State dropState = State(NULL);

//*****State Functions*****//
//initState Functions
void initEnter(FinteStateMachine& parent) {

    fsm = parent;
    //initialize necessary variables & sensors
}

void initUpdate() {

}

void initExit() {

}

//scanState
void scanEnter(FiniteStateMachine& parent) {

    fsm = parent;
    //initialize necessary variables & sensors
}

void scanUpdate() {
    
}

void scanExit() {

}

//moveToState
void moveToEnter(FiniteStateMachine& parent) {

    fsm = parent;
    //initialize necessary variables & sensors
}

void moveUpdate() {

}

void moveExit() {

}

