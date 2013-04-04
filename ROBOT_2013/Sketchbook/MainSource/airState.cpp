#include "airState.h"

extern FiniteStateMachine fsm;

//Timer Time needed to center in Air State
static Timer timer(1000);

//Externed hardwares
extern Sonar sonarRight;
extern Sonar sonarLeft;

extern IRAverager leftIR;
extern IRAverager rightIR;

extern cam leftCam;
extern cam rightCam;

//Hardware classes----
extern Movement move;

//Whether order of bays in air and whats in hand is the same
static bool airOrderSame;

//Air zone colours, listed west to east. 
static Block airZoneLeft; 

//Variables for blocks currently held
extern Block lBlock; //Block held by the left claw.
extern Block rBlock; //Block held by the right claw.

State scanAirPlatformState = State(scanAirPlatformEnter, scanAirPlatform, scanAP_cleanUp);
State dropAirBlocksState = State(dropAirBlocksEnter, dropAirBlocks, dropAP_cleanUp);

int airInternalState;

/**
 * called once when entering moveToPlatformEnterState
 */
void moveToPlatformEnter() {
	airInternalState = 0;
}

/**
 * Moving from Pick_Up to Air
 */ 
void moveToAirPlatform() {
	switch (airInternalState) {
		
		case 0: //Turn around to face wall oposite pick_up area
			move.turnAround();
			airInternalState++;
			break;
			
		case 1: //Once reached wall go to next internalState
			if(goToWall()) {
				airInternalState++;
				move.slideRight(FAST);
			}
			break;
			
		case 2: 
			if(sonarLeft.getDistance() < 20) /*SONAR DISTANCE FROM WALL CERTAIN DISTANCE*/ {
				move.stop();
				move.turnAround();
				move.stop();
				move.forward(MEDIUM);
				airInternalState++;
			}
			break;
					
		case 3: //Move forward until reaching the mid-point ramp's overhang
			if(leftIR.getIR() > 10.0 && rightIR.getIR() > 10.0) { //If front is off edge
				move.stop();
				airInternalState++;
				timer.init(1000);
				timer.start();
				move.backward(VERY_SLOW);
			} 
			else if (leftIR.getIR() > 10.0) { //If right IR off ledge
				//Do corrective actions
				
			} 
			
			else if (rightIR.getIR() > 10.0) { //If left IR off ledge
				//Do corrective actions
				
			}
			break;
			
		case 4: //Keep moving backwards until the timer is done, meaning the robot is centered enough, when it is, turn 90 deg to face air loading zone
			if(timer.isDone()) {
				move.stop();
				move.turn90(LEFT);
				airInternalState++;
				move.forward(MEDIUM);
			}
			break;
			
		case 5: //Move forward until the air loading zone is reached. Once reached, transition to scanning state.
			if(leftIR.getIR() > 10.0 && rightIR.getIR() > 10.0) { //Front (both IR's) hanging off front edge of Air platform
				move.stop();
				airInternalState++;
				fsm.transitionTo(scanAirPlatformState);
			} else if(leftIR.getIR() > 10.0) {
				
			} else if(rightIR.getIR() > 10.0) {
				
			}
			break;
	}
}

/**
 * Called once when entering scanAirPlatformState
 */
void scanAirPlatformEnter() {
	airInternalState = 0;
}

/**
 * Scans the left 
 */
void scanAirPlatform() {
	//Scanning Air
	switch (airInternalState) {
		case 0:
			airInternalState++;
			timer.init(500);  
			timer.start();
			move.backward(VERY_SLOW);
			break;
		
		case 1:
			if(timer.isDone() && leftIR.getIR() < 10.0 && rightIR.getIR() < 10.0) { //When timer is over and both IR's back on land
				move.stop();
				airInternalState++;
				move.slideLeft(VERY_SLOW); //Start strafing left
			}
			break;
        case 2:
			if(leftIR.getIR() > 10.0) { //If left IR hanging off edge
				move.stop();
				airInternalState++;
				move.slideRight(VERY_SLOW);	
			}
			break;
        case 3: //Strafe right until leftCam.inZone() of left most bay -> then read color
			if (leftCam.inZone()) { /**ASSUMING CAM WORKS FOR BAYS ON THE AIR PLATFORM**/
				move.stop();
				airOrderSame = (lBlock.colour == (airZoneLeft.colour = leftCam.getBlockColour())); //Assigns cam's detected color to left bay's color in airZone and tests whether it's equal to left held block's color
				fsm.transitionTo(dropAirBlocksState);	
			}
			break;
		}
}

