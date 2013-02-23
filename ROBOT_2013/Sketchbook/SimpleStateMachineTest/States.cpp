#include <Arduino.h>
#include "States.h"
#include "FiniteStateMachine.h"
#include "EasyTransfer.h"
#include "Movement.h"

using namespace std;

UART_STRUCT uart;

Movement move;

boolean posLClaw(boolean line[16]){
	if(line[3]==1 && line[6]==1){
		return 1;
	}
}

boolean posRClaw(boolean line[16]){
	if(line[9]==1 && line[12]==1){
		return 1;
	}
	return 0;	
}
//dir=0 is moving left
boolean updatePosition(int* cur,boolean line[16],boolean dir){
	static int lastPos;
	if((*cur)==lastPos)
		return 0;

	if(dir){
		if(posRClaw(line)){
			(*cur)++;
			return 1;
		}
	}
	else{
		if(posLClaw(line)){
			(*cur)--;
			return 1;			
		}
	}

	return 0;
}







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
int curBayPos;
//enter the state1 state
void state1Enter() {

	internalState = 0;
	Serial.print("Entering state1\n");
	pinMode(53, INPUT);
}

void state1Update() {

	move.forward(1);
	if(digitalRead(53)){
		move.stop();
		fsm->transitionTo(state2);
	}
}

void state1Exit() {
}

//enter the state2 state
void state2Enter() {

	internalState = 0;
	curBayPos = -1;
	Serial.print("Entering state2\n");
}

void state2Update() {

	move.slideRight(1);
	if(updatePosition(&curBayPos,uart.line,1)){
		move.stop();
		delay(2000);
	}
}

void state2Exit() {

	Serial.print("Exiting state2\n");
}
