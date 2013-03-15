#include "common.h"


block LClaw; //Block held by the left claw.
block RClaw; //Block held by the right claw.

block loadingZone[14]; // Blocks in the loading zone, listed west to east.
bColour seaZone[6]; //Sea zone colors, listed south to north.
bColour railZone[6]; //Rail zone colors, listed west to east. 

POSITION curPos = (POSITION)0;
POSITION nextPos = (POSITION)0;