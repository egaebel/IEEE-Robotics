#include "SimpleFunctions.h"

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
  
}
