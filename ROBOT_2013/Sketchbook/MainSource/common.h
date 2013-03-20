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

typedef struct block_t {
	COLOR color; 
	SIZE size; 
	bool present;
} Block;

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
#define COLOR_SENSOR_PIN_S0 22
#define COLOR_SENSOR_PIN_S1 24 //Pin for color sensor (pin 0 is fake) 
#define COLOR_SENSOR_PIN_S2 26
#define COLOR_SENSOR_PIN_S3 28
#define COLOR_SENSOR_PIN_OUT 30
#define COLOR_SENSOR_PIN_LED 32 

//Color Sensor Values
#define white_AveragePV_RED	2900
#define blue_AveragePV_RED 2900
#define red_AveragePV_RED 890
#define green_AveragePV_RED 2920
#define range_RED 100

#define white_AveragePV_BLUE 2000
#define blue_AveragePV_BLUE 2000
#define red_AveragePV_BLUE 2470
#define green_AveragePV_BLUE 2000
#define range_BLUE 100

#define white_AveragePV_BROWN 3300
#define blue_AveragePV_BROWN 3250
#define red_AveragePV_BROWN 2500
#define green_AveragePV_BROWN 3250
#define range_BROWN 150

#define white_AveragePV_YELLOW 700
#define blue_AveragePV_YELLOW 700
#define red_AveragePV_YELLOW 470
#define green_AveragePV_YELLOW 700
#define range_YELLOW 50

#define white_AveragePV_PURPLE 2265
#define blue_AveragePV_PURPLE 2250
#define red_AveragePV_PURPLE 1900
#define green_AveragePV_PURPLE 2250
#define range_PURPLE 100

#define white_AveragePV_GREEN 3100
#define blue_AveragePV_GREEN 3100
#define red_AveragePV_GREEN 3000
#define green_AveragePV_GREEN 3100
#define range_GREEN 200

#endif