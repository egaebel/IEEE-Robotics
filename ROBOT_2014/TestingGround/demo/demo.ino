#include <motors.h>
#include <linefollow.hpp>

//enum State { MAIN_LINE = 1, STRAIGHT_LINE_START, FIRE, STRAIGHT_LINE_END, CURVED_LINE_START, CURVED_LINE_END };

typedef enum
{
  FOLLOW_STRAIGHT_LINE,
  TURN_LEFT  
} j_state;

//Hardware interfaces
static Motors motors;
static LineFollower lineFollower;

//
//static State state;
static j_state j_s = FOLLOW_STRAIGHT_LINE;
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
	Serial.print("BEGINNING\n");
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
	//state = MAIN_LINE;
	lineCount = 0;
	Serial.print("Delaying....\n");
	Serial.print("STOPP....\n");
	motors.motorsStop();
	delay(5000);
}

//Loop
void loop() {

  /*
	Serial.print("STOPP....\n");
	motors.motorsStop();
	Serial.println("Going Forward");
	motors.motorsDrive(FORWARD);
	delay(3000);
	Serial.println("Going Backward");
	motors.motorsDrive(BACKWARD);
	delay(3000);
	Serial.println("Turning Left");
	motors.motorsTurnLeft();
	delay(3000);
	Serial.println("Turning RIGHT");
	motors.motorsTurnRight();
	delay(3000);
*/

switch (j_s)
{
        case FOLLOW_STRAIGHT_LINE:
            if (lineFollower.isCentered(leftLineFollowBits, rightLineFollowBits))
            {
                motors.motorsDrive(FORWARD); break;
            }
            else if (lineFollower.intersection(leftLineFollowBits, rightLineFollowBits))
            {
                j_s = TURN_LEFT; break; 
            }
            else
            {
                if (!lineFollower.isCentered(leftLineFollowBits, rightLineFollowBits) &&(leftLineFollowBits > rightLineFollowBits))
                    motors.motorsTurnLeft();
                if (!lineFollower.isCentered(leftLineFollowBits, rightLineFollowBits) &&(leftLineFollowBits < rightLineFollowBits))
                    motors.motorsTurnRight(); 
                j_s = FOLLOW_STRAIGHT_LINE; break;
            }
         case TURN_LEFT:
             delay(500);
             do {
                 motors.motorsTurnLeft();
                 lineFollower.Get_Line_Data(leftLineFollowBits, rightLineFollowBits);
             } while (leftLineFollowBits || rightLineFollowBits);
             
             do {
                 motors.motorsTurnLeft();
                 lineFollower.Get_Line_Data(leftLineFollowBits, rightLineFollowBits);
             } while (!leftLineFollowBits && !rightLineFollowBits);
             
             j_s = FOLLOW_STRAIGHT_LINE;
             break;
}


/*	switch(state) {

		case MAIN_LINE:
			Serial.println("MAIN LINE");
			motors.motorsDrive(FORWARD);
			if (lineFollower.intersection(leftLineFollowBits, rightLineFollowBits)) {
				Serial.println("INTERSECTION!");

				//followTheLine(leftLineFollowBits, rightLineFollowBits, true);
				followTheLine(leftLineFollowBits, rightLineFollowBits);

				Serial.println("Entering isCentered Loop..");
				//Follow the line along the turn until we're done
				while (!lineFollower.isCentered(leftLineFollowBits, rightLineFollowBits)) {
					Serial.println("Looping in isCentered...");
					//followTheLine(leftLineFollowBits, rightLineFollowBits, true);
                                        followTheLine(leftLineFollowBits, rightLineFollowBits);
				}
				Serial.println("Left bits == " + leftLineFollowBits);   
   				Serial.println("Right bits == " + rightLineFollowBits);
				
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
			//Remove later in favor of navigation to Blue firing block
			motors.motorsDrive(FORWARD);
			//lineFollower.Get_Line_Data(leftLineFollowBits, rightLineFollowBits);
			//followTheLine(leftLineFollowBits, rightLineFollowBits);
			motors.motorsStop();
			state = FIRE;
			break;
		case FIRE:
			delay(4000);
			Serial.println("IN 'FIRE' STATE! Making uTURN!");
			motors.motorsStop();
			motors.motorsUTurn();
			motors.motorsStop();
			state = STRAIGHT_LINE_END;
			break;
		case STRAIGHT_LINE_END:
			Serial.println("IN THE STRAIGHT_LINE_END STATE");
			motors.motorsDrive(FORWARD);
			if (lineFollower.intersection(leftLineFollowBits, rightLineFollowBits)) {
				
				Serial.println("INTERSECTION with the MAIN LINE...turning!\n");
				//followTheLine(leftLineFollowBits, rightLineFollowBits, true);
                                followTheLine(leftLineFollowBits, rightLineFollowBits);

				Serial.println("Entering isCentered Loop..");
				//Follow the line along the turn until we're done
				while (!lineFollower.isCentered(leftLineFollowBits, rightLineFollowBits)) {
					Serial.println("Looping in isCentered...");
					//followTheLine(leftLineFollowBits, rightLineFollowBits, true);
                                          followTheLine(leftLineFollowBits, rightLineFollowBits);
				}
				state = MAIN_LINE;
			}
			//followTheLine(leftLineFollowBits, rightLineFollowBits);
			break;
		case CURVED_LINE_START:
			break;
		case CURVED_LINE_END:
			break;
	}
*/
}

void followTheLine(byte leftBits, byte rightBits)
{
	short leftPWM = 0;
	short rightPWM = 0;
	Turn turnDirection;

	if (leftBits >= rightBits)
	{
		Serial.println("going left....");
		leftPWM = motors.speed / leftBits;
		rightPWM = motors.speed;
                motors.motorsTurnLeft();
	}
	else if (rightBits > leftBits)
	{
		Serial.println("going right....");
		leftPWM = motors.speed;
		rightPWM = motors.speed / rightBits;
                motors.motorsTurnRight();
	}
	else
	{
		leftPWM = motors.speed;
		rightPWM = motors.speed;
                motors.motorsDrive(FORWARD);
	}


	return;
}
