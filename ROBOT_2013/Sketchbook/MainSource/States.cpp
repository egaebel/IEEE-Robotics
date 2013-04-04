#include "States.h"

extern FiniteStateMachine fsm;

//Externed hardwares
extern Sonar sonarRight;
extern Sonar sonarLeft;

extern IRAverager leftIR;
extern IRAverager rightIR;

extern cam leftCam;
extern cam rightCam;

//Hardware classes----
static Movement move;

//Timer Time needed to center in Air State
static Timer timer(1000);

//positions used for the state machine
static POSITION curPos = POS_START;
static POSITION nextPos;

//Variables for blocks needed in claws
static Block lTargetBlock;
static Block rTargetBlock;
static int lTargetPos;
static int rTargetPos;

//Variables for blocks currently held
static Block lBlock; //Block held by the left claw.
static Block rBlock; //Block held by the right claw.

//array of pointers to Blocks
//for reference::
    //*Air is the northmost zone
    //*Sea is the westmost zone
// Blocks in the loading zone, listed west to east.
static Block loadingZone[14];
//Sea zone colours, listed south to north.
static Block seaZone[6]; 
//Rail zone colours, listed west to east. 
static Block railZone[6]; 
//Air zone colours, listed west to east. 
static Block airZoneLeft; 


//Drop-off zones complete
static bool railDone;
static bool seaDone;

//Whether order of bays in air and whats in hand is the same
static bool airOrderSame;

static const int PICKUP_SIZE = 14;
static const int RAIL_SEA_SIZE = 6;
static const int AIR_SIZE = 2;

//used to keep track where we are in a zone 
    //(ie 2 would be 3rd block in a loading zone)
//what section of the zone the left claw is over
static int lBlockPos; 
//what section of the zone the right claw is over
static int rBlockPos;
//a sub-state used in each state, state-ception
static int internalState; 
//boolean used to indicate between states if we are scanning zones
static bool isScanning;


Block *blocks;


//State objects
//Put it here since it needs to know that the functions exist
State initState = State(initEnter, initUpdate, defExit);
State scanState = State(scanEnter, scanUpdate, defExit);
State moveToState = State(moveToEnter, moveToUpdate, defExit);
State pickUpState = State(pickUpEnter, pickUpUpdate, defExit);
State dropState = State(dropEnter, dropUpdate, defExit);
//End State objects

//*****START State Functions*****//
//initState Functions
void initEnter() {

    //initialize necessary variables
    internalState = 0;
    move.init();
    rightCam.init();
    leftCam.init();
    isScanning = true;
}

void initUpdate() {
    //setup wall follower

    switch(internalState){
        case 0:
            move.backward(0.25);
            move.setDown();
            internalState++;
            break;
        case 1:
            if(goToWall()){
                curPos = POS_START;
                nextPos = POS_SEA;
                fsm.transitionTo(moveToState);
            }
            break;
    }
}

void defExit() {}

//scanState
void scanEnter() {
    internalState = 0;
    rBlockPos = 0;
    lBlockPos = 0;
}

