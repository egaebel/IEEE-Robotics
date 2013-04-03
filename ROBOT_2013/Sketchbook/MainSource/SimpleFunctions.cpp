#include "SimpleFunctions.h"

extern Movement move;
extern cam rightCam;
extern Sonar sonarRight;
extern Sonar sonarLeft;
bool fullOfBlocks(Block blocks[], int numBlocks) {

	for (int i = 0; i < numBlocks; i++) {

		if (!blocks[i].present) {

			return false;
		} 
	}
	return true;
}

/* Sets lBlockPos and rBlockPos if we are within the safeDistance, returns false if we are not, true if we are*/
bool getBayPos(int distance, int safeDistance, int * lBlockPos, int * rBlockPos) {

	bool haveInfo = false;

	//check if we're past the beginning of the begin rail zone
	if (distance >= safeDistance) {

		haveInfo = true;
		distance -= safeDistance;
		distance /= SONAR_BLOCK_WIDTH;
		*rBlockPos = distance;
		*lBlockPos = *lBlockPos + 1;
	}

	return haveInfo;
}

bool centerBay(side strafeDir, bPosition pos, cam* c){
	if(goToWall()){
		switch(c->inZone(pos))  {
			case CENTER:
				move.stop();
				return true;
    		break;
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
  	if(digitalRead(53)&&digitalRead(52)){
		return true;
	}
    else if( digitalRead(53)){
      	move.setSpeed(0,.25,0,-1);
    }
    else if( digitalRead(52)){
      	move.setSpeed(.25,0,1,0);
    }
    else{
       	move.forward(0.10);
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
	Sonar *tempSonar;
	side posSide;
	int dist;
	switch(bay){
		case POS_PICK_UP:
			tempSonar = &sonarRight;
			dist = 122 - (nBay*7);
			break;
		case POS_RAIL:
			tempSonar = &sonarRight;
			dist = 104 - (nBay*7);
			break;
		case POS_SEA:
			tempSonar = &sonarLeft;
			dist =  37 + (nBay*7);
			break;
	}
	if(tempSonar->getDistance()>dist){
		if(bay==POS_SEA)
			move.slideWall(LEFT);
		else
			move.slideWall(RIGHT);
		return 0;
	}
	else if(tempSonar->getDistance()<dist){
		if(bay==POS_SEA)
			move.slideWall(RIGHT);
		else
			move.slideWall(LEFT);
		return 0;
	}
	else
		return 1;

}
