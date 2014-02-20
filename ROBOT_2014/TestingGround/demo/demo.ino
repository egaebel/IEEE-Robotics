
#include "motors.h"

enum State { MAIN_LINE = 1, STRAIGHT_LINE_START, FIRE, STRAIGHT_LINE_END, CURVED_LINE };

//Hardware interfaces
static Motors motors;
static LineFollower lineFollower

//
static State state;
static byte leftLineFollowBits;
static byte rightLineFollowBits;

//Setup variables etc
void setup() {

	Serial.begin(9600);
	//init all hardware
	motors.setup(255);
	state = MAIN_LINE;
	delay(3);
}

void loop() {

	switch(state) {

		case MAIN_LINE:


			break;
		case STRAIGHT_LINE_START:
			break;
		case STRAIGHT_LINE_END:
			break;
		case FIRE:
			break;
		case CURVED_LINE:
			break;
	}
}

void followTheLine(byte leftBits, byte rightBits)
{
	short leftPWM = 0;
	short rightPWM = 0;

	if (leftBits > rightBits)
	{
		leftPWM = motors.speed / leftBits;
		rightPWM = motors.speed;
	}
	else if (rightBits > leftBits)
	{
		leftPWM = motors.speed;
		rightPWM = motors.speed / rightBits;
	}
	else
	{
		leftPWM = motors.speed;
		rightPWM = motors.speed;
	}

	motors.motorsTurn(leftPWM, rightPWM);

	return;
}