void scanUpdate() {
    //Perform the scanning actions
    switch(internalState){
        //Move until hitting a colour
        case 0:
            if (curPos == POS_SEA) {
                //focused on bay, read colour
                if(centerBay(RIGHT,curPos,RIGHT)){
                    move.stop();             
                    loadingZone[rBlockPos].colour = rightCam.getBlockColour(); 
                    loadingZone[rBlockPos].size = rightCam.getBlockSize(loadingZone[rBlockPos].colour);
                    rBlockPos++;
                    if(rBlockPos > 5)
                        internalState = 2;
                    else
                        internalState = 1;
                }
            }
            //scanning rail (state transition 6)
            else if (curPos == POS_RAIL) {
                //if we're focused on a bay, read colour
                if (centerBay(LEFT,curPos,LEFT)) {
                    move.stop();
                    loadingZone[lBlockPos].colour = leftCam.getBlockColour(); 
                    loadingZone[lBlockPos].size = leftCam.getBlockSize(loadingZone[lBlockPos].colour);
                    lBlockPos++;
                    if(lBlockPos > 5)
                        internalState = 2;
                    else
                        internalState = 1;
                }
            }
            //scanning pickup (state transition 4)
            else if (curPos == POS_PICK_UP) {
                //TODO: change to "onBlock" or something?
                if (centerBay(RIGHT,curPos,RIGHT)) {
                    move.stop();
                    loadingZone[rBlockPos].colour = rightCam.getBlockColour(); 
    	            loadingZone[rBlockPos].size = rightCam.getBlockSize(loadingZone[rBlockPos].colour);
                    loadingZone[rBlockPos].present = true;
                    rBlockPos++;
                    if(rBlockPos > 13)
                        internalState = 2;
                    else
                        internalState = 1;
                }
            }           
            //Scanning Air
            else if (curPos == POS_AIR) {
				internalState = 10;
				timer.init(500);  
				timer.start();
				move.backward(VERY_SLOW);
				break;
			}
        //We already read this colour, so just keep moving until white
        case 1:
            if(goToWall()){
                if(rightCam.betweenZones())
                        internalState = 0;
                if(curPos== POS_RAIL)
                    move.slideWall(LEFT);
                else
                    move.slideWall(RIGHT);
            }
            break;
        case 2:
            //nextPos and curPos transitions were handled in most recent moveTo call
            fsm.transitionTo(moveToState);
            break;
        case 10:
			if(timer.isDone() && leftIR.getIR() < 10.0 && rightIR.getIR() < 10.0) { //When timer is over and both IR's back on land
				move.stop();
				internalState++;
				move.slideLeft(VERY_SLOW); //Start strafing left
			}
            break;
        case 11:
			if(leftIR.getIR() > 10.0) { //If left IR hanging off edge
				move.stop();
				internalState++;
				move.slideRight(VERY_SLOW);	
			}
			break;
        case 12: //Strafe right until leftCam.inZone() of left most bay -> then read color
			if (leftCam.inZone()) { /**ASSUMING CAM WORKS FOR BAYS ON THE AIR PLATFORM**/
				move.stop();
				airOrderSame = (lBlock.colour == (airZoneLeft.colour = leftCam.getBlockColour())); //Assigns cam's detected color to left bay's color in airZone and tests whether it's equal to left held block's color
				fsm.transitionTo(dropState);	
			}
			break;
	}
}

//moveToState
void moveToEnter() {

    //initialize necessary variables & sensors
    internalState = 0;
}

