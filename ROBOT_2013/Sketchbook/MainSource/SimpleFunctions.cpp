#include "SimpleFunctions.h"
#include "IRlib.h"
extern Movement move;
extern cam rightCam;
extern cam leftCam;
extern Sonar sonarRight;
extern Sonar sonarLeft;
extern IRAverager irLeft;
extern IRAverager irRight;

void updateBayBool(Block * blocks, int size, bool * done) {

	*done = fullOfBlocks(blocks, size);
}

bool fullOfBlocks(Block blocks[], int numBlocks) {

	for (int i = 0; i < numBlocks; i++) {

		if (!blocks[i].present) {

			return false;
		} 
	}
	return true;
}

bool centerBay(side strafeDir, bPosition pos, side sCam){
	cam *c;
	if(sCam==RIGHT)
		c = &rightCam;
	else
		c = &leftCam;
		
	if(goToWall()){
		switch(c->inZone(pos))  {
			case CENTER:
				move.stop();
				return true;
    		case LEFT:
    			move.setSpeed(0,.25,.05,.05); //SLOW
    			break;
    		case RIGHT:
    			move.setSpeed(.25,0,-.05,-.05); //SLOW
    			break;
    		case NO_SIDE:
    			move.slideWall(strafeDir);
    			break;
    	}
    }
    return false;
}

bool goToWall()
{
  	if(digitalRead(BUMPER_L) && digitalRead(BUMPER_R)){
		return true;
	}
    else if( digitalRead(BUMPER_L)){
      	move.setSpeed(0,.25,0,-1);
    }
    else if( digitalRead(BUMPER_R)){
      	move.setSpeed(.25,0,1,0);
    }
    else{
       	move.forward(.1);
    }

    return false;
}

Block* getZoneByPos(bPosition pos, Block * seaZone, Block * railZone, Block * loadingZone) {

	if (pos == POS_SEA) {
		return seaZone;
	}
	else if (pos == POS_RAIL) {
		return railZone;
	}
	else if (pos == POS_PICK_UP) {
		return loadingZone;
	}
	else {
		return NULL;
	}
}

bool goToBay(bPosition bay, int nBay, side clawSide) {
	if(goToWall()){
		
		Sonar *tempSonar;
		side sideRef;


		if(bay==POS_SEA || bay == POS_PICK_UP || bay==POS_RAIL){
			sideRef = LEFT;
			tempSonar = &sonarLeft;
		}
		else{
			sideRef = RIGHT;
			tempSonar = &sonarRight;
		}

		int curDist = tempSonar->getDistance();

		if(bay==POS_PICK_UP){
			//if(analogRead(LEFT_IR)>295)
				//curDist = 20;
			curDist = irLeft.getIR();
		}
		
		else if(bay==POS_RAIL){
			if(irRight.getIR()<75){
				//curDist = irRight.getIR();
				//sideRef = RIGHT;
				curDist = 999;
			}
		}
		
		int dist = getBayDist(bay,nBay,clawSide,sideRef);

		if(curDist > dist){

			if(sideRef == LEFT)
				move.slideWall(LEFT);
			else
				move.slideWall(RIGHT);
			return false;
		}
		else if(curDist < dist){
			if(sideRef == LEFT)
				move.slideWall(RIGHT);
			else
				move.slideWall(LEFT);
			
			return false;
		}
		else{
			move.stop();
			return true;
		}
	}

}

int getBayDist(bPosition bay, int nBay, side clawSide, side refSide) {
	int dist;
	int clawAddition = 0;
	if(clawSide == LEFT)
		clawAddition = CLAW_CENTER_DISTANCE;

	switch(bay){
		case POS_PICK_UP:
			if(refSide==LEFT)
				dist = 24 + (nBay*7) + clawAddition;
			break;
		case POS_RAIL:
			//dist = 142 - (nBay*7) + clawAddition;
			if(refSide==LEFT)
				dist = 68 + (nBay*7) + clawAddition;
			else
				dist = 140 - (nBay*7) - clawAddition; //TODO: 170 prolly not good enough
			break;
		case POS_SEA:
			if(refSide==LEFT)
				dist =  33 + (nBay*7) + clawAddition;
			break;
	}
	
	return dist;
}

/* Takes in each Block array and sets the loadPos values in the sea and rail
   to their respective bay values of the blocks in loading. */
void calculateBlockTargets(Block * loading, Block * sea, Block * rail) {

	Block * temp1;
	Block * temp2;
	for (int i = 0; i < 6; i++) {

		temp1 = &sea[i];
		temp2 = &rail[i];
		for (int k = 0; k < 14; k++) {

			if (loading[k] == *temp1) {

				temp1->loadPos = k;
			}
			else if (loading[k] == *temp2) {

				temp2->loadPos = k;
			}
		}
	}
}