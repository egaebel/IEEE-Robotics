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
bool buttonPushed = false;

State state1 = State(state1Enter, state1Update, state1Exit);
State state2 = State(state2Enter, state2Update, state2Exit);
State state3 = State(state2Enter, state2Update, state2Exit);

FiniteStateMachine fsm(state1);

int internalState;
int curBayPos;
//enter the state1 state
void state1Enter() {
        move.init();
	internalState = 0;
	pinMode(53, INPUT);
	move.init();
}


void state1Update() {
	switch(internalState){
		case 0:
			move.forward(1);
                        Serial.println("Move forward");
			if(0){//digitalRead(53)){
				//fsm->immediateTransitionTo(state3);
				Serial.print("button press!\n");
				move.stop();
				internalState++;
			}
		break;
		case 1:
			move.slideRight(1);
		if(updatePosition(&curBayPos,uart.line,1)){
			move.stop();
			delay(2000);
		}
		break;
	}
}

void state1Exit() {
	Serial.print("Exiting state1\n");
}

//enter the state2 state
void state2Enter() {

	internalState = 0;
	curBayPos = -1;
	Serial.print("Entering state2\n");
}

void state2Update() {

	move.slideRight(1);
	Serial.print("state2UPDATE\n");
	if(updatePosition(&curBayPos,uart.line,1)){
		Serial.print("updatePosition?2????\n");
		move.stop();
		delay(2000);
	}
}

void state2Exit() {

	Serial.print("Exiting state2\n");
}
