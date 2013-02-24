#ifndef _COMMON_H_
#define _COMMON_H_

#include <Arduino.h>
#include <Servo.h>
#include "LineSensor.h"
#include "Claw.h"

/*
 * This is where all globals and constants should be defined
*/
//Enumerations
enum POSITION{	
	POS_START = 0,
	POS_SEA,
	POS_RAIL,
	POS_LOAD,
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

//Structs
POSITION curPos = POS_START;
POSITION nextPos;

typedef struct block_t {
	COLOR color; 
	SIZE size; 
	bool present;
} Block;

//Variables
Block lBlock; //Block held by the left claw.
Block rBlock; //Block held by the right claw.

Block *loadingZone[14]; // Blocks in the loading zone, listed west to east.
Block *seaZone[6]; //Sea zone colors, listed south to north.
Block *railZone[6]; //Rail zone colors, listed west to east. 

const int PICKUP_SIZE = 14;
const int RAIL_SEA_SIZE = 6;
const int AIR_SIZE = 2;

/* PIN DEFINITIONS */
	//Motors (Servos)
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
	//Top Motor
#define COLOR_SENSOR 0 //Pin for color sensor (pin 0 is fake) 

#endif