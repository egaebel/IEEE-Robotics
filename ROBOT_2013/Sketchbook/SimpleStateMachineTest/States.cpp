#include <Arduino.h>
#include "States.h"
#include "FiniteStateMachine.h"
#include "Movement.h"
#include "cmu.h"
using namespace std;

UART_STRUCT uart;

Movement move;
cam camR(2);

//Prototypes
void state1Enter();
void state1Update();
void state1Exit();

void state2Enter();
void state2Update();
void state2Exit();

State state1 = State(state1Enter, state1Update, state1Exit);
State state2 = State(state2Enter, state2Update, state2Exit);

FiniteStateMachine fsm(state1);

int internalState;
int curBayPos;
//enter the state1 state
void state1Enter() {
	internalState = 0;
	pinMode(53, INPUT);
	move.init();
	camR.init();
}


void state1Update() {
	switch(internalState){
		case 0:
			if(digitalRead(53)){
				move.openRightClaw();
			}
			if(digitalRead(52)){
				move.closeRightClaw();
			}
			move.retractClaw();
		break;
		case 2:
			if(digitalRead(53)&&digitalRead(52)){
				move.stop();
				internalState++;
			}
            else if( digitalRead(53)){
            	move.setSpeed(0,.25,0,-1);
            }
            else if( digitalRead(52)){
            	move.setSpeed(.25,0,1,0);
            }
            else{
              	move.forward(0.25);
            }
			break;
		case 1:
            if(digitalRead(52)&&digitalRead(53)){
            	move.setSpeed(.25,0,-1,-1);
		    	if(camR.inZone()){
						move.stop();
						Serial.println(camR.getBlockColour());
		    	}
		    	move.setSpeed(.25,0,-1,-1);
            }
            else{
            	move.stop();
                internalState = 0;
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
	Serial.print("Entering state2\n");
}

void state2Update() {

}

void state2Exit() {

	Serial.print("Exiting state2\n");
}
