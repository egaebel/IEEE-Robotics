#include <Arduino.h>
#include "FiniteStateMachine.h"
using namespace std;

//Prototypes
void state1Enter();
void state1Update();
void state1Exit();

void state2Enter();
void state2Update();
void state2Exit();
//

State state1 = State(state1Enter, state1Update, state1Exit);
State state2 = State(state2Enter, state2Update, state2Exit);

extern FiniteStateMachine * fsm;
int internalState;

//enter the state1 state
void state1Enter() {

	internalState = 0;
	Serial.begin(115200);
	Serial.print("Entering state1\n");
}

void state1Update() {

	if (internalState == 12) {
		Serial.begin(115200);
		Serial.print("Transitioning to state2!\n");
		fsm->transitionTo(state2);
	}
	else {
		//do stuff
		internalState++;
	}
}

void state1Exit() {

	Serial.begin(115200);
	Serial.print("Exiting state1\n");
}

//enter the state2 state
void state2Enter() {

	internalState = 0;
	Serial.begin(115200);
	Serial.print("Entering state2\n");
}

void state2Update() {

	if (internalState == 17) {
		Serial.begin(115200);
		Serial.print("Transitioning to state1!\n");
		fsm->transitionTo(state1);
	}
	else {
		//do stuff
		internalState++;
	}
}

void state2Exit() {

	Serial.begin(115200);
	Serial.print("Exiting state2\n");
}