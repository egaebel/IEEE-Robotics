#ifndef SIMPLE_FUNCTIONS_H_
#define SIMPLE_FUNCTIONS_H_

#include "common.h"
#include "Movement.h"
#include "cam.h"
#include "Sonar.h"

typedef struct block_t Block;

bool fullOfBlocks(Block blocks[], int numBlocks);
bool getBayPos(int distance, int safeDistance, int * lBlockPos, int * rBlockPos);
bool goToWall();
bool centerBay(side strafeDir, bPosition pos ,side );
Block* getZoneByPos(bPosition pos, Block * seaZone, Block * railZone, Block * loadingZone);
bool goToBay(bPosition, int nBay, side clawSide);
int getBayDist(bPosition,int,side);
#endif
