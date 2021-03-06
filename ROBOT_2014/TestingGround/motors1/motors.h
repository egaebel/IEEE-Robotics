#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>

//Define the Pins----------
//Motor Pair 1 (ON THE LEFT SIDE)
#define PIN_DIRECTION_LEFT 8 //Direction
#define PIN_PWM_LEFT 3 //Speed

//Motor Pair 2 (ON THE RIGHT SIDE)
#define PIN_DIRECTION_RIGHT 9 //Direction
#define PIN_PWM_RIGHT 4 //Speed

//Enum for specifying direction to move in
enum Direction { FORWARD = 1, BACKWARD };

//Enum for specifying the direction to turn in
enum Turn {	LEFT = 1, RIGHT };

//~Class for moving all of the motors at once
class motors
{
	//==================================================
	//~~PIN INFORMATION=================================
	//==================================================
	/*
		Connections:
		- Pin 3 ---> PWM for left motors
		- Pin 8 ---> PIN_DIRECTION_LEFT

		- Pin 4 ---> PWM for right motors
		- Pin 9 ---> PIN_DIRECTION_RIGHT
	*/
	//==================================================

	public:
		void setup(int defaultSpeed);
		void motorsDrive(Direction motorsDirection);
		void motorsStop();
		void motorsStop();
		void motorsTurnLeft();
		void motorsTurnRight();
		int speed;
	private:
		void motorsTurn(Turn motorsTurn);
};
#endif