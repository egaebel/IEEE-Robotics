#include "States.h"
#include "airState.h"

extern FiniteStateMachine fsm;

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

//Non-Static
//Variables for blocks currently held
Block rBlock;
Block lBlock;

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
//pointer used to point to one of the bays
static Block *blocks;

//Drop-off zones complete
static bool railDone;
static bool seaDone;

//The sizes of the zones
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
//boolean indicating if the robot is scanning zones
static bool isScanning; 

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
        //TODO: CHANGE BACK TO 0
    internalState = 2;
    move.init();
    isScanning = true;
}

void initUpdate() {
    //setup wall follower
    switch(internalState){
        case 0:
            //TODO: probably needs to back up more than normal
            if (move.backOffWall()) {
                internalState++;
            }
            break;
        case 1:
            if (move.setDown()) {
                internalState++;
            }
        case 2:
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
    
    fsm.transitionTo(moveToState);
    /*
    //Perform the scanning actions
    switch(internalState){
        //Move until hitting a colour
        case 0:
			Serial.println("Reached case 1");
            if (curPos == POS_SEA) {
                //focused on bay, read colour
                if(centerBay(RIGHT,curPos,RIGHT)){
                    move.stop();             
                    seaZone[rBlockPos].colour = rightCam.getBlockColour(); 
                    seaZone[rBlockPos].size = rightCam.getBlockSize(loadingZone[rBlockPos].colour);
                    seaZone[rBlockPos].present = false;
                    rBlockPos++;
                    if(rBlockPos == 5)
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
                    railZone[lBlockPos].colour = leftCam.getBlockColour(); 
                    railZone[lBlockPos].size = leftCam.getBlockSize(loadingZone[lBlockPos].colour);
                    railZone[lBlockPos].present = false;
                    lBlockPos++;
                    if(lBlockPos == 5)
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
                    loadingZone[lBlockPos].colour = leftCam.getBlockColour(); 
    	            loadingZone[lBlockPos].size = leftCam.getBlockSize(loadingZone[rBlockPos].colour);
                    loadingZone[lBlockPos].present = true;
                    lBlockPos++;
                    if(lBlockPos == 13) {
                        internalState = 2;
                        isScanning = false;
					}
                    else {
                        internalState = 1;
					}
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
	} */
}
void moveToExit(){

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
        Serial.println("MOVE TO: START TO SEA");
        if(goToBay(POS_SEA,0,RIGHT)){
            curPos = nextPos;
            nextPos = POS_RAIL;
            fsm.transitionTo(scanState);
        }
    }
    //pickup to sea (10 in state diagram)
/*    Back off wall, turns 90 deg left to face sea wall,
    goes to wall, switches to drop off state*/
    else if (curPos == POS_PICK_UP && nextPos == POS_SEA) {
        Serial.println("MOVE TO: PICK_UP TO SEA");
        switch (internalState) {

            case 0:
                if(move.backOffWall())
                    internalState++;
                break;
            case 1:
                if(move.turn90(LEFT))
                    internalState++;
                break;
            //move to wall
            case 2:
                if (goToWall()) {
                    curPos = nextPos;
                    nextPos = POS_PICK_UP;
                    fsm.transitionTo(dropState);  
                }
                break;
        }
    }
/*    Move left if we need to by checking sonar, so we don't pack up into pickUp,
    back off of wall, turn 90 to the right (pickup zone), go to wall,
    if the scanning flag is set, switch to scan state else switch to pickup, 
    if we are done with sea,make rail the next zone after pick up */
    else if (curPos == POS_SEA && nextPos == POS_PICK_UP) {
        Serial.println("MOVE TO: SEA TO PICKUP");
        switch (internalState) {
            //check if we need to move sector in zone
            case 0:
                //check if we are safe to turn around
                move.slideLeft(0.1);
                if (sonarLeft.getDistance() < SEA_SAFE_ZONE) {
                    move.stop();
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
                if(move.turn90(RIGHT))
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
    //Move off wall, do 180 to face rail zone, go to rail wall, switch to drop off state
    else if(curPos == POS_PICK_UP && nextPos == POS_RAIL){
        Serial.println("MOVE TO: PICKUP TO RAIL");
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
                        curPos = nextPos;
                        nextPos = POS_AIR;
                        fsm.transitionTo(moveToState);
                    }
                }
                break;
        }
    }
    //Move off of wall, 180 to face pick up, go to pick_up wall
    //Transition to scan state if scan flag is still set, else pick up
    else if (curPos == POS_RAIL && nextPos == POS_PICK_UP) {
        Serial.println("MOVE TO: RAIL TO PICKUP");
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

                //check if we need to be scanning pickup
                if (isScanning) {
                    fsm.transitionTo(scanState);
                }
                else {
                    fsm.transitionTo(pickUpState);
                }
                break;
        }
    }
    //back off wall, turn 90 to the left, transition to scanstate
    else if(curPos == POS_SEA && nextPos == POS_RAIL){
        Serial.println("MOVE TO: SEA TO RAIL");
        switch(internalState){
            case 0:
                move.slideLeft(0.1);
                if (sonarLeft.getDistance() < SEA_SAFE_ZONE) {
                    move.stop();
                    internalState++;
                }
            case 1:
                if(move.backOffWall())
                    internalState++;
                break;
            
            case 2:
                if(move.turn90(LEFT)){

                }
                break;
            case 3:
                if(goToWall()){
                    curPos = nextPos;
                    nextPos = POS_PICK_UP;
                    fsm.transitionTo(scanState);
                }
                break;
        }
    }
    else {
        //shouldn't be here, maybe make another case?
        //assert(0);
    }
}

