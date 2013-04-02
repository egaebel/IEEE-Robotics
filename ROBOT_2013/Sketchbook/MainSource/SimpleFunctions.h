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
Block* getZoneByPos(bPosition pos, Block * seaZone, Block * railZone, Block * loadingZone);
#endif