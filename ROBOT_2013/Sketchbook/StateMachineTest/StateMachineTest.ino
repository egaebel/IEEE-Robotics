#include <Arduino.h>
#include "FiniteStateMachine.h"
#include "States.h"
//#include "BackEndColorSensor.h"
extern State initState;
//FiniteStateMachine fsm(initState);
//BackEndColorSensor colorSensor;
Claw lClaw;
Claw rClaw;

void setup() {
    Serial.begin(9600);
    LineSensor line;
    FiniteStateMachine fsm(initState);
}

//go through the entire state machine
void loop() {
           
}