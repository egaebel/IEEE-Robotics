#ifndef _COMMONS_H_
#define _COMMONS_H_

/*
 * This is where all globals and constants should be defined
*/

enum POSITIONS{	POS_START = 0,
				POS_SEA_LOAD,
				POS_RAIL_LOAD,
				POS_PICK_UP,
				POS_AIR
	      	};

enum COLOR{}; //TODO: Colors Should be defined.

int curPos = 0;
int nextPos = 0;

/* PIN DEFINITIONS */
#define COLOR_SENSOR 0 //Pin for color sensor (pin 0 is fake) 
	//Motors (Servos)
#define LEFT_MOTOR 9
#define RIGHT_MOTOR 10
#define REAR_MOTOR 11

#endif