/**
 * Called once when entering dropAirBlocksState
 */
void dropAirBlocksEnter() {
	airInternalState = 0;
}

/**
 *  Drop blocks in air bays
 */
void dropAirBlocks() {
	if(airOrderSame)  { //If order same as blocks in the hand
			switch (airInternalState) {
				case 0://Assume that when right block over right target bay, left held block over left target bay
					move.extendClaw(LEFT);
					move.openClaw(LEFT);
					move.retractClaw(LEFT);
					move.closeClaw(LEFT);
					
					move.extendClaw(RIGHT);
					move.openClaw(RIGHT);
					move.retractClaw(RIGHT);
					move.closeClaw(RIGHT);
					
					airInternalState++;
				
				case 1:
					//Not needed unless above assumption (in  comment above) is false
					break;
			}
		}
		
	else { //Order of bay colors not same as order of blocks in hand
		switch (airInternalState) {
			case 0:
				airInternalState++;
				timer.init(500);
				timer.start();
				move.backward(VERY_SLOW);
				break;
				
			case 1:
				if(timer.isDone() && leftIR.getIR() < 10.00 && rightIR.getIR() < 10.00) {
					move.stop();
					airInternalState++;	
					move.slideLeft(VERY_SLOW);
				}
				break;
				
			case 2: //Strafe to left edge
				if(leftIR.getIR() > 10.00) {
					move.stop();
					airInternalState++;	
					move.forward(VERY_SLOW);
				}
				break;
			
			case 3:
				if(rightIR.getIR() > 10.00)  { //If right IR is over the edge of the platform (because left already is over)
					airInternalState++;
					move.slideRight(VERY_SLOW); //start right strafe
				} 
				break;
			
			case 4: //Strafe right until right arm is over the air's left bay and drop the block
				if (rightCam.inZone()) { //right cam over left bay (b/c rightcam will hit the left bay first)
					move.stop();
					move.extendClaw(RIGHT);
					move.openClaw(RIGHT);
					move.retractClaw(RIGHT);
					move.closeClaw(RIGHT);
					airInternalState++;
					move.backward(VERY_SLOW);
					timer.init(1000); //Set timer for moving back enough (to avoid hitting placed block)
					timer.start();
				}
				break;
				
			case 5: //BackUp (to avoid hitting placed block in upcoming next internal-state's right strafe)
				if(timer.isDone() && rightIR.getIR() > 10.00 && leftIR.getIR() > 10.00) {
					move.stop();
					move.slideRight(VERY_SLOW);
					airInternalState++;
				}
				break;
			
			case 6:
				if(rightIR.getIR() > 10.00) { //When reaches the right edge
					move.stop();
					move.forward(VERY_SLOW);
					airInternalState++;
				}
				break;
			
			case 7:
				if(leftIR.getIR() > 10.0 ){ //If at front edge of the air platform (because right already off the edge)
					move.stop();
					move.slideLeft(VERY_SLOW); //start left strafe
					airInternalState++;
				}
				break;
				
			case 8:
				if (rightCam.inZone()) { //left cam over right bay (b/c left cam will hit the right bay first (coming left from the left edge))
					move.stop();
					move.extendClaw(LEFT);
					move.openClaw(LEFT);
					move.retractClaw(LEFT);
					move.closeClaw(LEFT);
					airInternalState++;
				}
				break;
		}	
	}
	
}

void moveAP_cleanUp() {	};
void scanAP_cleanUp() { };
void dropAP_cleanUp() { };