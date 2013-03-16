#ifndef _COMMON_H_
#define _COMMON_H_

//#include <Arduino.h>

/*
 * This is where all globals and constants should be defined
*/
//Enumerations
enum POSITION{	
	POS_START = 0,
	POS_SEA,
	POS_RAIL,
	POS_PICK_UP,
	POS_AIR
};

enum COLOR{
    WHITE = 0,
    BLUE = 1,
    RED = 2,
    GREEN = 3
}; //TODO: There are still some colors to be defined, waiting on color sensor defs

enum SIZE{
	SMALL, 
	MEDIUM, 
	LARGE
};

typedef struct block_t {
	COLOR color; 
	SIZE size; 
	bool present;
} Block;


/* PIN DEFINITIONS */ //max pin is 53!
/*	//Motors (Servos)
#define LEFT_MOTOR 13
#define RIGHT_MOTOR 12
#define REAR_MOTOR 11 //Used for setSpeed
#define REAR_MOTOR_L 11
#define REAR_MOTOR_R 10
#define TOP_MOTOR 2
	//Claw Servos
#define LCLAW_SERVO 9
#define RCLAW_SERVO 8
#define LCLAW_EXTEND_SERVO 7
#define RCLAW_EXTEND_SERVO 6
	//Wallfolloer servos
#define WALL_FOLLOW_SERVO 5
#define BUMPER_2_PIN 4
#define BUMPER_1_PIN 3
#define COLOR_SENSOR 0 //Pin for color sensor (pin 0 is fake) 
*/

#endif