#ifndef _COMMONS_H_
#define _COMMONS_H_

/*
 * This is where all globals and constants should be defined
*/

enum POSITION{	POS_START = 0,
				POS_SEA_LOAD,
				POS_RAIL_LOAD,
				POS_PICK_UP,
				POS_AIR
	      	};

enum COLOR{}; //TODO: Colors Should be defined.
enum SIZE{}; //TODO: Sizes of the blocks.

POSITION curPos = 0;
POSITION nextPos = 0;

struct block {
	COLOR color; 
	SIZE size; 
}

struct block LClaw; //Block held by the left claw.
struct block RClaw; //Block held by the right claw.

struct block LoadingZone[14]; // Blocks in the loading zone, listed west to east.
COLOR seaZone[6]; //Sea zone colors, listed south to north.
COLOR railZone[6]; //Rail zone colors, listed west to east. 

/* PIN DEFINITIONS */
#define COLOR_SENSOR 0 //Pin for color sensor (pin 0 is fake) 
	//Motors (Servos)
#define LEFT_MOTOR 9
#define RIGHT_MOTOR 10
#define REAR_MOTOR 11

#endif
