#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>

//Define the Pins----------
//Motor Pair 1 (ON THE LEFT SIDE)
/*
#define PIN_DIRECTION_LEFT 26 //Direction
#define PIN_PWM_LEFT 3 //Speed

//Motor Pair 2 (ON THE RIGHT SIDE)
#define PIN_DIRECTION_RIGHT 28 //Direction
#define PIN_PWM_RIGHT 4 //Speed
*/

//Enum for specifying direction to move in
enum Direction { FORWARD = 1, BACKWARD };

//Enum for specifying the direction to turn in
enum Turn {	LEFT = 1, RIGHT };

//~Class for moving all of the motors at once
class Motors
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
		Motors(unsigned short pwmPinLeft, 
					unsigned short directionPinLeft, 
					unsigned short pwmPinRight, 
					unsigned short directionPinRight, 
					unsigned int defaultSpeed);
		void motorsDrive(Direction motorsDirection);
		void motorsStop();
		void motorsTurnLeft();
		void motorsTurnRight();
		void motorsTurn(Turn motorsTurn);
		void motorsTurn(short leftPWM, short rightPWM);
		void motorsUTurn();
		int speed;
		bool flipDirection;
	private:
		void motorsTurn(short leftPWM, short rightPWM, Turn motorsTurn);
};
#endif