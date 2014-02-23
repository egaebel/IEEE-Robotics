
#include "motors.h"
#include "C:\Users\ethan\Documents\ACTUAL My Documents\Robotics Team\IEEE-Robotics\ROBOT_2014\TestingGround\demo\linefollow.hpp"

enum State { MAIN_LINE = 1, STRAIGHT_LINE_START, FIRE, STRAIGHT_LINE_END, CURVED_LINE_START, CURVED_LINE_END };

//Hardware interfaces
static Motors motors;
static LineFollower lineFollower;

//
static State state;
static byte leftLineFollowBits;
static byte rightLineFollowBits;
static int lineCount;

int main() {

	setup();
	loop();
}

//Setup variables etc
void setup() {

	Serial.begin(9600);
	//init all hardware
	motors.setup(255);

	state = MAIN_LINE;
	lineCount = 0;
	delay(3);
}

//Loop
void loop() {

	switch(state) {

		case MAIN_LINE:
			if (lineFollower.intersection(leftLineFollowBits, rightLineFollowBits)) {
				followTheLine(leftLineFollowBits, rightLineFollowBits);
				
				//Switch states depending on which line we've discovered
				if (lineCount == 0 || lineCount == 2) {
					state = STRAIGHT_LINE_START;
					lineCount++;
				}
				else if (lineCount == 1) {
					//state = CURVED_LINE;
					lineCount++;
				}
			}
			followTheLine(leftLineFollowBits, rightLineFollowBits);
			break;
		case STRAIGHT_LINE_START:
			lineFollower.Get_Line_Data(leftLineFollowBits, rightLineFollowBits);
			followTheLine(leftLineFollowBits, rightLineFollowBits);
			delay(7);
			state = FIRE;
			break;
		case STRAIGHT_LINE_END:
			if (lineFollower.intersection(leftLineFollowBits, rightLineFollowBits)) {
				state = MAIN_LINE;
			}
			followTheLine(leftLineFollowBits, rightLineFollowBits);
			break;
		case FIRE:
			delay(3);
			motors.motorsUTurn();
			break;
		case CURVED_LINE_START:
			break;
		case CURVED_LINE_END:
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
