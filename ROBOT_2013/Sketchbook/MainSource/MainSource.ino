#include "FiniteStateMachine.h"
#include "States.h"
#include <Servo.h>

extern State initState;
FiniteStateMachine fsm(initState);

void setup() {
	//setup claws
	//lClaw.init(LCLAW_EXTEND_SERVO, LCLAW_SERVO);
	//rClaw.init(RCLAW_EXTEND_SERVO, RCLAW_SERVO);
	//fsm
	Serial.begin(9600);
}

void loop() {
    fsm.update();
}
