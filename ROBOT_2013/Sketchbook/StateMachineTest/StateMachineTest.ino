#include "FiniteStateMachine.h"
#include "States.h"
//#include "common.h"
//#include "Movement.h"
//#include "Claw.h"
//#include "WallFollower.h"
//#include "LineSensor.h"
//#include <Servo.h>

//#include "BackEndColorSensor.h"
//BackEndColorSensor colorSensor;
extern State initState;

//Movement move;
//LineSensor line;
//WallFollower wallFollower;


FiniteStateMachine fsm(initState);

void setup() {
    Serial.begin(9600);
}

//go through the entire state machine
void loop() {
    fsm.update();
}