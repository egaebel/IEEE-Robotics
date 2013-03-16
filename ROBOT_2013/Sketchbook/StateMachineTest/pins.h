#ifndef PINS_H_
#define PINS_H_

/* PIN DEFINITIONS */ //max pin is 53!

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
#define COLOR_SENSOR 0 //Pin for color sensor (pin 0 is fake)

#endif