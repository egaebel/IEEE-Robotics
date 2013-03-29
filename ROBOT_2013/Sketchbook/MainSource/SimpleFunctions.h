#ifndef SIMPLE_FUNCTIONS_H_
#define SIMPLE_FUNCTIONS_H_

#include "common.h"

typedef struct block_t Block;

bool fullOfBlocks(Block blocks[], int numBlocks);
bool getBayPos(Block blocks[], int numBlocks, COLOR leftColor, COLOR rightColor, int * lBlockPos, int * rBlockPos);

#endif