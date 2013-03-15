#ifndef _COMMON_H_
#define _COMMON_H_

/*
 * This is where all globals and constants should be defined
*/

typedef enum POSITION{	POS_START = 0,
				POS_SEA_LOAD,
				POS_RAIL_LOAD,
				POS_PICK_UP,
				POS_AIR
	      	};


typedef enum bColour_t{	WHITE=0,//LEAVE WHITE AS 0
			BLUE,
			RED,
			GREEN,
			PURPLE,
			BROWN,
			YELLOW,
			BLACK//here so we can iterate white to black
		}bColour;

typedef enum bSize_t{	RAIL=0,
			SEA,
			AIR}bSize;

typedef struct block_t {
	bColour colour; 
	bSize size; 
} block;


/* PIN DEFINITIONS */
#define COLOR_SENSOR 0 //Pin for color sensor (pin 0 is fake) 
	//Motors (Servos)
#define LEFT_MOTOR 13
#define RIGHT_MOTOR 12
#define REAR_MOTOR 11 //Used for setSpeed
#define REAR_MOTOR_L 11
#define REAR_MOTOR_R 10
	//Claw Servos
#define LCLAW_SERVO 9
#define RCLAW_SERVO 8
#define LCLAW_EXTEND_SERVO 7
#define RCLAW_EXTEND_SERVO 6
	//Wallfolloer servos
#define WALL_FOLLOW_SERVO 5
        //Top Motor
#define TOP_MOTO 2

#endif
