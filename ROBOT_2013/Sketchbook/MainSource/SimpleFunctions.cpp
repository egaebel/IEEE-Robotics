#include "SimpleFunctions.h"

extern Movement move;
extern cam rightCam;
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

/** Used only in Air state 
 *  If the front two IR sensors is hanging of the edge of the elevated ramp/air area
 */
bool frontHangingOffEdge()  {
	/* !!! */	/* !!! */	/* !!! */	/* !!! */	/* !!! */
	//TO BE IMPLEMENTED (USE IR TO DETERMINE DISTANCE TO GROUND)
	//TO BE IMPLEMENTED (USE IR TO DETERMINE DISTANCE TO GROUND)
	//TO BE IMPLEMENTED (USE IR TO DETERMINE DISTANCE TO GROUND)
	
	return false;
}

/** Used only in Air state
 *  If the left IR sensor is hanging of the edge of the elevated ramp/air area
 */
bool leftIRHangingOffEdge()  {
	/* !!! */	/* !!! */	/* !!! */	/* !!! */	/* !!! */
	//TO BE IMPLEMENTED (USE IR TO DETERMINE DISTANCE TO GROUND)
	//TO BE IMPLEMENTED (USE IR TO DETERMINE DISTANCE TO GROUND)
	//TO BE IMPLEMENTED (USE IR TO DETERMINE DISTANCE TO GROUND)
	
	return false;
}

bool rightIRHangingOffEdge()  {
	/* !!! */	/* !!! */	/* !!! */	/* !!! */	/* !!! */
	//TO BE IMPLEMENTED (USE IR TO DETERMINE DISTANCE TO GROUND)
	//TO BE IMPLEMENTED (USE IR TO DETERMINE DISTANCE TO GROUND)
	//TO BE IMPLEMENTED (USE IR TO DETERMINE DISTANCE TO GROUND)
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

