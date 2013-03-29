#include "States.h"

extern FiniteStateMachine fsm;

//Hardware classes----
static Movement move;
static WallFollower wallFollower;

static Claw rClaw;
static Claw lClaw;

static cam leftCam;
static cam rightCam;

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
static Block * loadingZone[14];
//Sea zone colors, listed south to north.
static Block * seaZone[6]; 
//Rail zone colors, listed west to east. 
static Block * railZone[6]; 

//Drop-off zones complete
static bool railDone;
static bool seaDone;

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

//State objects
//Put it here since it needs to know that the functions exist
State initState = State(initEnter, initUpdate, initExit);
State scanState = State(scanEnter, scanUpdate, scanExit);
State moveToState = State(moveToEnter, moveToUpdate, moveToExit);
State pickUpState = State(pickUpEnter, pickUpUpdate, pickUpExit);
State dropState = State(dropEnter, dropUpdate, dropExit);
//End State objects

//*****START State Functions*****//
//initState Functions
void initEnter() {

    //initialize necessary variables
    internalState = 0;
    move.init();
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
            move.forward(0.1);
            if(wallFollower.isTouching()){
                curPos = POS_START;
                nextPos = POS_SEA;
                fsm.transitionTo(moveToState);
            }
            break;
    }
}

void initExit() {}

//scanState
void scanEnter() {
    internalState = 0;
    rBlockPos = 0;
    lBlockPos = 0;
}

void scanUpdate() {
    
    //Perform the scanning actions
    switch(internalState){
        //Move until hitting a color
        case 0:
            //scanning sea (state transition 2 in state diagram)
            if (curPos == POS_SEA) {

                move.slideRight(0.25);

                //focused on bay, read color
                if(rightCam.inZone()){
                    move.stop();             
                    //put color read code here!
                    //--------------------------
                    //--------------------------

                    rBlockPos++;
                    if(rBlockPos > 5){
                        //we are done lets moveTo the next place
                        internalState = 2;
                    }
                    else{
                        internalState = 1;
                    }
                }
            }
            //scanning rail (state transition 6)
            else if (curPos == POS_RAIL) {

                move.slideLeft(0.25);

                //if we're focused on a bay, read color
                if (leftCam.inZone()) {
                    move.stop();
                    //put color read code here!
                    //--------------------------
                    //--------------------------
                    rBlockPos++;
                    if(rBlockPos > 5){
                        //we are done lets moveTo the next place
                        internalState = 2;
                    }
                    else{
                        internalState = 1;
                    }
                }
            }
            //scanning pickup (state transition 4)
            else {

                move.slideRight(0.25);
                //TODO: change to "onBlock" or something?
                if (rightCam.inZone()) {
                    move.stop();
                    //put color read code here!
                    //--------------------------
                    //--------------------------
                    loadingZone[rBlockPos]->colour = rightCam.detectColor(); 
	            loadingZone[rBlockPos]->size = rightCam.detectSize();
                    loadingZone[rBlockPos]->present = true;
                    rBlockPos++;
                    if(rBlockPos > 13){
                        //we are done lets moveTo the next place
                        internalState = 2;
                    }
                    else{
                        internalState = 1;
                    }
                }
            }

            break;
        //We already read this color, so just keep moving until white
        case 1:
            //move right
            if (curPos == POS_SEA || curPos == POS_PICK_UP) {
                move.slideRight(0.25);
                //TODO: CREATE inbetweenZones
                if(rightCam.inbetweenZones()){
                    move.stop();
                    internalState = 0;
                }
            }
            //move left
            else {
                move.slideLeft(0.25);
                //TODO: CREATE inbetweenZones
                if(leftCam.inbetweenZones()){
                    move.stop();
                    internalState = 0;
                }
            }
            break;
        case 2:
            //nextPos and curPos transitions were handled in most recent moveTo call
            fsm.transitionTo(moveToState);
            //TODO: not sure if we need to change enter...it currently takes a *fsm...
            //fsm.enter();
            break;
    }
}

void scanExit() {}

//moveToState
void moveToEnter() {

    //initialize necessary variables & sensors
    internalState = 0;
}

