#include "FiniteStateMachine.h"
#include "States.h"
#include "States_TEST.h"
#include <Servo.h>


#define DEBUG_FSM 1
extern State state1;
FiniteStateMachine fsm_debug(state1);

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
#if DEBUG_FSM ==0
    fsm.update();
#else
	fsm_debug.update();
#endif
}
