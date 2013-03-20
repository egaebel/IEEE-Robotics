#include "FiniteStateMachine.h"
#include "States.h"
#include <Servo.h>

extern State initState;
FiniteStateMachine fsm(initState);

void setup() {
    Serial.begin(9600);
}

//go through the entire state machine
void loop() {
    fsm.update();
}