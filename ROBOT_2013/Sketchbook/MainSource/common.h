#ifndef _COMMON_H_
#define _COMMON_H_

/*
 * This is where all globals and constants should be defined
*/

typedef enum POSITION{	POS_START = 0,
				POS_SEA,
				POS_RAIL,
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

typedef enum SIZE{
	SMALL, 
	MEDIUM, 
	LARGE
} bSize;

typedef struct block_t {
	bColour colour; 
	bSize size; 
        bool present;
} Block;

/* PIN DEFINITIONS */
#define COLOR_SENSOR 0 //Pin for color sensor (pin 0 is fake) 
	//Motors (Servos)
#define MOTOR_FRONT_L 11
#define MOTOR_FRONT_R 12
#define MOTOR_BACK_L 10
#define MOTOR_BACK_R 13
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
#define COLOR_SENSOR_PIN_S0 22
#define COLOR_SENSOR_PIN_S1 24 //Pin for color sensor (pin 0 is fake) 
#define COLOR_SENSOR_PIN_S2 26
#define COLOR_SENSOR_PIN_S3 28
#define COLOR_SENSOR_PIN_OUT 30
#define COLOR_SENSOR_PIN_LED 32 

#define BUMPER_L 53
#define BUMPER_R 52
#endif
