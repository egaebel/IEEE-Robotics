#include "SimpleFunctions.h"

extern Movement move;
bool fullOfBlocks(Block blocks[], int numBlocks) {

	for (int i = 0; i < numBlocks; i++) {

		if (!blocks[i].present) {

			return false;
		} 
	}
	return true;
}

//blocks should not have ANY NULL elements
//returns false if no information was obtained
bool getBayPos(Block blocks[], int numBlocks, bColour leftColor, bColour rightColor, int * lBlockPos, int * rBlockPos) {

	bool haveInfo = false;

	for (int i = 0; i < numBlocks; i++) {

		if (blocks[i].colour == rightColor) {

			*rBlockPos = i;
			haveInfo = true;
		}
		else if (blocks[i].colour == leftColor) {

			*lBlockPos = i;
			haveInfo = true;
		}
	}

	return haveInfo;
}
bool goToWall()
{
  	if(digitalRead(53)&&digitalRead(52)){
		move.stop();
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
