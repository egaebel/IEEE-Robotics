#include <Arduino.h>
#include "FiniteStateMachine.h"
#include "States.h"
#include "common.h"
//#include "BackEndColorSensor.h"
extern State initState;
//FiniteStateMachine fsm(initState);
//BackEndColorSensor colorSensor;
Claw lClaw;
Claw rClaw;

void setup() {
    Serial.begin(9600);
    FiniteStateMachine fsm(initState);
    LineSensor line;
    WallFollower wallFollower;
}

//go through the entire state machine
void loop() {
    fsm->update();
}