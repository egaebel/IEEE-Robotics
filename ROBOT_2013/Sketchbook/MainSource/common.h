#ifndef _COMMON_H_
#define _COMMON_H_

//Sonar Distances
#define SEA_START_LEFT_DIST 0
#define SEA_START_RIGHT_DIST 0
#define SONAR_BLOCK_WIDTH 6
#define SEA_SAFE_ZONE 0
#define PICKUP_START_LEFT_DIST 0
#define CLAW_CENTER_DISTANCE 8
//#define 
//in centimeters
#define BAY_WIDTH 6.5
#define BAY_DEPTH 15.5
#define LINE_WIDTH 1 

#define AIR_BLOCK 5
#define SEA_BLOCK 7.5
#define RAIL_BLOCK 10
#define BLOCK_WIDTH 4

/*
 * This is where all globals and constants should be defined
*/

typedef enum POSITION {	
	POS_START = 0,
	POS_SEA,
	POS_RAIL,
	POS_PICK_UP,
	POS_AIR
} bPosition;

typedef enum bColour_t {	
	WHITE=0,//LEAVE WHITE AS 0
	YELLOW,
	RED,
	PURPLE,
	BLUE,
	BROWN,
	GREEN,
	BLACK//here so we can iterate white to black
} bColour;

typedef enum SIZE{
	SMALL, 
	MED, 
	LARGE
} bSize;

typedef enum side_t{
	LEFT,
	RIGHT,
	CENTER,
	NO_SIDE
} side;

typedef struct block_t {
	bColour colour; 
	bSize size; 
    bool present;
    int loadPos;
    bool operator == (const block_t & other) const {
    	if (colour == other.colour) {
    		if (size == other.size) {
    			return true;
    		}
    	}
    	return false;
    }
} Block;

/* PIN DEFINITIONS */
	//Motors (Servos)
#define MOTOR_FRONT_L 11
#define MOTOR_FRONT_R 12
#define MOTOR_BACK_L 10
#define MOTOR_BACK_R 13
	//Claw Servos
#define LCLAW_SERVO 4
#define RCLAW_SERVO 5
#define LCLAW_EXTEND_SERVO 6
#define RCLAW_EXTEND_SERVO 7

#define LEFT_CLAW_BUMPER 23
#define RIGHT_CLAW_BUMPER 22

	//Top Motors
#define TREB_LEFT_SERVO 2
#define TREB_RIGHT_SERVO 3

#define RIGHT_TRACK_BUMPER_IN 22

	//CMU Cams
#define LCAM_PIN 2
#define RCAM_PIN 3
	//IR

#define LEFT_FOR_IR 3
#define LEFT_BACK_IR 2
#define RIGHT_BACK_IR 7
#define RIGHT_FOR_IR 8


#define IR_BLOCK_THRES 470

#define RIGHT_IR 5
#define LEFT_IR	4


	//Bumpers
#define BUMPER_L 53
#define BUMPER_R 52
    //Sonar - Addresses
#define SONAR_LEFT 0x34 //0x70
#define SONAR_RIGHT 0x36 //0x70
    //Sonar - Interrupts
#define SONAR_LEFT_INT 4  //PIN 19
#define SONAR_RIGHT_INT 5 //PIN 18

	//Air State Speeds
#define VERY_SLOW 0.05 //When threat of falling of ramp is high
#define SLOW 0.1 
#define MEDIUM 0.25
#define FAST 0.5 
#define VERY_FAST 0.75

#endif
