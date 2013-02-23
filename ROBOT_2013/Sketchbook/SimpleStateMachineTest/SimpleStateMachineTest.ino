#include "FiniteStateMachine.h"
#include "States.h"

extern State state1;
FiniteStateMachine fsm(state1);

void setup() {
    Serial.begin(9600);
}

//go through the entire state machine
void loop() {
    fsm.update();
}