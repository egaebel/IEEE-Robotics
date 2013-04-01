#include "States.h"

extern FiniteStateMachine fsm;

//Hardware classes----
static Movement move;

static cam leftCam = cam(LCAM_PIN);
static cam rightCam = cam(RCAM_PIN);

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
            if(goToWall(&move)){
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
            //scanning sea (state transition 2 in state diagram)
            if (curPos == POS_SEA) {

                move.slideRight(0.25);

                //focused on bay, read colour
                if(rightCam.inZone()){
                    move.stop();             
                    loadingZone[rBlockPos].colour = rightCam.getBlockColour(); 
                    loadingZone[rBlockPos].size = rightCam.getBlockSize(loadingZone[rBlockPos].colour);
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

                //if we're focused on a bay, read colour
                if (leftCam.inZone()) {
                    
                    move.stop();
                    loadingZone[lBlockPos].colour = leftCam.getBlockColour(); 
                    loadingZone[lBlockPos].size = leftCam.getBlockSize(loadingZone[lBlockPos].colour);
                    lBlockPos++;
                    if(lBlockPos > 5){
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
                    loadingZone[rBlockPos].colour = rightCam.getBlockColour(); 
    	            loadingZone[rBlockPos].size = rightCam.getBlockSize(loadingZone[rBlockPos].colour);
                    loadingZone[rBlockPos].present = true;
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
        //We already read this colour, so just keep moving until white
        case 1:
            //move right
            if (curPos == POS_SEA || curPos == POS_PICK_UP) {
                move.slideRight(0.25);
                if(rightCam.betweenZones()){
                    move.stop();
                    internalState = 0;
                }
            }
            //move left
            else {
                move.slideLeft(0.25);
                if(leftCam.betweenZones()){
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

//moveToState
void moveToEnter() {

    //initialize necessary variables & sensors
    internalState = 0;
}

//Handles all the movement, set curPos and nextPos before entering
void moveToUpdate() {

    //Start to sea (1 in state diagram) 
    if(curPos == POS_START && nextPos == POS_SEA){
        switch(internalState) {
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
                if (goToWall(&move)) {
                    move.stop();
                    internalState++;
                }
                break;
            //ensure we are the start of the zone
            case 3:
                move.slideLeft(0.1);
                //we want to be a certain distance from the back wall
                if (rightCam.inZone() && (sonarRight.getDistance() == SEA_START_LEFT_DIST)) {
                    move.stop();
                    rBlockPos = 0;
                    lBlockPos = -1;
                    internalState++;
                }
                break;
            //slide into position
            case 4:
                move.slideRight(0.25);
                //make it so the cams can both read a zone
                if (rightCam.inZone() 
                    /*&& leftCam.inZone()*/) {
                    move.stop();
                    rBlockPos++;
                    lBlockPos++;
                    internalState++;
                }
                break;
            //slide two spaces
            case 5:
                move.slideRight(0.1);
                if (rightCam.inZone()/* && leftCam.inZone() */
                    && leftCam.getBayColour() != loadingZone[rBlockPos].colour) {
                    move.stop();
                    rBlockPos += 2;
                    lBlockPos += 2;
                    internalState++;
                }
                break;
            //transition to scan
            case 6:
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
                if(goToWall(&move)) {
                    move.stop();
                    internalState++;
                }
                break;
            //move to the right of the zone
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
            //move to wall
            case 3:
                if (goToWall(&move)) {
                    internalState++;
                }
                break;
            //center OR slide Left
            case 4:
                //center over block or sector (if not off the edge)
                //if centered internalState += 2;
                //if not over zone internalState++;
                break;
            //not in the zone fully, need to slide
            case 5:
                //moving left
                move.slideLeft(0.1);
                //if both cameras are in zones
                if (leftCam.inZone() && rightCam.inZone()) {
                    internalState--;
                }
                break;
            //determine position
            case 6:
                //returns if the robot is in a bay (true if it is, false if not) sets lBlockPos and rBlockPos if over bay
                if (getBayPos(seaZone, RAIL_SEA_SIZE, 
                        leftCam.getBayColour(), rightCam.getBayColour(), 
                        &lBlockPos, &rBlockPos)) {
                    internalState++;
                }
                break;
            case 7:
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
            case 0:
                //check if we are safe to turn around
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
            case 1:
                move.backward(0.1);
                internalState++;
                break;
            //turn right
            case 2:
                move.turnRight(0.1);
                internalState++;
                break;
            //move to wall
            case 3:
                if (goToWall(&move)) {
                    internalState++;
                }
                break;
            case 4: 
                move.slideRight(0.25);
                if (rightCam.betweenZones()) {
                    move.stop();
                    internalState++;
                }
                break;
            case 5:
                //center over block or sector (if not off the edge)
                //if centered internalState += 2;
                //if not over zone internalState++;
                break;
            //
            case 6:
                //moving left
                move.slideLeft(0.1);
                //if both cameras are in zones (make possible....)
                if (leftCam.inZone() /*&& rightCam.inZone()*/) {
                    internalState--;
                }
                break;
            //determine position
            case 7:
                //returns if the robot is in a bay (true if it is, false if not) sets lBlockPos and rBlockPos if over bay
                if (getBayPos(seaZone, RAIL_SEA_SIZE, leftCam.getBayColour(), rightCam.getBayColour(), &lBlockPos, &rBlockPos)) {
                    internalState++;
                }
                break;
            case 8:
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
                if(goToWall(&move)) {
                    move.stop();
                    internalState++;
                }
                break;
            //figure out whether to go left or right based on location in rail
            case 3:
                //returns if the robot is in a bay (true if it is, false if not) sets lBlockPos and rBlockPos if over bay
                if (getBayPos(seaZone, RAIL_SEA_SIZE, leftCam.getBayColour(), rightCam.getBayColour(), &lBlockPos, &rBlockPos)) {
                    internalState++;
                }
                //to the left of the sea zone
                else {
                    internalState += 2;
                }
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
                    curPos = nextPos;
                    nextPos = POS_AIR;
                    fsm.transitionTo(moveToState);
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
                if (goToWall(&move)) {
                    internalState++;
                }
                break;
            case 4:
                 //returns if the robot is in a bay (true if it is, false if not) sets lBlockPos and rBlockPos if over bay
                if (getBayPos(seaZone, RAIL_SEA_SIZE, leftCam.getBayColour(), rightCam.getBayColour(), &lBlockPos, &rBlockPos)) {
                    internalState++;
                }
                //to the left of the sea zone
                else {
                    internalState += 2;
                }
                break;
            //TODO: we may need to add a case in case the robot goes ridiculously diagonal to the right
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

//pickUpState
void pickUpEnter() {
    internalState = 0;

  	//Figure out which blocks you need to pick up
	if(!seaDone) {
		for(int i = 0; i < 6; i++) {
			if(!railZone[i].present == false) {
				lTargetBlock = railZone[i];
				rTargetBlock = railZone[++i];
                break;
			}
		}
	}
	else if(!railDone) {
		for(int i = 0; i < 6; i++) {
			if(seaZone[i].present == false) {
				lTargetBlock = seaZone[i];
				rTargetBlock = seaZone[++i];
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

    Block * blocks = getZoneByPos(curPos, seaZone, railZone, loadingZone);

    //Set target positions of blocks in loading
    for(int i = 0; i < 14; i++) {
        if(blocks[i].colour == lBlock.colour) {
            lTargetPos = i;
        }
        else if(blocks[i].colour == rTargetBlock.colour) {
            rTargetPos = i;
        }
    }
}

void dropUpdate() {
    
    //we aren't at air (phew!)
    if (curPos != POS_AIR) {    
        //scan and move until the second colour is encountered (main case)
        switch (internalState) {
            case 0:
                move.slideLeft(0.25);
                if (lBlock.colour == leftCam.getBayColour()) {
                    internalState++;            
                }
                break;
            case 1:
                //drop blocks
                move.extendClaw(LEFT);
                move.openClaw(LEFT);
                move.retractClaw(LEFT);
                move.closeClaw(LEFT);

                move.extendClaw(RIGHT);
                move.openClaw(RIGHT);
                move.retractClaw(RIGHT);
                move.closeClaw(RIGHT);

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
        switch (internalState) {
            case 0:
                  break;
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
