#include <Arduino.h>
#include "States_Test.h"
#include "FiniteStateMachine.h"
#include "Movement.h"
#include "cam.h"
using namespace std;

UART_STRUCT uart;

Movement move;
cam camR(3);

//Prototypes
void state1Enter();
void state1Update();
void state1Exit();

void state2Enter();
void state2Update();
void state2Exit();

State state1 = State(state1Enter, state1Update, state1Exit);
State state2 = State(state2Enter, state2Update, state2Exit);

extern FiniteStateMachine fsm_test;

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
			camR.getTrackingData(BROWN);
		break;
		case 45:
			camR.inZone();
			break;
		case 69:
			if(digitalRead(53)){
				//move.openClaw(RIGHT);
				move.retractClaw(RIGHT);
			}
			if(digitalRead(52)){
				move.extendClaw(LEFT);
				//move.closeClaw(RIGHT);
			}
		break;
		case 32:
			if(goToWall(&move)){
				move.stop();
				internalState++;
			}
			break;
		case 1:
            if(digitalRead(52)&&digitalRead(53)){
		    	if(camR.inZone()){
						move.stop();
						Serial.println(camR.getBlockColour());
						internalState++;
		    	}
		    	move.setSpeed(.25,0,-.1,-.1);
            }
            else{
            	move.stop();
                internalState = 0;
            }
		break;
		case 2:
			if(digitalRead(52)&&digitalRead(53)){
		    	if(camR.betweenZones()){
						internalState--;
		    	}
		    	move.setSpeed(.25,0,-.1,-.1);
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
