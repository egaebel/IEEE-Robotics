
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

//Line Sensor pins
static const short Load    = 22; // Controls ShiftRegister's Shift/Load pin(1)
static const short sensor  = 24; // Controls LineFollower's Enable pin
static const short L_side_pin = 10; // PWM pin for left trek
static const short R_side_pin = 9;  // PWM pin for right trek
static const short Dir_Right_Side  =  30; // Controls direction of the right track(LOW - forward, HIGH - Reverse)
static const short Dir_Left_Side  =   31; // Controls direction of the left track (LOW - forward, HIGH - Reverse)

//Setup variables etc
void setup() {

	Serial.begin(9600);
	delay(4000);
	Serial.print("BEGINNINGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG\n");
	//init all hardware
	motors.setup(255);
	motors.motorsStop();
	//LineFollower variables setup
	SPI.begin();
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	SPI.setDataMode(SPI_MODE3);
	SPI.setBitOrder(MSBFIRST);

	pinMode(Load, OUTPUT);
	digitalWrite(Load, HIGH);
	pinMode(sensor, OUTPUT);
	digitalWrite(sensor, HIGH); //activates LineFollower Kit

	pinMode(L_side_pin, OUTPUT);
	pinMode(R_side_pin, OUTPUT);
	pinMode(Dir_Right_Side, OUTPUT);
	pinMode(Dir_Left_Side, OUTPUT);

	//Variables setup
	state = MAIN_LINE;
	lineCount = 0;
	Serial.print("Delaying....\n");
	motors.motorsDrive(FORWARD);
	delay(10000);
	Serial.print("STOPP....\n");
	motors.motorsStop();
	delay(10000);
}

//Loop
void loop() {

	switch(state) {

		case MAIN_LINE:
			Serial.println("MAIN LINE");
			motors.motorsDrive(FORWARD);
			if (lineFollower.intersection(leftLineFollowBits, rightLineFollowBits)) {
				Serial.println("INTERSECTION!");
				//followTheLine(leftLineFollowBits, rightLineFollowBits);
				motors.motorsStop();
				Serial.println("Turning LEFT");
				motors.motorsTurnLeft();
				delay(10000);
				motors.motorsStop();
				
				//Switch states depending on which line we've discovered
				if (lineCount == 0 || lineCount == 2) {
					Serial.println("On  one of the straight lines");
					state = STRAIGHT_LINE_START;
					lineCount++;
				}
				else if (lineCount == 1) {
					//state = CURVED_LINE;
					lineCount++;
				}
			}
			//followTheLine(leftLineFollowBits, rightLineFollowBits);
			break;
		case STRAIGHT_LINE_START:
			Serial.println("ON A SIDE STRAIGHT LINE");
			delay(5000);
			motors.motorsDrive(FORWARD);
			delay(3000);
			//lineFollower.Get_Line_Data(leftLineFollowBits, rightLineFollowBits);
			//followTheLine(leftLineFollowBits, rightLineFollowBits);
			motors.motorsStop();
			state = FIRE;
			break;
		case STRAIGHT_LINE_END:
			Serial.println("IN THE STRAIGHT_LINE_END STATE");
			delay(5000);
			motors.motorsDrive(FORWARD);
			if (lineFollower.intersection(leftLineFollowBits, rightLineFollowBits)) {
				Serial.println("INTERSECTION with the MAIN LINE...turning!\n");
				state = MAIN_LINE;
				motors.motorsTurnLeft();
				delay(10000);
				motors.motorsStop();
				state = MAIN_LINE;
			}
			//followTheLine(leftLineFollowBits, rightLineFollowBits);
			break;
		case FIRE:
			delay(4000);
			Serial.println("IN 'FIRE' STATE! Making uTURN!");
			motors.motorsStop();
			motors.motorsUTurn();
			motors.motorsStop();
			state = STRAIGHT_LINE_END;
			break;
		case CURVED_LINE_START:
			break;
		case CURVED_LINE_END:
			break;
	}
	delay(8000);
//*/
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