void pickUpShortestEnter(){
    internalState = 0;
    Block *curZone;
    int min1Dist = 9999;
    int min2Dist = 9999;
    int tempDist;
    int block1;
    int block2;
    int i1;
    int i2;
    //What type of blocks are we doing?
    if(!seaDone)
        curZone = seaZone;
    else if(!railDone)
        curZone = railZone;
    //what are the two closest blocsks?
    if(!seaDone || !railDone){
        for(int i = 0; i < 6;i++){
            if(curZone[i].present == false){
                tempDist = getBayDist(POS_PICK_UP,curZone[i].loadPos,RIGHT);
                if(tempDist < min1Dist){
                    min2Dist = min1Dist;
                    block2 = block1;
                    i2 = i1;
                    min1Dist = tempDist;
                    block1 = curZone[i].loadPos;
                    i1 = i;
                }
                else if(tempDist <= min2Dist){
                    min2Dist = tempDist;
                    block2 = curZone[i].loadPos;
                    i2 = i1;
                }
            }
        }
    }
    //this is air!
    else{

        //find the air blocks
        int i = 0;
        for(; i < 14; i++) {
            if(loadingZone[i].present == true) {
                block2 = i;
                break;
            }
        }
        for(; i < 14; i++) {
            if(loadingZone[i].present == true) {
                block1 = i;
                break;
            }
        }

        //if the right block is closer, swap the two blocks
        if(getBayDist(POS_PICK_UP, block1, RIGHT) < getBayDist(POS_PICK_UP, block2, RIGHT)){
            block2 ^= block1;
            block1 ^= block2;
            block2 ^= block1;
        }
    }
    
    //if block2 is in the last pickup position
        //or if we're dropping off at the last position in sea
    if (block2 == 13 || (i2 == 5 && !seaDone)) {
        rTargetPos = block2;
        lTargetPos = block1;
    }
    //base case
    else {
        rTargetPos = block1;
        lTargetPos = block2;
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
		for(; i < 14; i++) {
			if(loadingZone[i].present == true) {
				lTargetBlock = loadingZone[i];
				break;
			}
		}
		for(; i < 14; i++) {
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

	Serial.println("in PickUp update");
	//Perform the pick up actions
	switch(internalState) {
		//Move to the left target
		case 0:
			if (goToBay(POS_PICK_UP, lTargetPos, LEFT)) {
                internalState++;
            }
            break;
        //center over left bay
        case 1:
            if(centerBay(LEFT, curPos, LEFT)) {
                internalState++;
            }
            break;
		//Pick up left target
		case 2:
			if (move.pickupClaw(LEFT)) {
                lBlock = loadingZone[lTargetPos];
                loadingZone[lTargetPos].present = false;
    	        internalState++;
            }
			break;
		//Move to the right target
		case 3:
            if (goToBay(POS_PICK_UP, rTargetPos, RIGHT)) {
                internalState++;
            }
            break;
        //center over left bay
        case 4:
            if(centerBay(RIGHT, curPos, RIGHT)) {
                internalState++;
            }
            break;
		//Pick up right target
		case 5:
			if(move.pickupClaw(RIGHT)) {
                rBlock = loadingZone[rTargetPos];
                loadingZone[rTargetPos].present = false;
                internalState++;
            }
			break;
        //transitioning
        case 6:
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
        if(blocks[i].colour == lTargetBlock.colour) {
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
                if(!blocks[lTargetPos].present) {
                    if(goToBay(curPos,lTargetPos,LEFT)){
                        activeClaw = LEFT;
                        blocks[lTargetPos].present = 1;
                        internalState++;
                    }
                }
                else if (!blocks[rTargetPos].present){
                    if(goToBay(curPos,rTargetPos,RIGHT)){
                        activeClaw = RIGHT;
                        blocks[rTargetPos].present = 1;
                        internalState++;
                    }
                }
                else {
                    fsm.transitionTo(moveToState);
                }
                break;
            //setup which claw to center over
            case 1:
                if(centerBay(activeClaw,curPos,activeClaw))
                    internalState++;
                break;
            //drop the claw
            case 2:
                if (move.dropClaw(activeClaw))
                    internalState = 0;
                break;
        }
    }
    //we are at air and at right edge of air platform
    else {
		
	}    
}

//*****END State Functions*****//