//Handles all the movement, set curPos and nextPos before entering
void moveToUpdate() {

    //Start to sea (1 in state diagram) 
    if(curPos == POS_START && nextPos == POS_SEA){
        if(goToBay(POS_SEA,0,RIGHT)){
            fsm.transitionTo(scanState);
        }
    }
    //pickup to sea (10 in state diagram)
    else if (curPos == POS_PICK_UP && nextPos == POS_SEA) {

        switch (internalState) {

            case 1:
                if(move.backOffWall())
                    internalState++;
                break;
            case 2:
                if(move.turn90(LEFT))
                    internalState++;
                break;
            //move to wall
            case 3:
                if (goToWall()) {
                    curPos = nextPos;
                    nextPos = POS_PICK_UP;
                    fsm.transitionTo(dropState);  
                }
                break;
        }
    }
    //sea to pickup (12 in state diagram)
    else if (curPos == POS_SEA && nextPos == POS_PICK_UP) {

        switch (internalState) {

            //check if we need to move sector in zone
            case 0:
                //check if we are safe to turn around
                if (sonarRight.getDistance() >= SEA_SAFE_ZONE) {
                    move.slideLeft(0.1);
                    //check if we're in the safe zone now
                    if (sonarRight.getDistance() >= SEA_SAFE_ZONE) {
                        internalState++;
                    }
                }
                else {
                    internalState++;
                }
                break;
            //move back..and
            case 1:
                if(move.backOffWall())
                    internalState++;
                break;
            //turn right
            case 2:
                move.turn90(RIGHT);
                internalState++;
                break;
            //move to wall
            case 3:
                if (goToWall()) {
                    internalState++;
                }
                break;
            //TODO: Review this
            case 4:
                curPos = nextPos;
                
                if (!seaDone) { 
                    nextPos = POS_SEA;
                }
                //else if (seaDone && railDone) {} //maybe....
                else {
                    nextPos = POS_RAIL;
                }

                //if we need to scan pickup
                if (isScanning) {
                    fsm.transitionTo(scanState);
                }
                else {
                    fsm.transitionTo(pickUpState);
                }

                break;
        }
    }
    //pickup to rail (16 & 22 in state diagram)
    else if(curPos == POS_PICK_UP && nextPos == POS_RAIL){
        
        switch(internalState) {
            //backup 
            case 0:
                if(move.backOffWall())
                    internalState++;
                break;
            case 1:
                if(move.turnAround())
                    internalState++;
                break;
            case 2:
                if(goToWall()) {
                    internalState++;
                }
                break;
            //figure out whether to go left or right based on location in rail
            //TODO: review this
            case 3:
                if (isScanning) {
                    curPos = nextPos;
                    nextPos = POS_PICK_UP;
                    fsm.transitionTo(scanState);
                }
                else {
                    //if rail isn't full
                    if (!fullOfBlocks(railZone, RAIL_SEA_SIZE)) {
                        curPos = nextPos;
                        nextPos = POS_PICK_UP;
                        fsm.transitionTo(dropState);
                    }
                    //if air isn't full (which it won't be)
                    else {
                        //TODO: Kickstart the air states
                        curPos = nextPos;
                        nextPos = POS_AIR;
                        fsm.transitionTo(moveToState);
                    }
                }
                //TODO: not sure if we need to change enter...it currently takes a *fsm...
                //fsm.enter();
                break;
        }
    }
    //rail to pickup (7 & 18 in state diagram)
    else if (curPos == POS_RAIL && nextPos == POS_PICK_UP) {

        switch (internalState) {

            case 0:
                if(move.backOffWall())
                    internalState++;
                break;
            case 1:
                if(move.turnAround())
                    internalState++;
                break;
            case 2:
                if (goToWall()) {
                    internalState++;
                }
                break;
            //TODO: REVIEW THIs
            case 3:
                curPos = nextPos;
                if (!seaDone) {
                  nextPos = POS_SEA;
                }
                else {
                  nextPos = POS_RAIL;
                }
                fsm.transitionTo(pickUpState);
                break;
        }
    }
    //Moving from Pick_Up to Air
    else if (curPos == POS_PICK_UP && nextPos == POS_AIR) {
		switch (internalState) {
			
			case 0: //Turn around to face wall oposite pick_up area
				move.turnAround();
				internalState = 2;
				break;
				
			case 1: //Once reached wall go to next internalState
				if(goToWall()) {
					internalState++;
					move.slideRight(FAST);
				}
				break;
				
			case 2: 
				if(true /*SONAR DISTANCE FROM WALL CERTAIN DISTANCE*/) {
					move.stop();
					move.turnAround();
					move.stop();
					move.forward(MEDIUM);
					internalState++;
				}
				break;
						
			case 3: //Move forward until reaching the mid-point ramp's overhang
				if(leftIR.getIR() > 10.0 && rightIR.getIR() > 10.0) { //If front is off edge
					move.stop();
					internalState++;
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
					internalState++;
					move.forward(MEDIUM);
				}
				break;
				
			case 5: //Move forward until the air loading zone is reached. Once reached, transition to scanning state.
				if(leftIR.getIR() > 10.0 && rightIR.getIR() > 10.0) { //Front (both IR's) hanging off front edge of Air platform
					move.stop();
					internalState++;
					fsm.transitionTo(scanState);
					curPos = POS_AIR; //Now at air
				} else if(leftIR.getIR() > 10.0) {
					
				} else if(rightIR.getIR() > 10.0) {
					
				}
				break;
		}
	}
    
    else {
        //shouldn't be here, maybe make another case?
        //assert(0);
    }
}

//pickUpState
void pickUpEnter() {
    internalState = 0;

  	//Figure out which blocks you need to pick up
	if(!seaDone) {
		for(int i = 0; i < 6; i++) {
			if(seaZone[i].present == false) {
				lTargetBlock = seaZone[i];
				rTargetBlock = seaZone[++i];
                break;
			}
		}
	}
	
	else if(!railDone) {
		for(int i = 0; i < 6; i++) {
			if(!railZone[i].present == false) {
				lTargetBlock = railZone[i];
				rTargetBlock = railZone[++i];
                break;
			}
		}
	}
	
	else {
	    int i = 0;
		for(i = 0; i < 14; i++) {
			if(loadingZone[i].present == true) {
				lTargetBlock = loadingZone[i];
				break;
			}
		}
		
		for(i++; i < 14; i++) {
			if(loadingZone[i].present == true) {
				rTargetBlock = loadingZone[i];
                break;
			}
		}
	}
	
	//Set target positions of blocks in loading
	for(int i = 0; i < 14; i++) {
		if(loadingZone[i].colour == lTargetBlock.colour && loadingZone[i].size == lTargetBlock.size) {
			lTargetPos = i;
		}
		else if(loadingZone[i].colour == rTargetBlock.colour && loadingZone[i].size == rTargetBlock.size) {
			rTargetPos = i;
		}
	}
}

