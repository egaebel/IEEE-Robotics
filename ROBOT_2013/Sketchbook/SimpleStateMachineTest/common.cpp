#include "common.h"


block LClaw; //Block held by the left claw.
block RClaw; //Block held by the right claw.

block loadingZone[14]; // Blocks in the loading zone, listed west to east.
<<<<<<< HEAD
COLOR seaZone[6]; //Sea zone colors, listed south to north.
COLOR railZone[6]; //Rail zone colors, listed west to east. 
=======
bColour seaZone[6]; //Sea zone colors, listed south to north.
bColour railZone[6]; //Rail zone colors, listed west to east. 
>>>>>>> c4570bd0993087da60f6bf4bf48948322a6ad139

POSITION curPos = (POSITION)0;
POSITION nextPos = (POSITION)0;