#include "SimpleFunctions.h"

bool fullOfBlocks(Block *blocks[], int numBlocks) {

	for (int i = 0; i < numBlocks; i++) {

		if (blocks[i] == 0) {

			return false;
		} 
	}
	return true;
}