#ifndef SIMPLE_FUNCTIONS_H_
#define SIMPLE_FUNCTIONS_H_

#include "common.h"
#include "Movement.h"
#include "cam.h"
typedef struct block_t Block;

bool fullOfBlocks(Block blocks[], int numBlocks);
bool getBayPos(Block blocks[], int numBlocks, bColour leftColor, bColour rightColor, int * lBlockPos, int * rBlockPos);
bool goToWall();
bool centerBay(side strafeDir, bay b);
bool frontHangingOffEdge();  //If front 2 IR's hanging off elevated part
bool rightIRHangingOffEdge(); //If right IR hanging off elevated part
bool leftIRHangingOffEdge(); //If left IR hanging off elevated part
Block* getZoneByPos(bPosition pos, Block * seaZone, Block * railZone, Block * loadingZone);
#endif
