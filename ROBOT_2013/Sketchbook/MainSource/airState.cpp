#include "airState.h"

extern FiniteStateMachine fsm;

//Timer Time needed to center in Air State
static Timer timer(1000);

//Externed hardwares-----------------------//
//-----------------------------------------//
extern Sonar sonarRight;
extern Sonar sonarLeft;

extern IRAverager leftIR;
extern IRAverager rightIR;

extern cam leftCam;
extern cam rightCam;

extern Movement move;
//-----------------------------------------//
//End Externed Hardwares-------------------//

//Variables for blocks currently held
extern Block lBlock; //Block held by the left claw.
extern Block rBlock; //Block held by the right claw.

//Whether order of bays in air and whats in hand is the same
static bool airOrderSame;

static int airInternalState; //Used for internal state 

State scanAirPlatformState = State(scanAirPlatformEnter, scanAirPlatform, scanAP_cleanUp);
State dropAirBlocksState = State(dropAirBlocksEnter, dropAirBlocks, dropAP_cleanUp);

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
			
			if(move.backOffWall()) { //Back off the wall
				airInternalState++;
			}			
			break;
		
		case 1:
			if(move.turnAround(RIGHT)) {
				airInternalState++;
			}
			break;
			
		case 2: //Now turned, facing the opposite wall
			if(goToWall()) {
				move.stop();
				airInternalState++;
				move.slideWall(LEFT);
				timer.init(3000);
				timer.start();
			}
			break;
		case 3: //TESTTTT////
			if(!timer.isDone()) {
				move.stop();
                                airInternalState++;
			}
			break;
			
		// case 3: 
			// if(sonarLeft.getDistance() <= 20) { //Stop when 20cm from far-side wall
				// move.stop();
				// Serial.println("Begin Movin backward");
				// move.backwardForDuration(FAST, 100);
				// Serial.println("End Movin backward");
				// airInternalState++;
			// }
			// break;
			
		case 4:
			Serial.println("At case 4: moving off wall!");
			if(move.backOffWall()) { //Back off wall
				move.stop();
				airInternalState++;
				
			}
			break;
		
		case 5:
			if(move.turnAround(RIGHT)) {
				Serial.println("Moving forward");
				move.forwardForDuration(VERY_FAST, 1500);
				move.forward(MEDIUM); //Move forward
				/*STOP*/
                                airInternalState++;
			}
			break;
	
		case 6: //Move forward until reaching the mid-point ramp's overhang
			// if(leftIR.getIR() > 10.0 && rightIR.getIR() > 10.0) { //If front is off edge
			if(sonarLeft.getDistance() <= 25) {
				move.stop();
				Serial.println("Reached edge of first platform");
				move.backwardForDuration(VERY_SLOW, 100); //Move backwards from ledge
				move.turn90(LEFT); //Turn to face up-ramp
				Serial.println("Just turned left");
				//airInternalState++;
				move.forwardForDuration(FAST, 2500);
				move.forward(MEDIUM);
                                Serial.println("Going forward up second ramp");
                                airInternalState++;
			}
			break;
			
		case 7: //Move forward until the air loading zone is reached. Once reached, transition to scanning state.
			// if(leftIR.getIR() > 10.0 && rightIR.getIR() > 10.0) { //Front (both IR's) hanging off front edge of Air platform
			if(sonarRight.getDistance() <= 25) {
				move.stop();
                                Serial.println("Reached edge of second platform.");
				fsm.transitionTo(scanAirPlatformState);
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
		case 0: //At air platform, slide left.
			//Move backwards for time and until both IR's back over the platform
			while(leftIR.getIR() > 10.0 && rightIR.getIR() > 10.0) {
				move.backwardForDuration(VERY_SLOW,40);
                                Serial.println("Sliding back until not over edge.");
			}
			
			airInternalState++;
			move.slideLeft(SLOW); //Start strafing left
                        Serial.println("Strafing left to edge.");
			break;
			
        case 1: //Stop strafing left when left IR hanging off edge (At left edge of platform)
			if(leftIR.getIR() > 10.0) {
				move.stop();
                                Serial.println("Left side over the edge.");
				airInternalState++;
				move.slideRight(VERY_SLOW);
                                Serial.println("Sliding right ot left zone of air.");	
			}
			break;
        case 2: //Strafe right until leftCam.inZone() of left most bay -> then read color
			if (leftCam.inZone()) { /**ASSUMING CAM IN ZONE MEANS LEFT BLOCK IS OVER THE BAY**/
				move.stop();
                                Serial.println("Left cam is in left zone.");
				airOrderSame = (lBlock.colour == leftCam.getBayColour()); //Assigns cam's detected color to left bay's color in airZone and tests whether it's equal to left held block's color
				if(airOrderSame)
                                {
                                    Serial.println("Left block is the same as left bay. Same for right.");
                                }
                                else
                                {
                                    Serial.println("Left block is for right bay. Right block is for left bay.");
                                }
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
	if(airOrderSame)  { 
			switch (airInternalState) {
				//If order same as blocks in the hand
				case 0:
                                        Serial.println("Dropping both claws.");
					//Assume that when right block over right target bay, left held block over left target bay
					move.dropClaw(LEFT);
					move.dropClaw(RIGHT);
					
                                        airInternalState++;
					//<VICTORY DANCE!>
					break;
				
				//Use this case to infinitely repeat and keep robot from doing anything stupid (like trying to drop again).
				case 1:
                                        Serial.println("Finished course.");
					break;
			}
	}
		
	else { //Order of bay colors not same as order of blocks in hand
		switch(airInternalState) {
			case 0:
				//Move just-right of the current left bay, to get our of inZone() (skip current bay under left block)
				move.slideRightForDuration(VERY_SLOW, 30);
				Serial.println("Sliding right to line up left claw with right air zone.");
				move.slideRight(VERY_SLOW);
				airInternalState++; 
			
			case 1:
				if(leftCam.inZone()) {
					move.stop();
                                        Serial.println("Left cam is in right air zone.");
                                        Serial.println("Droping left claw.");
					move.dropClaw(LEFT);
					
                                        //This needs to be longer than previous state because need to slide left to 
					move.slideLeftForDuration(SLOW, 100);
                                        move.slideLeft(VERY_SLOW);
                                        Serial.println("Sliding left to line up right claw with left air zone.");
					airInternalState++;
				}
			case 2:
				if(rightCam.inZone()) { //right cam in right bay (if reads block as a bay)
					move.stop();
                                        Serial.println("Right cam is in left zone.");
                                        Serial.println("Droping right claw.");
					move.dropClaw(RIGHT);
                                        airInternalState++;
					//<VICTORY DANCE!>
				}
                         //Use this case to infinitely repeat and keep robot from doing anything stupid (like trying to drop again).
                         case 3:
                                 Serial.println("Finished course.");
                                 break;
		}
	}
			
		//switch (airInternalState) {
			//case 0:
				//airInternalState++;
				//timer.init(200);
				//timer.start();
				//move.backward(VERY_SLOW);
				//break;
				
			//case 1:
				//if(timer.isDone() && leftIR.getIR() < 10.00 && rightIR.getIR() < 10.00) {
					//move.stop();
					//airInternalState++;	
					//move.slideLeft(VERY_SLOW);
				//}
				//break;
				
			//case 2: //Strafe to left edge
				//if(leftIR.getIR() > 10.00) {
					//move.stop();
					//airInternalState++;	
					//move.forward(VERY_SLOW);
				//}
				//break;
			
			//case 3:
				//if(rightIR.getIR() > 10.00)  { //If right IR is over the edge of the platform (because left already is over)
					//airInternalState++;
					//move.slideRight(VERY_SLOW); //start right strafe
				//} 
				//break;
			
			//case 4: //Strafe right until right arm is over the air's left bay and drop the block
				//if (rightCam.inZone()) { //right cam over left bay (b/c rightcam will hit the left bay first)
					//move.stop();
					//move.extendClaw(RIGHT);
					//move.openClaw(RIGHT);
					//move.retractClaw(RIGHT);
					//move.closeClaw(RIGHT);
					//airInternalState++;
					//move.backward(VERY_SLOW);
					//timer.init(1000); //Set timer for moving back enough (to avoid hitting placed block)
					//timer.start();
				//}
				//break;
				
			//case 5: //BackUp (to avoid hitting placed block in upcoming next internal-state's right strafe)
				//if(timer.isDone() && rightIR.getIR() > 10.00 && leftIR.getIR() > 10.00) {
					//move.stop();
					//move.slideRight(VERY_SLOW);
					//airInternalState++;
				//}
				//break;
			
			//case 6:
				//if(rightIR.getIR() > 10.00) { //When reaches the right edge
					//move.stop();
					//move.forward(VERY_SLOW);
					//airInternalState++;
				//}
				//break;
			
			//case 7:
				//if(leftIR.getIR() > 10.0 ){ //If at front edge of the air platform (because right already off the edge)
					//move.stop();
					//move.slideLeft(VERY_SLOW); //start left strafe
					//airInternalState++;
				//}
				//break;
				
			//case 8:
				//if (rightCam.inZone()) { //left cam over right bay (b/c left cam will hit the right bay first (coming left from the left edge))
					//move.stop();
					//move.extendClaw(LEFT);
					//move.openClaw(LEFT);
					//move.retractClaw(LEFT);
					//move.closeClaw(LEFT);
					//airInternalState++;
				//}
				//break;
		//}	
}

void moveAP_cleanUp() {	};
void scanAP_cleanUp() { };
void dropAP_cleanUp() { };
