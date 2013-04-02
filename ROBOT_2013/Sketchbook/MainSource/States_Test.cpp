#include <Arduino.h>
#include "States_Test.h"
#include "FiniteStateMachine.h"
#include "Movement.h"
#include "cam.h"
#include "Sonar.h"
using namespace std;

UART_STRUCT uart;

extern Movement move;
extern cam rightCam;

//Prototypes
void state1Enter();
void state1Update();
void state1Exit();

void state2Enter();
void state2Update();
void state2Exit();

State state1 = State(state1Enter, state1Update, state1Exit);
State state2 = State(state2Enter, state2Update, state2Exit);


extern Sonar sonarRight;
extern FiniteStateMachine fsm_test;

int internalState;
int curBayPos;
//enter the state1 state
void state1Enter() {
	internalState = 0;
	pinMode(53, INPUT);
	move.init();
	rightCam.init();
}


void state1Update() {

	switch(internalState){
		case 21321:
			Serial.println(sonarRight.getDistance());
			break;
		case 45:
			rightCam.inZone();
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
		case 0:
			if(goToWall()){
				move.stop();
				internalState++;
			}
			break;
		case 1:
            if(digitalRead(52)&&digitalRead(53)){
		    	switch(rightCam.inZone())  {
		    		case CENTER:
		    			move.stop();
						Serial.println(rightCam.getBlockColour());
						internalState++;
						move.setSpeed(.25,0,-.1,-.1);
		    		break;
		    		case LEFT:
		    			move.setSpeed(.25,0,.05,.05);
		    		break;
		    		case RIGHT:
		    			move.setSpeed(.25,0,-.05,-.05);
		    		break;
		    		case NO_SIDE:
		    			move.setSpeed(.25,0,-.1,-.1);
		    		break;
		    	}


		    	/**if(rightCam.inZone() != NO_SIDE){
						move.stop();
						Serial.println(rightCam.getBlockColour());
						internalState++;
		    	}
		    	move.setSpeed(.25,0,-.1,-.1);*/
            }
            else{
            	move.stop();
                internalState = 0;
            }
		break;
		case 2:
			if(digitalRead(52)&&digitalRead(53)){
		    	if(rightCam.betweenZones()){
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