void pickUpUpdate() {

	//Perform the pick up actions
	switch(internalState) {
		//Move to the left target
		case 0:
			if(lBlockPos == lTargetPos) {
				internalState++;
			} 
            //if the block is to the right of the left claw
            else if(lBlockPos < lTargetPos) {
		        move.slideRight(0.25);
                //TODO: change to "onBlock" or something?
                if(leftCam.inZone())
                {
                    if(lBlockPos < 0)
                    {
                        lBlockPos = 0;
                        rBlockPos = 1;
                    }
                    else if (leftCam.getBlockColour() != loadingZone[lBlockPos].colour
                        || leftCam.getBlockSize(leftCam.getBlockColour()) != loadingZone[lBlockPos].size)
                    {
                        move.stop();
                        lBlockPos++;
                        rBlockPos++;
                    }
                }
			} 
            else if(lBlockPos > lTargetPos) {
                move.slideLeft(0.25);
                //TODO: change to "onBlock" or something?
                if(leftCam.inZone())
                {
                    if(lBlockPos > 13)
                    {
                        lBlockPos = 13;
                        rBlockPos = 14;
                    }
                    else if (leftCam.getBlockColour() != loadingZone[lBlockPos].colour 
                        || leftCam.getBlockSize(leftCam.getBlockColour()) != loadingZone[lBlockPos].size)
                    {
                        move.stop();
                        lBlockPos--;
                        rBlockPos--;
                    }
                }
			}
			break;
		//Pick up left target
		case 1:
			move.openClaw(LEFT);
			move.extendClaw(LEFT);
			move.closeClaw(LEFT);
                        move.retractClaw(LEFT);
                        loadingZone[lTargetPos].present = false;
       		        internalState++;
			break;
		//Move to the right target
		case 2:
			if(rBlockPos == rTargetPos) {
				internalState++;
			} 
            else if(rBlockPos < rTargetPos) {
                move.slideRight(0.25);
                //TODO: change to "onBlock" or something?
                if(rightCam.inZone())
                {
                    if(rBlockPos < 0)
                    {
                        rBlockPos = 0;
                        rBlockPos = -1;
                    }
                    else if (rightCam.getBlockColour() != loadingZone[rBlockPos].colour
                        || rightCam.getBlockSize(rightCam.getBlockColour()) != loadingZone[rBlockPos].size)
                    {
                        move.stop();
                        lBlockPos++;
                        rBlockPos++;
                    }
                }
			} 
            else if(rBlockPos > rTargetPos) {
				move.slideLeft(0.25);
                //TODO: change to "onBlock" or something?
                if(rightCam.inZone())
                {
                    if(rBlockPos > 13)
                    {
                        lBlockPos = 12;
                        rBlockPos = 13;
                    }
                    else if (rightCam.getBlockColour() != loadingZone[rBlockPos].colour
                        || rightCam.getBlockSize(rightCam.getBlockColour()) != loadingZone[rBlockPos].size)
                    {
                        move.stop();
                        lBlockPos--;
                        rBlockPos--;
                    }
                }
			}
			break;
		
		//Pick up right target
		case 3:
			move.openClaw(RIGHT);
			move.extendClaw(RIGHT);
			move.closeClaw(RIGHT);
                        move.retractClaw(RIGHT);
                        loadingZone[rTargetPos].present = false;
			internalState++;
			break;
        case 4:
            fsm.transitionTo(moveToState);
            break;
	}
}

//dropState
void dropEnter() {
    internalState = 0;
    blocks = getZoneByPos(curPos, seaZone, railZone, loadingZone);

    //Set target positions of blocks in loading
    for(int i = 0; i < 6; i++) {
        if(blocks[i].colour == lBlock.colour) {
            lTargetPos = i;
        }
        else if(blocks[i].colour == rTargetBlock.colour) {
            rTargetPos = i;
        }
    }
}