//Handles all the movement, set curPos and nextPos before entering
void moveToUpdate() {

    //Start to sea (1 in state diagram) 
    if(curPos == POS_START && nextPos == POS_SEA){
        switch(internalState){
            //move to first bay in sea
            case 0:
                move.slideRight(0.25);
                if(rightCam.inZone()){
                    move.stop();
                    internalState++;
                }
                break;
            //transition to scanning
            case 1:
                fsm.transitionTo(scanState); 
                curPos = nextPos;
                nextPos = POS_PICK_UP;
                break;
        }
    }
    //sea to pickup (scanning) (3 in the state diagram)
    else if (isScanning && curPos == POS_SEA && nextPos == POS_PICK_UP) {
        switch(internalState) {
            //internal states 0-2 backup, turn, and move to the wall
            case 0:
                //TODO: figure out the speed needed, or get stopping condition
                move.backward(0.1);
                internalState++;
                break;
            case 1:
                move.stop();
                //TODO: figure out the speed needed, or get stopping condition
                move.turnRight(0.1);
                internalState++;
                break;
            case 2:
                move.stop();
                move.forward(0.1);
                if (wallFollower.isTouching()) {
                    move.stop();
                    internalState++;
                }
                break;
            case 3:
                move.slideRight(0.1);
                if (rightCam.inZone()) {
                    move.stop();
                    internalState++;
                }
                break;
            case 4:
                curPos = nextPos;
                nextPos = POS_RAIL;
                fsm.transitionTo(scanState);
                break;
        }
    }
    //pickup to rail (scanning) (5 in the state diagram)
    else if(isScanning && curPos == POS_PICK_UP && nextPos == POS_RAIL){

        switch(internalState){    
            //backup from wall
            case 0:
                move.backward(0.25);
                internalState++;
                break;
            //turn around
            case 1:
                move.stop();
                move.turnAround();
                internalState++;
                break;
            //and hit the wall
            case 2:
                move.stop();
                move.forward(0.1);
                if(wallFollower.isTouching()) {
                    move.stop();
                    internalState++;
                }
                break;
            //slide Right until we are past the first bay
            case 3:
                move.slideRight(0.25);
                //TODO: MAKE "IFBLACK" FUNCTION
                if(!rightCam.inZone()) {
                    move.stop();
                    internalState++;
                }
                break;
            //slide left until we are IN the first bay
            case 4: 
                move.slideLeft(0.25);
                if (leftCam.inZone()) {
                    move.stop();
                    internalState++;
                }
                break;
            case 5:
                //We are done scanning after this last scan!
                isScanning = false;
                curPos = nextPos;
                nextPos = POS_PICK_UP;
                fsm.transitionTo(scanState);
                break;
        }
    }
    //pickup to sea (10 in state diagram)
    else if (curPos == POS_PICK_UP && nextPos == POS_SEA) {

        switch (internalState) {

            case 1:
                move.backward(0.1);
                internalState++;
                break;
            case 2:
                move.turnLeft(0.1);
                internalState++;
                break;
            case 3:
                move.forward(0.1);
                if (wallFollower.isTouching()) {
                    internalState++;
                }
                break;
            //center
            case 4:
                //center over block or sector (if not off the edge)
                //if centered internalState++;
                break;
            case 5:
                //figure out where we are! (in the middle of the sea)
                //rightCam.getBayColor()
                //leftCam.getBayColor();
                //if () {}
                    //internalState++;
                //to the left of the sea zone
                //else {}
                    //internalState += 2;
                break;
            case 6:
                //moving right
                move.slideRight(0.1);
                //if in zone and centered 
                internalState += 2;
                break;
            case 7:
                //moving left
                move.slideLeft(0.1);
                //if in zone and centered 
                internalState++;
                break;
            case 8:
                curPos = nextPos;
                nextPos = POS_PICK_UP;
                fsm.transitionTo(dropState);
                break;
        }
    }
    //sea to pickup (12 in state diagram)
    else if (curPos == POS_SEA && nextPos == POS_PICK_UP) {

        switch (internalState) {

            //check if we need to move sector in zone
            case 1:
                //check if we are safe to turn around
                //TODO must do stuff
                if (rBlockPos > 2) {
                    move.slideLeft(0.1);
                    //if we are now inZone
                    if (rightCam.inZone()) {
                        rBlockPos--;
                        lBlockPos--;
                    }
                    //if we are at the block at index 2, then we're safe
                    if (rBlockPos < 3) {
                        internalState++;
                    }
                }
                else {
                    internalState++;
                }
                break;
            //move back..and
            case 2:
                move.backward(0.1);
                internalState++;
                break;
            //turn right
            case 3:
                move.turnRight(0.1);
                internalState++;
                break;
            //move to wall
            case 4:
                move.forward(0.1);
                if (wallFollower.isTouching()) {
                    internalState++;
                }
                break;
            case 5: 
                move.slideRight(0.25);
                if (rightCam.inbetweenZones()) {
                    move.stop();
                    internalState++;
                }
                break;
            case 6:
                curPos = nextPos;
                
                if (!seaDone) { 
                    nextPos = POS_SEA;
                }
                //else if (seaDone && railDone) {} //maybe....
                else {
                    nextPos = POS_RAIL;
                }
                fsm.transitionTo(pickUpState);

                break;
        }
    }
    //pickup to rail (16 & 22 in state diagram)
    else if(curPos == POS_PICK_UP && nextPos == POS_RAIL){
        
        switch(internalState) {
            //backup 
            case 0:
                move.backward(0.1);
                internalState++;
                break;
            case 1:
                move.stop();
                move.turnAround();
                internalState++;
                break;
            case 2:
                move.stop();
                move.forward(0.1);
                if(wallFollower.isTouching()) {
                    move.stop();
                    internalState++;
                }
                break;
            //figure out whether to go left or right based on location in rail
            case 3:
                //in the middle of rail zone
                //if () {}
                    //internalState++;
                //to the right of the rail zone
                //else {}
                    //internalState += 2;
                break;
            //move left until in first rail bay
            case 4:
                move.slideLeft(0.25);
                if (leftCam.inZone()) {
                    move.stop();
                    internalState += 2;
                }
                break;
            //move right until in first rail bay
            case 5:
                move.slideRight(0.25);
                if (rightCam.inZone() && rBlockPos == 0) {
                    move.stop();
                    internalState++;
                }
                break;
            case 6:
                //if rail isn't full
                if (!fullOfBlocks(railZone, RAIL_SEA_SIZE)) {
                    curPos = nextPos;
                    nextPos = POS_PICK_UP;
                    fsm.transitionTo(dropState);
                }
                //if air isn't full (which it won't be)
                else {
                    //TODO: Kickstart the air states
                    //curPos = nextPos;
                    //nextPos = POS_AIR;
                    //fsm.transitionTo(moveToState);
                }
                //TODO: not sure if we need to change enter...it currently takes a *fsm...
                //fsm.enter();
                break;
        }
    }
  
    //rail to pickup (7 & 18 in state diagram)
    else if (curPos == POS_RAIL && nextPos == POS_PICK_UP) {

        switch (internalState) {

            case 1:
                move.backward(0.1);
                internalState++;
                break;
            case 2:
                move.turnAround();
                internalState++;
                break;
            case 3:
                move.forward(0.1);
                if (wallFollower.isTouching()) {
                    internalState++;
                }
                break;
            case 4:
                 //in the middle of pick up zone
                //if () {}
                    //internalState++;
                //to the right of the rail zone
                //else {}
                    //internalState += 2;
                break;
            case 5:
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

void moveToExit() {}

//pickUpState
void pickUpEnter() {
    internalState = 0;
    if(rightCam.inZone())
    {
        bColour currentColor = rightCam.detectColor();
        bSize currentSize = rightCam.detectSize();
        for(int i = 0; i < 14; i++) {
	        if(loadingZone[i]->colour == currentColor && loadingZone[i]->size == currentSize) {
			rBlockPos = i;
                        lBlockPos = i - 1;
		}
	}
    }
    else if(leftCam.inZone())
    {
        bColour currentColor = leftCam.detectColor();
        bSize currentSize = leftCam.detectSize();
        for(int i = 0; i < 14; i++) {
	        if(loadingZone[i]->colour == currentColor && loadingZone[i]->size == currentSize) {
			lBlockPos = i;
                        rBlockPos = i + 1;
		}
	}
    }
    else
    {
        rBlockPos = -1;
        lBlockPos = -2;
    }  
      	//Figure out which blocks you need to pick up
	if(!railDone) {
		for(int i = 0; i < 6; i++) {
			if(!railZone[i]->present == false) {
				lTargetBlock = *railZone[i];
				rTargetBlock = *railZone[++i];
			}
		}
	}
	else if(!seaDone) {
		for(int i = 0; i < 6; i++) {
			if(seaZone[i]->present == false) {
				lTargetBlock = *seaZone[i];
				rTargetBlock = *seaZone[++i];
			}
		}
	}
	else {
	int i = 0;
		for(i = 0; i < 14; i++) {
			if(loadingZone[i]->present == true) {
				lTargetBlock = *loadingZone[i];
				break;
			}
		}
		
		for(i++; i < 14; i++) {
			if(loadingZone[i]->present == true) {
				rTargetBlock = *loadingZone[i];
			}
		}
	}
	
	//Set target positions
	for(int i = 0; i < 14; i++) {
		if(loadingZone[i]->colour == lTargetBlock.colour && loadingZone[i]->size == lTargetBlock.size) {
			lTargetPos = i;
		}
		else if(loadingZone[i]->colour == rTargetBlock.colour && loadingZone[i]->size == rTargetBlock.size) {
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
			} else if(lBlockPos < lTargetPos) {
				move.slideRight(0.25);
                                //TODO: change to "onBlock" or something?
                                if(leftCam.inZone())
                                {
                                    if(lBlockPos < 0);
                                    {
                                        lBlockPos = 0;
                                        rBlockPos = 1;
                                    }
                                    else if (leftCam.detectColor() != loadingZone[lBlockPos]->colour || leftCam.detectSize() != loadingZone[lBlockPos]->size) {
                                        move.stop();
                                        lBlockPos++;
                                        rBlockPos++;
                                    }
                                }
			} else if(lBlockPos > lTargetPos) {
                                move.slideLeft(0.25);
                                //TODO: change to "onBlock" or something?
                                if(leftCam.inZone());
                                {
                                    if(lBlockPos > 13)
                                    {
                                        lBlockPos = 13;
                                        rBlockPos = 14;
                                    }
                                    else if (leftCam.detectColor() != loadingZone[lBlockPos]->colour || leftCam.detectSize() != loadingZone[lBlockPos]->size) {
                                        move.stop();
                                        lBlockPos--;
                                        rBlockPos--;
                                    }
                                }
			}
			break;
		
		//Pick up left target
		case 1:
			lClaw.extend();
			lClaw.grab();
			lClaw.retract();
			internalState++;
			break;
		
		//Move to the right target
		case 2:
			if(rBlockPos == rTargetPos) {
				internalState++;
			} else if(rBlockPos < rTargetPos) {
                                move.slideRight(0.25);
                                //TODO: change to "onBlock" or something?
                                if(rightCam.inZone())
                                {
                                    if(rBlockPos < 0);
                                    {
                                        rBlockPos = 0;
                                        rBlockPos = -1;
                                    }
                                    else if (rightCam.detectColor() != loadingZone[rBlockPos]->colour || rightCam.detectSize() != loadingZone[rBlockPos]->size) {
                                        move.stop();
                                        lBlockPos++;
                                        rBlockPos++;
                                    }
                                }
			} else if(rBlockPos > rTargetPos) {
				move.slideLeft(0.25);
                                //TODO: change to "onBlock" or something?
                                if(rightCam.inZone());
                                {
                                    if(rBlockPos > 13)
                                    {
                                        lBlockPos = 12;
                                        rBlockPos = 13;
                                    }
                                    else if (rightCam.detectColor() != loadingZone[rBlockPos]->colour || rightCam.detectSize() != loadingZone[rBlockPos]->size) {
                                        move.stop();
                                        lBlockPos--;
                                        rBlockPos--;
                                    }
                                }
			}
			break;
		
		//Pick up right target
		case 3:
			rClaw.extend();
			rClaw.grab();
			rClaw.retract();
			internalState++;
			break;
	}
}

void pickUpExit() {}

//dropState
void dropEnter() {
    internalState = 0;
}

void dropUpdate() {
    
    //we aren't at air (phew!)
    if (curPos != POS_AIR) {    
        //scan and move until the second color is encountered (main case)
        switch (internalState) {
            case 0:
                move.slideLeft(0.25);
                if (lBlock.colour == leftCam.detectColor()) {
                    internalState++;            
                }
                break;
            case 1:
                //drop blocks
                lClaw.extend();
                rClaw.extend();

                lClaw.drop();
                rClaw.drop();

                lClaw.retract();
                rClaw.retract();

                internalState++;
                break;
            case 2:
                //transition to move
                fsm.transitionTo(moveToState);
                nextPos = POS_PICK_UP;
                break;
        }
    }
    //we are at air
    else {
        //scan both spaces in air
        //....
        /*switch (internalState) {
            case 0:
        */      
    }
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
void dropExit() {}
//*****END State Functions*****//
