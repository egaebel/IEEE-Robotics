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
bool getBayPos(Block blocks[], int numBlocks, COLOR leftColor, COLOR rightColor, int * lBlockPos, int * rBlockPos) {

	bool haveInfo = false;

	for (int i = 0; i < numBlocks; i++) {

		if (blocks[i].color == rightColor) {

			*rBlockPos = i;
			haveInfo = true;
		}
		else if (blocks[i].color == leftColor) {

			*lBlockPos = i;
			haveInfo = true;
		}
	}

	return haveInfo;
}