void dropUpdate() {
    static side activeClaw;
    //we aren't at air (phew!)
    if (curPos != POS_AIR) {    
        //scan and move until the second colour is encountered (main case)
        switch (internalState) {
            case 0:
                if(!blocks[lTargetPos].present)
                    if(goToBay(curPos,lTargetPos,LEFT)){
                        activeClaw = LEFT;
                        blocks[lTargetPos].present = 1;
                        internalState++;
                    }

                else if (!blocks[rTargetPos].present){
                    if(goToBay(curPos,rTargetPos,RIGHT)){
                        activeClaw = LEFT;
                        blocks[rTargetPos].present = 1;
                        internalState++;
                    }
                }
                else{
                    fsm.transitionTo(moveToState);
                    nextPos = POS_PICK_UP;
                }
                break;
            case 1:
                if(centerBay(RIGHT,curPos,RIGHT))
                    internalState++;
                break;
            case 2:
                if(move.extendClaw(activeClaw))
                    internalState++;
                break;
            case 3:
                if(move.openClaw(activeClaw))
                    internalState++;
                break;
            case 4:
                if(move.retractClaw(activeClaw))
                    internalState++;
                break;
            case 5:
                if(move.closeClaw(activeClaw))
                    internalState = 0;
                break;
        }
    }
    
    //we are at air and at right edge of air platform
    else {
		if(airOrderSame)  {
			switch (internalState) {
				case 0://Assume that when right block over right target bay, left held block over left target bay
					move.extendClaw(LEFT);
					move.openClaw(LEFT);
					move.retractClaw(LEFT);
					move.closeClaw(LEFT);
					
					move.extendClaw(RIGHT);
					move.openClaw(RIGHT);
					move.retractClaw(RIGHT);
					move.closeClaw(RIGHT);
					
					internalState++;
				
				case 1:
					//Not needed unless above assumption (in  comment above) is false
					break;
			}
		}
		
		else { //Order of bay colors not same as order of blocks in hand
			switch (internalState) {
				case 0:
					internalState++;
					timer.init(500);
					timer.start();
					move.backward(VERY_SLOW);
					break;
					
				case 1:
					if(timer.isDone() && leftIR.getIR() < 10.00 && rightIR.getIR() < 10.00) {
						move.stop();
						internalState++;	
						move.slideLeft(VERY_SLOW);
					}
					break;
					
				case 2: //Strafe to left edge
					if(leftIR.getIR() > 10.00) {
						move.stop();
						internalState++;	
						move.forward(VERY_SLOW);
					}
					break;
				
				case 3:
					if(rightIR.getIR() > 10.00)  { //If right IR is over the edge of the platform (because left already is over)
						internalState++;
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
						internalState++;
						move.backward(VERY_SLOW);
						timer.init(2000); //Set timer for moving back enough (to avoid hitting placed block)
						timer.start();
					}
					break;
					
				case 5: //BackUp (to avoid hitting placed block in upcoming next internal-state's right strafe)
					if(timer.isDone() && rightIR.getIR() > 10.00 && leftIR.getIR() > 10.00) {
						move.stop();
						move.slideRight(VERY_SLOW);
						internalState++;
					}
					break;
				
				case 6:
					if(rightIR.getIR() > 10.00) { //When reaches the right edge
						move.stop();
						move.forward(VERY_SLOW);
						internalState++;
					}
					break;
				
				case 7:
					if(leftIR.getIR() > 10.0 && rightIR.getIR() > 10.0) { //If at front edge of the air platform
						move.stop();
						move.slideLeft(VERY_SLOW); //start left strafe
						internalState++;
					}
					break;
					
				case 8:
					if (rightCam.inZone()) { //left cam over right bay (b/c left cam will hit the right bay first (coming left from the left edge))
						move.stop();
						move.extendClaw(LEFT);
						move.openClaw(LEFT);
						move.retractClaw(LEFT);
						move.closeClaw(LEFT);
						internalState++;
					}
					break;
			}	
		}
	}    
}


//returns true if we are centered
    //false if we are not
bool center() {

    //if in a bay on both {
        //do stuff with white lines and bays to get a full bay in view of each cmu cam (equally)
        //if the boxes are the same size and the space on the side of the boxes are the same (we're centered!)
        //if (/*leftCam.getBoxWidth() == rightCam.getBoxWidth() 
            //&& leftCam.getLeftOfBoxWidth() == rightCam.getLeftOfBoxWidth() 
            //&& leftCam.getRightOfBoxWidth() == rightCam.getRightOfBoxWidth()*/) {

            //return true;
        //}
        //if there are white lines in the center (not exactly centered) of the camera view
            //and there are colors to the left of both white lines
        //else if (/*(leftCam.hasWhiteLineInCenter() && leftCam.hasColorOnLeft()) 
            //&& (rightCam.hasWhiteLineInCenter() && rightCam.hasColorOnLeft())*/) {
            //go left
        //}
        //if there are white lines in the center (not exactly centered) of the camera view
            //and there are colors to the right of both white lines.
        //else if (/*(leftCam.hasWhiteLineInCenter() && leftCam.hasColorOnRight()) 
            //&& (rightCam.hasWhiteLineInCenter() && rightCam.hasColorOnRight())*/) {
            //go right
        //}
        //
    //}
    //else {
        //return false;
    //}

    return false;
}

/*
bool centered(cam &theCam)
{
	if(theCam.locateZone() > 0)
	{
		move.slideRight(0.1);
	}
	else
	{
		move.slideLeft(0.1);
	}
	if (theCam.inZone())
	{
		move.stop();
		return true;
	}
	return false;
}
*/
//*****END State Functions*****//
