#ifndef _COMMON_H_
#define _COMMON_H_

/*
 * This is where all globals and constants should be defined
*/

enum POSITION{	POS_START = 0,
				POS_SEA_LOAD,
				POS_RAIL_LOAD,
				POS_PICK_UP_RIGHT,
				POS_PICK_UP_LEFT,
				POS_AIR
	      	};

enum COLOR{}; //TODO: Colors Should be defined.
enum SIZE{}; //TODO: Sizes of the blocks.

POSITION curPos = (POSITION)0;
POSITION nextPos = (POSITION)0;

typedef struct block_t {
	COLOR color; 
	SIZE size; 
} block;

block LClaw; //Block held by the left claw.
block RClaw; //Block held by the right claw.

block LoadingZone[14]; // Blocks in the loading zone, listed west to east.
COLOR seaZone[6]; //Sea zone colors, listed south to north.
COLOR railZone[6]; //Rail zone colors, listed west to east. 

/* PIN DEFINITIONS */
#define COLOR_SENSOR 0 //Pin for color sensor (pin 0 is fake) 
	//Motors (Servos)
#define LEFT_MOTOR 13
#define RIGHT_MOTOR 12
#define REAR_MOTOR 11
	//Claw Servos
#define LCLAW_SERVO 10
#define RCLAW_SERVO 9
#define LCLAW_EXTEND_SERVO 8
#define RCLAW_EXTEND_SERVO 7
	//Wallfolloer servos
#define WALL_FOLLOW_SERVO 6

#endif
