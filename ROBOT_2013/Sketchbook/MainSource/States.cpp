#include "States.h"
#include "airState.h"

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
Block lBlock; //Block held by the left claw.
Block rBlock; //Block held by the right claw.

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

//Drop-off zones complete
static bool railDone;
static bool seaDone;

//Whether order of bays in air and whats in hand is the same
static bool airOrderSame;

const int PICKUP_SIZE = 14;
const int RAIL_SEA_SIZE = 6;
const int AIR_SIZE = 2;

//used to keep track where we are in a zone 
    //(ie 2 would be 3rd block in a loading zone)
//what section of the zone the left claw is over
static int lBlockPos; 
//what section of the zone the right claw is over
static int rBlockPos;
//a sub-state used in each state, state-ception
static int internalState;

bool isScanning; 


Block *blocks;


//State objects
//Put it here since it needs to know that the functions exist
State initState = State(initEnter, initUpdate, defExit);
State scanState = State(scanEnter, scanUpdate, defExit);
State moveToState = State(moveToEnter, moveToUpdate, defExit);
State pickUpState = State(pickUpEnter, pickUpUpdate, defExit);
State dropState = State(dropEnter, dropUpdate, defExit);
/** Air **/
State moveToAirState = State(moveToPlatformEnter, moveToAirPlatform, moveAP_cleanUp);

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
